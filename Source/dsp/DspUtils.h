/*
    Small self contained DSP helpers.

    These live outside the JUCE layer on purpose: the whole audio engine can be
    compiled and tested on its own (see Test/render_test.cpp), which keeps the
    sound identical between the offline renderer and the real time plugin.
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdint>
#include <vector>

namespace mlofi
{

inline float dbToGain (float db) noexcept { return std::pow (10.0f, db * 0.05f); }

inline float gainToDb (float gain) noexcept
{
    return 20.0f * std::log10 (std::max (gain, 1.0e-7f));
}

inline float clampf (float v, float lo, float hi) noexcept
{
    return v < lo ? lo : (v > hi ? hi : v);
}

/*  Deterministic noise source.  Every instance owns its own state so the plugin
    never shares random numbers with the offline renderer by accident. */
class Rng
{
public:
    explicit Rng (std::uint32_t seed = 22222u) : state (seed | 1u) {}

    std::uint32_t nextUInt() noexcept
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    /*  uniform in [-1, 1) */
    float nextBipolar() noexcept
    {
        return static_cast<float> (nextUInt() >> 8) * (2.0f / 16777216.0f) - 1.0f;
    }

    /*  uniform in [0, 1) */
    float nextUnipolar() noexcept
    {
        return static_cast<float> (nextUInt() >> 8) * (1.0f / 16777216.0f);
    }

private:
    std::uint32_t state;
};

/*  Transposed direct form II biquad, RBJ cookbook coefficients. */
class Biquad
{
public:
    void reset() noexcept { s1 = s2 = 0.0f; }

    inline float process (float x) noexcept
    {
        const float y = b0 * x + s1;
        s1 = b1 * x - a1 * y + s2;
        s2 = b2 * x - a2 * y;
        return y;
    }

    void setCoefficients (float nb0, float nb1, float nb2, float na1, float na2) noexcept
    {
        b0 = nb0; b1 = nb1; b2 = nb2; a1 = na1; a2 = na2;
    }

    static Biquad lowpass (double fs, double f0, double q)
    {
        Biquad b;
        const double w = 2.0 * 3.14159265358979323846 * std::min (f0, fs * 0.49) / fs;
        const double cw = std::cos (w), sw = std::sin (w);
        const double alpha = sw / (2.0 * std::max (q, 0.05));
        const double a0 = 1.0 + alpha;
        b.setCoefficients (float (((1.0 - cw) * 0.5) / a0),
                           float ((1.0 - cw) / a0),
                           float (((1.0 - cw) * 0.5) / a0),
                           float ((-2.0 * cw) / a0),
                           float ((1.0 - alpha) / a0));
        return b;
    }

    static Biquad highpass (double fs, double f0, double q)
    {
        Biquad b;
        const double w = 2.0 * 3.14159265358979323846 * std::max (f0, 5.0) / fs;
        const double cw = std::cos (w), sw = std::sin (w);
        const double alpha = sw / (2.0 * std::max (q, 0.05));
        const double a0 = 1.0 + alpha;
        b.setCoefficients (float (((1.0 + cw) * 0.5) / a0),
                           float ((-(1.0 + cw)) / a0),
                           float (((1.0 + cw) * 0.5) / a0),
                           float ((-2.0 * cw) / a0),
                           float ((1.0 - alpha) / a0));
        return b;
    }

    static Biquad peaking (double fs, double f0, double q, double gainDb)
    {
        Biquad b;
        const double A = std::pow (10.0, gainDb / 40.0);
        const double w = 2.0 * 3.14159265358979323846 * std::min (f0, fs * 0.49) / fs;
        const double cw = std::cos (w), sw = std::sin (w);
        const double alpha = sw / (2.0 * std::max (q, 0.05));
        const double a0 = 1.0 + alpha / A;
        b.setCoefficients (float ((1.0 + alpha * A) / a0),
                           float ((-2.0 * cw) / a0),
                           float ((1.0 - alpha * A) / a0),
                           float ((-2.0 * cw) / a0),
                           float ((1.0 - alpha / A) / a0));
        return b;
    }

    static Biquad lowShelf (double fs, double f0, double gainDb, double slope)
    {
        Biquad b;
        const double A = std::pow (10.0, gainDb / 40.0);
        const double w = 2.0 * 3.14159265358979323846 * std::max (f0, 5.0) / fs;
        const double cw = std::cos (w), sw = std::sin (w);
        const double alpha = sw * 0.5 * std::sqrt ((A + 1.0 / A) * (1.0 / std::max (slope, 0.05) - 1.0) + 2.0);
        const double twoSqrtAalpha = 2.0 * std::sqrt (A) * alpha;
        const double a0 = (A + 1.0) + (A - 1.0) * cw + twoSqrtAalpha;
        b.setCoefficients (float ((A * ((A + 1.0) - (A - 1.0) * cw + twoSqrtAalpha)) / a0),
                           float ((2.0 * A * ((A - 1.0) - (A + 1.0) * cw)) / a0),
                           float ((A * ((A + 1.0) - (A - 1.0) * cw - twoSqrtAalpha)) / a0),
                           float ((-2.0 * ((A - 1.0) + (A + 1.0) * cw)) / a0),
                           float (((A + 1.0) + (A - 1.0) * cw - twoSqrtAalpha) / a0));
        return b;
    }

    static Biquad highShelf (double fs, double f0, double gainDb, double slope)
    {
        Biquad b;
        const double A = std::pow (10.0, gainDb / 40.0);
        const double w = 2.0 * 3.14159265358979323846 * std::min (f0, fs * 0.49) / fs;
        const double cw = std::cos (w), sw = std::sin (w);
        const double alpha = sw * 0.5 * std::sqrt ((A + 1.0 / A) * (1.0 / std::max (slope, 0.05) - 1.0) + 2.0);
        const double twoSqrtAalpha = 2.0 * std::sqrt (A) * alpha;
        const double a0 = (A + 1.0) - (A - 1.0) * cw + twoSqrtAalpha;
        b.setCoefficients (float ((A * ((A + 1.0) + (A - 1.0) * cw + twoSqrtAalpha)) / a0),
                           float ((-2.0 * A * ((A - 1.0) + (A + 1.0) * cw)) / a0),
                           float ((A * ((A + 1.0) + (A - 1.0) * cw - twoSqrtAalpha)) / a0),
                           float ((2.0 * ((A - 1.0) - (A + 1.0) * cw)) / a0),
                           float (((A + 1.0) - (A - 1.0) * cw - twoSqrtAalpha) / a0));
        return b;
    }

private:
    float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    float s1 = 0.0f, s2 = 0.0f;
};

/*  Butterworth style cascade (order 2n). */
class Butterworth
{
public:
    void setupLowpass (double fs, double f0, int order)
    {
        sections.clear();
        for (int i = 0; i < order / 2; ++i)
            sections.push_back (Biquad::lowpass (fs, f0, butterQ (order, i)));
        for (auto& s : sections) s.reset();
    }

    void setupHighpass (double fs, double f0, int order)
    {
        sections.clear();
        for (int i = 0; i < order / 2; ++i)
            sections.push_back (Biquad::highpass (fs, f0, butterQ (order, i)));
        for (auto& s : sections) s.reset();
    }

    void reset() { for (auto& s : sections) s.reset(); }

    inline float process (float x) noexcept
    {
        for (auto& s : sections) x = s.process (x);
        return x;
    }

private:
    static double butterQ (int order, int index)
    {
        const double n = static_cast<double> (order);
        const double theta = 3.14159265358979323846 * (2.0 * index + 1.0) / (2.0 * n);
        return 1.0 / (2.0 * std::sin (theta));
    }

    std::vector<Biquad> sections;
};

/*  Simple fractional delay line. */
class DelayLine
{
public:
    void prepare (int maxSamples)
    {
        buffer.assign (static_cast<std::size_t> (std::max (maxSamples, 8)), 0.0f);
        writePos = 0;
    }

    void reset() { std::fill (buffer.begin(), buffer.end(), 0.0f); writePos = 0; }

    inline void push (float x) noexcept
    {
        buffer[static_cast<std::size_t> (writePos)] = x;
        if (++writePos >= static_cast<int> (buffer.size()))
            writePos = 0;
    }

    /*  delay in samples, may be fractional */
    inline float read (float delaySamples) const noexcept
    {
        const int size = static_cast<int> (buffer.size());
        float pos = static_cast<float> (writePos) - 1.0f - delaySamples;
        while (pos < 0.0f)
            pos += static_cast<float> (size);
        const int i0 = static_cast<int> (pos);
        const float frac = pos - static_cast<float> (i0);
        const int i1 = (i0 + 1) % size;
        return buffer[static_cast<std::size_t> (i0)] * (1.0f - frac)
             + buffer[static_cast<std::size_t> (i1)] * frac;
    }

private:
    std::vector<float> buffer;
    int writePos = 0;
};

/*  Schroeder style room: four damped combs into two allpasses, one per channel.

    Deliberately small and cheap — it lives on the audio thread. Decay and damping
    are chosen to match the room used by the offline renderer. */
class RoomReverb
{
public:
    void prepare (double sampleRate, int channels)
    {
        fs = sampleRate;
        states.clear();
        for (int ch = 0; ch < std::max (1, channels); ++ch)
        {
            State s;
            /* offset the right channel so the two sides do not correlate */
            const double spread = ch == 0 ? 1.0 : 1.07;
            static const double combMs[4] = { 29.7, 37.1, 41.1, 43.7 };
            static const double apMs[2] = { 5.0, 1.7 };
            for (int i = 0; i < 4; ++i)
            {
                const auto len = static_cast<int> (sampleRate * combMs[i] * spread / 1000.0);
                s.comb[i].prepare (len + 8);
                s.combDelay[i] = static_cast<float> (len);
            }
            for (int i = 0; i < 2; ++i)
            {
                const auto len = static_cast<int> (sampleRate * apMs[i] * spread / 1000.0);
                s.allpass[i].prepare (len + 8);
                s.allpassDelay[i] = static_cast<float> (len);
            }
            s.damp.reset();
            s.early.prepare (static_cast<int> (sampleRate * 0.08) + 8);
            for (int i = 0; i < 5; ++i)
                s.earlyDelay[i] = static_cast<float> (sampleRate * (0.007 + 0.011 * i));
            states.push_back (std::move (s));
        }
        updateDamping();
    }

    void reset()
    {
        for (auto& s : states)
        {
            for (auto& c : s.comb) c.reset();
            for (auto& a : s.allpass) a.reset();
            s.damp.reset();
            s.early.reset();
            s.lowState = 0.0f;
        }
    }

    void setDecaySeconds (double rt60)
    {
        decay = std::max (0.05, rt60);
        updateFeedback();
    }

    void setDamping (double cutoffHz)
    {
        dampCutoff = cutoffHz;
        updateDamping();
    }

    inline float process (int channel, float x) noexcept
    {
        if (states.empty())
            return x;
        State& s = states[static_cast<std::size_t> (channel % static_cast<int> (states.size()))];

        /* early reflections */
        float early = 0.0f;
        static const float earlyGain[5] = { 0.55f, 0.42f, 0.30f, 0.22f, 0.16f };
        for (int i = 0; i < 5; ++i)
            early += earlyGain[i] * s.early.read (s.earlyDelay[i]);
        s.early.push (x);

        float acc = 0.0f;
        for (int i = 0; i < 4; ++i)
        {
            const float delayed = s.comb[i].read (s.combDelay[i]);
            /* one pole damping inside the feedback loop */
            s.lowState = delayed * (1.0f - dampCoef) + s.lowState * dampCoef;
            acc += (delayed * 0.75f + s.lowState * 0.25f) * 0.25f;
            s.comb[i].push (x + s.lowState * feedback[i]);
        }

        float y = acc;
        for (int i = 0; i < 2; ++i)
        {
            const float d = s.allpass[i].read (s.allpassDelay[i]);
            const float out = -y + d;
            s.allpass[i].push (y + d * 0.5f);
            y = out;
        }

        return early * 0.6f + y;
    }

private:
    struct State
    {
        DelayLine comb[4];
        float combDelay[4] { 1.0f, 2.0f, 3.0f, 4.0f };
        DelayLine allpass[2];
        float allpassDelay[2] { 1.0f, 2.0f };
        DelayLine early;
        float earlyDelay[5] { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
        Biquad damp;
        float lowState = 0.0f;
    };

    void updateFeedback()
    {
        for (int i = 0; i < 4; ++i)
        {
            static const double combMs[4] = { 29.7, 37.1, 41.1, 43.7 };
            const double d = combMs[i] / 1000.0;
            feedback[i] = static_cast<float> (std::pow (10.0, -3.0 * d / decay));
        }
    }

    void updateDamping()
    {
        if (fs <= 0.0)
            return;
        /* one pole lowpass coefficient for the given cutoff */
        const double w = 2.0 * 3.14159265358979323846 * std::min (dampCutoff, fs * 0.45) / fs;
        dampCoef = static_cast<float> (std::exp (-w));
    }

    std::vector<State> states;
    double fs = 44100.0;
    double decay = 0.42;
    double dampCutoff = 3200.0;
    float dampCoef = 0.6f;
    float feedback[4] { 0.8f, 0.8f, 0.8f, 0.8f };
};

} // namespace mlofi
