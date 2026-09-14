/*
    Offline renderer / self test.

    Renders a WAV through the same engine the plugin uses and prints a small set
    of objective measurements. Used by CI after every build and by the developer
    when comparing against the original Python implementation.

    Usage:
        render_test [input.wav output.wav] [options]

    Options (defaults are the "millennium MP3 player" preset):
        --bit 13 --sr 22050 --tape 11000 --wow 0.16 --drive 0.18
        --hiss -74 --hum -90 --kbps 64 --spkhi 90 --spklo 8200
        --res 0.18 --rev 0.10 --width 0.66 --outdb 0
        --in <input.wav> --write <output.wav>
*/

#include "../Source/dsp/LoFiEngine.h"
#include "WavIo.h"

#include <cmath>
#include <complex>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace mlofi;

namespace
{

void synthesizeTestSignal (AudioFile& file, double seconds, int sampleRate)
{
    const int frames = static_cast<int> (seconds * sampleRate);
    file.sampleRate = sampleRate;
    file.channels.assign (2, std::vector<float> (static_cast<std::size_t> (frames)));

    const double twoPi = 6.283185307179586;
    for (int i = 0; i < frames; ++i)
    {
        const double t = static_cast<double> (i) / sampleRate;
        /* a pad, a bass line and a bright hat-ish noise so every stage has
           something to bite on */
        const double pad = 0.28 * std::sin (twoPi * 220.0 * t)
                         + 0.18 * std::sin (twoPi * 329.63 * t)
                         + 0.12 * std::sin (twoPi * 659.26 * t);
        const double bass = 0.30 * std::sin (twoPi * 110.0 * t);
        const double hatPhase = std::fmod (t, 0.25);
        const double hat = 0.22 * std::exp (-hatPhase * 90.0)
                         * std::sin (twoPi * 9000.0 * t);
        const double sweep = 0.10 * std::sin (twoPi * (300.0 + 9000.0 * t / seconds) * t);
        const double left = pad + bass + hat + sweep;
        const double right = pad * 0.92 + bass + hat * 0.85 + sweep * 1.05;
        file.channels[0][static_cast<std::size_t> (i)] = static_cast<float> (left * 0.6);
        file.channels[1][static_cast<std::size_t> (i)] = static_cast<float> (right * 0.6);
    }
}

/*  radix 2 FFT used only for the analysis printout */
void fft (std::vector<std::complex<double>>& a, bool inverse)
{
    const std::size_t n = a.size();
    for (std::size_t i = 1, j = 0; i < n; ++i)
    {
        std::size_t bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            std::swap (a[i], a[j]);
    }
    for (std::size_t len = 2; len <= n; len <<= 1)
    {
        const double ang = 2.0 * 3.14159265358979323846 / static_cast<double> (len) * (inverse ? 1.0 : -1.0);
        const std::complex<double> wl (std::cos (ang), std::sin (ang));
        for (std::size_t i = 0; i < n; i += len)
        {
            std::complex<double> w (1.0, 0.0);
            for (std::size_t k = 0; k < len / 2; ++k)
            {
                const std::complex<double> u = a[i + k];
                const std::complex<double> v = a[i + k + len / 2] * w;
                a[i + k] = u + v;
                a[i + k + len / 2] = u - v;
                w *= wl;
            }
        }
    }
    if (inverse)
        for (auto& v : a)
            v /= static_cast<double> (n);
}

struct Metrics
{
    double peakDb = -200.0;
    double rmsDb = -200.0;
    double centroid = 0.0;
    double highPercent = 0.0;   // energy above 8 kHz
    double lowPercent = 0.0;    // energy below 200 Hz
    bool   finite = true;
};

Metrics measure (const AudioFile& file)
{
    Metrics m;
    const int frames = file.numFrames();
    if (frames <= 0)
        return m;

    double peak = 0.0, sumSq = 0.0;
    for (int ch = 0; ch < file.numChannels(); ++ch)
        for (int i = 0; i < frames; ++i)
        {
            const double v = file.channels[static_cast<std::size_t> (ch)][static_cast<std::size_t> (i)];
            if (! std::isfinite (v))
                m.finite = false;
            peak = std::max (peak, std::abs (v));
            sumSq += v * v;
        }
    const double total = static_cast<double> (frames * file.numChannels());
    m.peakDb = 20.0 * std::log10 (std::max (peak, 1.0e-7));
    m.rmsDb = 20.0 * std::log10 (std::max (std::sqrt (sumSq / total), 1.0e-7));

    /* spectrum of the first channel, averaged over a few windows */
    const int fftSize = 4096;
    const int windows = std::max (1, std::min (16, frames / fftSize));
    std::vector<double> accum (static_cast<std::size_t> (fftSize / 2 + 1), 0.0);
    std::vector<std::complex<double>> buffer (static_cast<std::size_t> (fftSize));
    for (int w = 0; w < windows; ++w)
    {
        const int offset = w * fftSize;
        for (int i = 0; i < fftSize; ++i)
        {
            const double sample = file.channels[0][static_cast<std::size_t> (std::min (offset + i, frames - 1))];
            const double window = 0.5 - 0.5 * std::cos (2.0 * 3.14159265358979323846 * i / (fftSize - 1));
            buffer[static_cast<std::size_t> (i)] = std::complex<double> (sample * window, 0.0);
        }
        fft (buffer, false);
        for (int k = 0; k <= fftSize / 2; ++k)
            accum[static_cast<std::size_t> (k)] += std::abs (buffer[static_cast<std::size_t> (k)]);
    }

    double sum = 0.0, weighted = 0.0, high = 0.0, low = 0.0;
    for (int k = 0; k <= fftSize / 2; ++k)
    {
        const double freq = static_cast<double> (k) * file.sampleRate / fftSize;
        const double mag = accum[static_cast<std::size_t> (k)];
        sum += mag;
        weighted += mag * freq;
        if (freq > 8000.0)
            high += mag;
        if (freq < 200.0)
            low += mag;
    }
    if (sum > 0.0)
    {
        m.centroid = weighted / sum;
        m.highPercent = high / sum * 100.0;
        m.lowPercent = low / sum * 100.0;
    }
    return m;
}

void printMetrics (const char* label, const Metrics& m)
{
    std::printf ("%-22s peak %7.2f dB  rms %7.2f dB  centroid %7.0f Hz  "
                 ">8k %6.2f%%  <200Hz %6.2f%%  finite %s\n",
                 label, m.peakDb, m.rmsDb, m.centroid, m.highPercent, m.lowPercent,
                 m.finite ? "yes" : "NO");
}

/*  Stability checks.

    Feeds the engine loud transients (louder than full scale, like a clipped
    host signal) with extreme settings, then silence, and makes sure that

      * nothing ever becomes NaN or infinite,
      * the output stays inside the valid range,
      * the engine does not keep generating a tone of its own once the input
        stops — that is exactly what a "random harsh beep" would be.
*/
bool runStabilityChecks()
{
    struct Case
    {
        const char* name;
        Params params;
        bool checkTail;
    };

    std::vector<Case> cases;
    {
        Params p;
        p.bitDepth = 4.0f; p.sampleRate = 4000.0f; p.tapeLowpass = 1500.0f;
        p.wow = 1.0f; p.drive = 1.0f; p.hissDb = -90.0f; p.humDb = -90.0f;
        p.codecKbps = 24; p.speakerHighpass = 700.0f; p.speakerLowpass = 1500.0f;
        p.resonance = 1.0f; p.reverb = 1.0f; p.width = 0.0f; p.outputDb = 6.0f;
        cases.push_back ({ "极端低端", p, true });
    }
    {
        Params p;
        p.bitDepth = 16.0f; p.sampleRate = 48000.0f; p.tapeLowpass = 12000.0f;
        p.wow = 0.0f; p.drive = 0.0f; p.hissDb = -90.0f; p.humDb = -90.0f;
        p.codecKbps = 320; p.speakerHighpass = 20.0f; p.speakerLowpass = 12000.0f;
        p.resonance = 1.0f; p.reverb = 1.0f; p.width = 1.0f; p.outputDb = 6.0f;
        cases.push_back ({ "极端高端", p, true });
    }
    {
        Params p;   /* default preset */
        cases.push_back ({ "默认设置", p, true });
    }
    {
        Params p;   /* loud hiss has to stay finite even if it never goes quiet */
        p.hissDb = -20.0f; p.humDb = -30.0f; p.reverb = 1.0f; p.drive = 1.0f;
        cases.push_back ({ "最大底噪", p, false });
    }

    const double sr = 48000.0;
    const int blockSize = 512;
    bool allPassed = true;

    for (const auto& testCase : cases)
    {
        LoFiEngine engine;
        engine.prepare (sr, blockSize, 2);

        std::vector<float> left (blockSize), right (blockSize);
        Rng rng (7u);
        float peak = 0.0f;
        bool finite = true;

        const int loudBlocks = static_cast<int> (2.0 * sr / blockSize);
        for (int block = 0; block < loudBlocks; ++block)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                const float transient = rng.nextUnipolar() < 0.02f ? 3.0f : 0.0f;
                left[static_cast<std::size_t> (i)] = transient + 0.9f * std::sin (0.31f * i);
                right[static_cast<std::size_t> (i)] = -transient + 0.9f * std::sin (0.29f * i);
            }
            float* pointers[2] = { left.data(), right.data() };
            engine.process (pointers, 2, blockSize, testCase.params);
            for (int i = 0; i < blockSize; ++i)
            {
                if (! std::isfinite (left[static_cast<std::size_t> (i)])
                    || ! std::isfinite (right[static_cast<std::size_t> (i)]))
                    finite = false;
                peak = std::max (peak, std::abs (left[static_cast<std::size_t> (i)]));
                peak = std::max (peak, std::abs (right[static_cast<std::size_t> (i)]));
            }
        }

        float tail = 0.0f;
        const int silentBlocks = static_cast<int> (3.0 * sr / blockSize);
        for (int block = 0; block < silentBlocks; ++block)
        {
            std::fill (left.begin(), left.end(), 0.0f);
            std::fill (right.begin(), right.end(), 0.0f);
            float* pointers[2] = { left.data(), right.data() };
            engine.process (pointers, 2, blockSize, testCase.params);
            if (block > static_cast<int> (2.5 * sr / blockSize))
                for (int i = 0; i < blockSize; ++i)
                    tail = std::max (tail, std::abs (left[static_cast<std::size_t> (i)]));
        }

        const bool passed = finite && peak <= 1.001f && (! testCase.checkTail || tail < 0.002f);
        allPassed = allPassed && passed;
        std::printf ("  [%s] %-10s 峰值 %.4f  静音后残留 %.6f\n",
                     passed ? "PASS" : "FAIL", testCase.name, peak, tail);
    }

    return allPassed;
}

} // namespace

int main (int argc, char** argv)
{
    std::string inputPath, outputPath;
    Params params;

    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        auto value = [&]() -> double
        {
            return (i + 1 < argc) ? std::atof (argv[++i]) : 0.0;
        };
        if (arg == "--bit")       params.bitDepth = static_cast<float> (value());
        else if (arg == "--sr")   params.sampleRate = static_cast<float> (value());
        else if (arg == "--tape") params.tapeLowpass = static_cast<float> (value());
        else if (arg == "--wow")  params.wow = static_cast<float> (value());
        else if (arg == "--drive") params.drive = static_cast<float> (value());
        else if (arg == "--hiss") params.hissDb = static_cast<float> (value());
        else if (arg == "--hum")  params.humDb = static_cast<float> (value());
        else if (arg == "--kbps") params.codecKbps = static_cast<int> (value());
        else if (arg == "--spkhi") params.speakerHighpass = static_cast<float> (value());
        else if (arg == "--spklo") params.speakerLowpass = static_cast<float> (value());
        else if (arg == "--res")  params.resonance = static_cast<float> (value());
        else if (arg == "--rev")  params.reverb = static_cast<float> (value());
        else if (arg == "--width") params.width = static_cast<float> (value());
        else if (arg == "--outdb") params.outputDb = static_cast<float> (value());
        else if (arg == "--in")
        {
            if (i + 1 < argc) inputPath = argv[++i];
        }
        else if (arg == "--write")
        {
            if (i + 1 < argc) outputPath = argv[++i];
        }
        else if (inputPath.empty()) inputPath = arg;
        else if (outputPath.empty()) outputPath = arg;
    }

    AudioFile file;
    if (! inputPath.empty())
    {
        if (! loadWav (inputPath, file))
        {
            std::printf ("could not read %s\n", inputPath.c_str());
            return 2;
        }
        std::printf ("input   : %s (%d frames, %d ch, %d Hz)\n",
                     inputPath.c_str(), file.numFrames(), file.numChannels(), file.sampleRate);
    }
    else
    {
        synthesizeTestSignal (file, 6.0, 44100);
        std::printf ("input   : built in test signal (%d frames, %d ch, %d Hz)\n",
                     file.numFrames(), file.numChannels(), file.sampleRate);
    }

    const Metrics before = measure (file);
    printMetrics ("dry", before);

    LoFiEngine engine;
    engine.prepare (file.sampleRate, 512, file.numChannels());

    std::vector<float*> pointers;
    for (auto& ch : file.channels)
        pointers.push_back (ch.data());

    /* render in small blocks so this exercises the same code path as a host */
    const int blockSize = 256;
    for (int start = 0; start < file.numFrames(); start += blockSize)
    {
        const int count = std::min (blockSize, file.numFrames() - start);
        std::vector<float*> block (pointers.size());
        for (std::size_t ch = 0; ch < pointers.size(); ++ch)
            block[ch] = pointers[ch] + start;
        engine.process (block.data(), static_cast<int> (block.size()), count, params);
    }

    const Metrics after = measure (file);
    printMetrics ("processed", after);

    if (! outputPath.empty())
    {
        if (saveWav16 (outputPath, file))
            std::printf ("written : %s\n", outputPath.c_str());
        else
        {
            std::printf ("could not write %s\n", outputPath.c_str());
            return 3;
        }
    }

    const bool ok = after.finite && after.peakDb <= 0.05 && after.rmsDb > -80.0;
    std::printf ("self test: %s\n", ok ? "PASS" : "FAIL");

    std::printf ("\nstability checks:\n");
    const bool stable = runStabilityChecks();
    std::printf ("stability: %s\n", stable ? "PASS" : "FAIL");

    return (ok && stable) ? 0 : 1;
}
