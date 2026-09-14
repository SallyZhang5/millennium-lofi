/*
    Millennium Lo-Fi audio engine.

    Pure C++ (no JUCE) so the exact same code can be compiled into the plugin and
    into the offline test renderer used by CI and by the developer's null tests.

    Signal chain, in the same order as the original offline tool:

        stereo width  ->  tape wow/flutter  ->  saturation  ->  tape bandwidth
        ->  hiss + hum  ->  digitisation (sample rate + bit depth)  ->  codec
        ->  small speaker  ->  room  ->  output trim
*/

#pragma once

#include "DspUtils.h"

namespace mlofi
{

struct Params
{
    float bitDepth = 13.0f;          // 4 .. 16, 16 = off
    float sampleRate = 22050.0f;     // 4000 .. 48000, >= host rate = off
    float tapeLowpass = 11000.0f;    // Hz
    float wow = 0.16f;               // 0 .. 1
    float drive = 0.18f;             // 0 .. 1
    float hissDb = -74.0f;           // -90 .. -20
    float humDb = -90.0f;            // -90 .. -30
    int   codecKbps = 64;            // 0 = off, otherwise 24 .. 320
    float speakerHighpass = 90.0f;
    float speakerLowpass = 8200.0f;
    float resonance = 0.18f;
    float reverb = 0.10f;
    float width = 0.66f;
    float outputDb = 0.0f;
};

/*  Codec band limits (Hz at 44.1 kHz) for the bitrate list used by the UI. */
inline float codecCutoffFor (int kbps) noexcept
{
    switch (kbps)
    {
        case 0:   return 0.0f;      // off
        case 24:  return 4200.0f;
        case 32:  return 5500.0f;
        case 48:  return 7500.0f;
        case 64:  return 10500.0f;
        case 96:  return 13500.0f;
        case 128: return 15500.0f;
        case 192: return 17500.0f;
        case 320: return 20000.0f;
        default:  return 11000.0f;
    }
}

class LoFiEngine
{
public:
    void prepare (double sampleRate, int /*maxBlockSize*/, int channels)
    {
        fs = sampleRate > 1000.0 ? sampleRate : 44100.0;
        numChannels = channels < 1 ? 1 : (channels > 2 ? 2 : channels);
        channelState.clear();
        channelState.resize (static_cast<std::size_t> (numChannels));
        for (int ch = 0; ch < numChannels; ++ch)
            prepareChannel (channelState[static_cast<std::size_t> (ch)], ch);

        driftValue = 0.0f;
        modOffset = 0.0f;
        sampleClock = 0;
        reverb.prepare (fs, numChannels);
        reverb.setDecaySeconds (0.42);
        reverb.setDamping (3200.0);
        lastCodecKbps = -1;
        lastTape = lastSpeakerHi = lastSpeakerLo = lastRes = lastSampleRate = -1.0f;
    }

    void reset()
    {
        for (auto& s : channelState)
            resetChannel (s);
        reverb.reset();
        modOffset = 0.0f;
    }

    /*  Reported to the host; the approximation codec needs no lookahead. */
    int latencySamples() const noexcept { return 0; }

    void process (float* const* channels, int numChannelsIn, int numSamples, const Params& p)
    {
        if (numSamples <= 0 || channelState.empty() || channels == nullptr)
            return;

        updateFilters (p);

        const bool stereo = numChannelsIn > 1 && channels[1] != nullptr
                            && channelState.size() > 1;
        const float codecSide = p.codecKbps > 0 ? (p.codecKbps <= 48 ? 0.55f
                                                 : p.codecKbps <= 96 ? 0.8f : 1.0f)
                                                : 1.0f;
        const float widthNow = clampf (p.width * codecSide, 0.0f, 1.0f);
        const float wowAmount = p.wow;
        const float driveGain = 1.0f + 9.0f * p.drive;
        const float driveNorm = p.drive > 0.001f ? 1.0f / std::tanh (driveGain) : 1.0f;
        const float wetMix = 0.05f + 0.55f * p.reverb;
        const float dryGain = 1.0f - wetMix * 0.6f;
        const float outGain = dbToGain (p.outputDb);

        /* per block modulation step */
        const double invFs = 1.0 / fs;

        for (int i = 0; i < numSamples; ++i)
        {
            float left = channels[0][i];
            float right = stereo ? channels[1][i] : left;
            const float dryLeft = left;
            const float dryRight = right;

            /*  never let a bad sample from the host poison the filter states */
            if (! std::isfinite (left))  left = 0.0f;
            if (! std::isfinite (right)) right = 0.0f;

            if (stereo)
            {
                const float mid = (left + right) * 0.5f;
                const float side = (left - right) * 0.5f * widthNow;
                left = mid + side;
                right = mid - side;
            }

            /* one shared tape transport for both channels */
            const double t = sampleClock * invFs;
            const double wowMod = wowAmount * (0.0030 * std::sin (2.0 * kPi * 0.63 * t + 0.4)
                                            + 0.0016 * std::sin (2.0 * kPi * 1.77 * t + 2.3));
            const double flutterMod = wowAmount * (0.0009 * std::sin (2.0 * kPi * 8.7 * t + 1.1)
                                                + 0.0006 * std::sin (2.0 * kPi * 12.4 * t + 0.2));
            /* slow random drift: one pole filtered noise, integrated below */
            const float driftNoise = driftRng.nextBipolar();
            driftValue += (driftNoise - driftValue) * 0.00035f;
            if (! std::isfinite (driftValue))
                driftValue = 0.0f;
            driftValue = clampf (driftValue, -0.05f, 0.05f);

            /*  The wow and flutter sines are integrated into a position offset
                (that is what makes the pitch wobble). The slow drift is added
                directly as a small position wobble instead: integrating noise
                would let the offset wander off and read outside the delay line,
                which is what produced the random harsh tone. */
            modOffset += static_cast<float> (wowMod + flutterMod);
            if (! std::isfinite (modOffset))
                modOffset = 0.0f;
            modOffset = clampf (modOffset, -1.5f, 1.5f);
            ++sampleClock;

            const float driftOffset = wowAmount * driftValue * 2.0f;
            float outL = processChannel (0, left, p, driftOffset, driveGain, driveNorm);
            float outR = stereo ? processChannel (1, right, p, driftOffset, driveGain, driveNorm)
                                : outL;

            const float wetL = reverb.process (0, outL);
            const float wetR = reverb.process (stereo ? 1 : 0, outR);

            outL = dryGain * outL + wetMix * wetL;
            outR = dryGain * outR + wetMix * wetR;

            outL = softClip (outL * outGain);
            outR = softClip (outR * outGain);

            /*  last line of defence: if anything ever goes non-finite, clear the
                affected state instead of letting a tone or noise burst through */
            if (! std::isfinite (outL) || ! std::isfinite (outR))
            {
                for (auto& s : channelState)
                    resetChannel (s);
                reverb.reset();
                outL = 0.0f;
                outR = 0.0f;
            }

            (void) dryLeft;
            (void) dryRight;
            channels[0][i] = outL;
            if (stereo)
                channels[1][i] = outR;
        }
    }

private:
    static constexpr double kPi = 3.14159265358979323846;

    struct ChannelState
    {
        DelayLine wowLine;
        Butterworth tapeLowpass;
        Biquad tapeShelf;
        Butterworth tapeHighpass;
        Butterworth antiAlias;
        Butterworth codecBand;
        Butterworth speakerLowpass;
        Butterworth speakerHighpass;
        Biquad speakerPeakLow;
        Biquad speakerPeakMid;
        Biquad hissHighpass;
        Biquad hissLowpass;
        Rng hissRng { 911u };
        Rng ditherRng { 4242u };
        /*  the sample rate reducer needs its own state per channel, otherwise
            the two channels overwrite each other's interpolator */
        double resamplePhase = 0.0;
        float heldPrevious = 0.0f;
        float heldCurrent = 0.0f;
    };

    void prepareChannel (ChannelState& s, int index)
    {
        s.wowLine.prepare (64);
        /*  orders match the offline reference implementation so the two sound
            like the same machine */
        s.tapeLowpass.setupLowpass (fs, 8000.0, 2);
        s.tapeShelf = Biquad::lowShelf (fs, 110.0, 2.5, 0.8);
        s.tapeHighpass.setupHighpass (fs, 28.0, 2);
        s.antiAlias.setupLowpass (fs, 10000.0, 4);
        s.codecBand.setupLowpass (fs, 12800.0, 8);
        s.speakerLowpass.setupLowpass (fs, 6000.0, 2);
        s.speakerHighpass.setupHighpass (fs, 200.0, 2);
        s.speakerPeakLow = Biquad::peaking (fs, 620.0, 1.4, 1.0);
        s.speakerPeakMid = Biquad::peaking (fs, 1900.0, 1.1, 3.0);
        s.hissHighpass = Biquad::highpass (fs, 900.0, 0.7);
        s.hissLowpass = Biquad::lowpass (fs, 9000.0, 0.7);
        s.hissRng = Rng (911u + static_cast<std::uint32_t> (index) * 7919u);
        s.ditherRng = Rng (4242u + static_cast<std::uint32_t> (index) * 104729u);
        s.resamplePhase = 0.0;
        s.heldPrevious = 0.0f;
        s.heldCurrent = 0.0f;
        resetChannel (s);
    }

    static void resetChannel (ChannelState& s)
    {
        s.wowLine.reset();
        s.tapeLowpass.reset();
        s.tapeShelf.reset();
        s.tapeHighpass.reset();
        s.antiAlias.reset();
        s.codecBand.reset();
        s.speakerLowpass.reset();
        s.speakerHighpass.reset();
        s.speakerPeakLow.reset();
        s.speakerPeakMid.reset();
        s.hissHighpass.reset();
        s.hissLowpass.reset();
        s.resamplePhase = 0.0;
        s.heldPrevious = 0.0f;
        s.heldCurrent = 0.0f;
    }

    void updateFilters (const Params& p)
    {
        if (std::abs (p.tapeLowpass - lastTape) > 1.0f)
        {
            lastTape = p.tapeLowpass;
            for (auto& s : channelState)
                s.tapeLowpass.setupLowpass (fs, std::max (p.tapeLowpass, 200.0f), 2);
        }

        if (std::abs (p.sampleRate - lastSampleRate) > 1.0f)
        {
            lastSampleRate = p.sampleRate;
            const float top = std::min (p.sampleRate * 0.46f, static_cast<float> (fs * 0.45));
            for (auto& s : channelState)
                s.antiAlias.setupLowpass (fs, std::max (top, 300.0f), 4);
        }

        if (p.codecKbps != lastCodecKbps)
        {
            lastCodecKbps = p.codecKbps;
            const float cutoff = codecCutoffFor (p.codecKbps) * static_cast<float> (fs / 44100.0);
            for (auto& s : channelState)
                s.codecBand.setupLowpass (fs, cutoff > 0.0f ? cutoff : static_cast<float> (fs * 0.45), 4);
        }

        if (std::abs (p.speakerHighpass - lastSpeakerHi) > 1.0f)
        {
            lastSpeakerHi = p.speakerHighpass;
            for (auto& s : channelState)
                s.speakerHighpass.setupHighpass (fs, std::max (p.speakerHighpass, 20.0f), 2);
        }

        if (std::abs (p.speakerLowpass - lastSpeakerLo) > 1.0f)
        {
            lastSpeakerLo = p.speakerLowpass;
            for (auto& s : channelState)
                s.speakerLowpass.setupLowpass (fs, std::max (p.speakerLowpass, 800.0f), 2);
        }

        if (std::abs (p.resonance - lastRes) > 0.002f)
        {
            lastRes = p.resonance;
            const double lowGain = 1.0 + 3.0 * p.resonance;
            const double midGain = 2.0 + 7.0 * p.resonance;
            for (auto& s : channelState)
            {
                s.speakerPeakLow = Biquad::peaking (fs, 620.0, 1.4, lowGain);
                s.speakerPeakMid = Biquad::peaking (fs, 1900.0, 1.1, midGain);
            }
        }
    }

    inline float processChannel (int index, float x, const Params& p,
                                 float driftOffset, float driveGain, float driveNorm) noexcept
    {
        ChannelState& s = channelState[static_cast<std::size_t> (index)];

        /* --- tape transport (wow / flutter) --------------------------- */
        const float readDelay = 4.0f + modOffset + driftOffset;
        const float wowOut = s.wowLine.read (readDelay);
        s.wowLine.push (x);
        float y = wowOut;

        /* --- saturation ----------------------------------------------- */
        if (p.drive > 0.001f)
            y = std::tanh (driveGain * y) * driveNorm;

        /* --- tape bandwidth ------------------------------------------- */
        if (p.tapeLowpass < fs * 0.45f)
        {
            y = s.tapeHighpass.process (y);
            y = s.tapeShelf.process (y);
            y = s.tapeLowpass.process (y);
        }

        /* --- hiss and hum --------------------------------------------- */
        if (p.hissDb > -89.0f)
        {
            const float noise = s.hissRng.nextBipolar();
            const float shaped = s.hissLowpass.process (s.hissHighpass.process (noise));
            y += shaped * dbToGain (p.hissDb) * 3.2f;
        }
        if (p.humDb > -89.0f)
            y += humAmount (p.humDb);

        /* --- digitisation --------------------------------------------- */
        const float targetRate = std::min (p.sampleRate, static_cast<float> (fs));
        if (targetRate < static_cast<float> (fs) * 0.99f)
        {
            const float filtered = s.antiAlias.process (y);
            s.resamplePhase += static_cast<double> (targetRate) / fs;
            if (s.resamplePhase >= 1.0)
            {
                s.resamplePhase -= 1.0;
                s.heldPrevious = s.heldCurrent;
                s.heldCurrent = filtered;
            }
            y = s.heldPrevious + (s.heldCurrent - s.heldPrevious) * static_cast<float> (s.resamplePhase);
        }

        if (p.bitDepth < 15.95f)
        {
            const int bits = static_cast<int> (clampf (p.bitDepth, 2.0f, 16.0f));
            const float levels = std::pow (2.0f, static_cast<float> (bits - 1));
            if (bits <= 12)
                y += (s.ditherRng.nextBipolar() * 0.5f) / levels;
            y = std::round (clampf (y, -1.2f, 1.2f) * levels) / levels;
        }

        /* --- lossy codec band limit ----------------------------------- */
        if (p.codecKbps > 0)
            y = s.codecBand.process (y);

        /* --- small plastic speaker ------------------------------------ */
        if (p.speakerHighpass > 21.0f)
            y = s.speakerHighpass.process (y);
        if (p.speakerLowpass < fs * 0.44f)
            y = s.speakerLowpass.process (y);
        if (p.resonance > 0.001f)
        {
            y = s.speakerPeakLow.process (y);
            y = s.speakerPeakMid.process (y);
        }

        return y;
    }

    inline float humAmount (float humDb) noexcept
    {
        const double t = sampleClock * (1.0 / fs);
        const double base = std::sin (2.0 * kPi * 50.0 * t)
                          + 0.35 * std::sin (2.0 * kPi * 150.0 * t + 0.6)
                          + 0.15 * std::sin (2.0 * kPi * 250.0 * t + 1.2);
        return static_cast<float> (base / 1.5 * 1.0) * dbToGain (humDb);
    }

    static inline float softClip (float x) noexcept
    {
        /*  fully transparent below 0.9, asymptotically approaches 1.0 above it
            so the plugin output can never leave the valid range */
        const float magnitude = std::abs (x);
        if (magnitude <= 0.9f)
            return x;
        const float over = (magnitude - 0.9f) * 10.0f;
        const float shaped = 0.9f + 0.1f * std::tanh (over);
        return x < 0.0f ? -shaped : shaped;
    }

    double fs = 44100.0;
    int numChannels = 2;
    long long sampleClock = 0;

    std::vector<ChannelState> channelState;
    RoomReverb reverb;

    Rng driftRng { 2000u };
    float driftValue = 0.0f;
    float modOffset = 0.0f;

    float lastTape = -1.0f, lastSpeakerHi = -1.0f, lastSpeakerLo = -1.0f, lastRes = -1.0f;
    float lastSampleRate = -1.0f;
    int lastCodecKbps = -1;
};

} // namespace mlofi
