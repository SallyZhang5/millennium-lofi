/*  Minimal WAV reader/writer used by the offline test renderer. */

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace mlofi
{

struct AudioFile
{
    std::vector<std::vector<float>> channels;
    int sampleRate = 44100;

    int numFrames() const
    {
        return channels.empty() ? 0 : static_cast<int> (channels[0].size());
    }

    int numChannels() const { return static_cast<int> (channels.size()); }
};

inline std::uint32_t readU32 (const unsigned char* p)
{
    return static_cast<std::uint32_t> (p[0]) | (static_cast<std::uint32_t> (p[1]) << 8)
         | (static_cast<std::uint32_t> (p[2]) << 16) | (static_cast<std::uint32_t> (p[3]) << 24);
}

inline std::uint16_t readU16 (const unsigned char* p)
{
    return static_cast<std::uint16_t> (p[0] | (p[1] << 8));
}

inline bool loadWav (const std::string& path, AudioFile& out)
{
    std::ifstream in (path, std::ios::binary);
    if (! in)
        return false;

    std::vector<unsigned char> data ((std::istreambuf_iterator<char> (in)),
                                     std::istreambuf_iterator<char>());
    if (data.size() < 44 || std::memcmp (data.data(), "RIFF", 4) != 0
        || std::memcmp (data.data() + 8, "WAVE", 4) != 0)
        return false;

    int audioFormat = 1, channels = 0, bits = 0, sampleRate = 0;
    std::size_t pos = 12;
    const unsigned char* sampleData = nullptr;
    std::size_t sampleBytes = 0;

    while (pos + 8 <= data.size())
    {
        const char* id = reinterpret_cast<const char*> (data.data() + pos);
        const std::uint32_t size = readU32 (data.data() + pos + 4);
        const std::size_t body = pos + 8;

        if (std::memcmp (id, "fmt ", 4) == 0 && body + 16 <= data.size())
        {
            audioFormat = readU16 (data.data() + body);
            channels = readU16 (data.data() + body + 2);
            sampleRate = static_cast<int> (readU32 (data.data() + body + 4));
            bits = readU16 (data.data() + body + 14);
            if (audioFormat == 0xFFFE && body + 26 <= data.size())
                audioFormat = readU16 (data.data() + body + 24);
        }
        else if (std::memcmp (id, "data", 4) == 0)
        {
            sampleData = data.data() + body;
            sampleBytes = std::min<std::size_t> (size, data.size() - body);
        }

        pos = body + size + (size & 1u);
    }

    if (sampleData == nullptr || channels <= 0 || sampleRate <= 0)
        return false;

    const int bytesPerSample = bits / 8;
    if (bytesPerSample <= 0)
        return false;

    const int frames = static_cast<int> (sampleBytes / (static_cast<std::size_t> (bytesPerSample)
                                                        * static_cast<std::size_t> (channels)));
    out.channels.assign (static_cast<std::size_t> (channels), std::vector<float> (static_cast<std::size_t> (frames)));
    out.sampleRate = sampleRate;

    for (int ch = 0; ch < channels; ++ch)
        for (int i = 0; i < frames; ++i)
        {
            const unsigned char* p = sampleData
                                   + (static_cast<std::size_t> (i) * static_cast<std::size_t> (channels)
                                      + static_cast<std::size_t> (ch)) * static_cast<std::size_t> (bytesPerSample);
            float value = 0.0f;
            if (audioFormat == 3 && bits == 32)
            {
                std::uint32_t raw = readU32 (p);
                float f;
                std::memcpy (&f, &raw, 4);
                value = f;
            }
            else if (bits == 16)
                value = static_cast<float> (static_cast<short> (readU16 (p))) / 32768.0f;
            else if (bits == 24)
            {
                std::int32_t v = (static_cast<std::int32_t> (p[2]) << 16)
                               | (static_cast<std::int32_t> (p[1]) << 8)
                               | static_cast<std::int32_t> (p[0]);
                if (v & 0x800000)
                    v |= ~0xFFFFFF;
                value = static_cast<float> (v) / 8388608.0f;
            }
            else if (bits == 32)
                value = static_cast<float> (static_cast<std::int32_t> (readU32 (p))) / 2147483648.0f;

            out.channels[static_cast<std::size_t> (ch)][static_cast<std::size_t> (i)] = value;
        }

    return true;
}

inline void writeU16 (std::ofstream& out, std::uint16_t v)
{
    const unsigned char b[2] = { static_cast<unsigned char> (v & 0xFF),
                                 static_cast<unsigned char> ((v >> 8) & 0xFF) };
    out.write (reinterpret_cast<const char*> (b), 2);
}

inline void writeU32 (std::ofstream& out, std::uint32_t v)
{
    const unsigned char b[4] = { static_cast<unsigned char> (v & 0xFF),
                                 static_cast<unsigned char> ((v >> 8) & 0xFF),
                                 static_cast<unsigned char> ((v >> 16) & 0xFF),
                                 static_cast<unsigned char> ((v >> 24) & 0xFF) };
    out.write (reinterpret_cast<const char*> (b), 4);
}

inline bool saveWav16 (const std::string& path, const AudioFile& file)
{
    const int channels = file.numChannels();
    const int frames = file.numFrames();
    if (channels <= 0)
        return false;

    std::ofstream out (path, std::ios::binary);
    if (! out)
        return false;

    const std::uint32_t dataBytes = static_cast<std::uint32_t> (frames) * channels * 2u;
    out.write ("RIFF", 4);
    writeU32 (out, 36u + dataBytes);
    out.write ("WAVE", 4);
    out.write ("fmt ", 4);
    writeU32 (out, 16);
    writeU16 (out, 1);
    writeU16 (out, static_cast<std::uint16_t> (channels));
    writeU32 (out, static_cast<std::uint32_t> (file.sampleRate));
    writeU32 (out, static_cast<std::uint32_t> (file.sampleRate * channels * 2));
    writeU16 (out, static_cast<std::uint16_t> (channels * 2));
    writeU16 (out, 16);
    out.write ("data", 4);
    writeU32 (out, dataBytes);

    for (int i = 0; i < frames; ++i)
        for (int ch = 0; ch < channels; ++ch)
        {
            const float v = file.channels[static_cast<std::size_t> (ch)][static_cast<std::size_t> (i)];
            const int clamped = static_cast<int> (std::lround (std::max (-1.0f, std::min (1.0f, v)) * 32767.0f));
            writeU16 (out, static_cast<std::uint16_t> (static_cast<short> (clamped)));
        }

    return static_cast<bool> (out);
}

} // namespace mlofi
