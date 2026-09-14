/*
    Shared scope data.

    The audio thread only writes here, the editor only reads. No locks and no
    allocation are used on the audio thread; a read may catch one frame that is
    half updated, which is invisible on a scope display.
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

namespace mlofi
{

class ScopeData
{
public:
    static constexpr int rawSize = 8192;      // power of two, newest frames
    static constexpr int numBuckets = 256;    // peak envelope history

    struct Envelope
    {
        float minL = 0.0f, maxL = 0.0f, minR = 0.0f, maxR = 0.0f;
    };

    void prepare (double sampleRate) noexcept
    {
        /*  the envelope window covers roughly 1.5 seconds regardless of rate */
        bucketSamples = std::max (64, static_cast<int> (sampleRate * 1.5 / numBuckets));
        reset();
    }

    void reset() noexcept
    {
        for (auto& frame : raw)
            frame = Frame {};
        for (auto& bucket : buckets)
            bucket = Envelope {};
        rawWrite = 0;
        bucketWrite = 0;
        bucketFill = 0;
    }

    /*  Called from the audio thread, once per sample. */
    inline void push (float left, float right) noexcept
    {
        if (! std::isfinite (left))  left = 0.0f;
        if (! std::isfinite (right)) right = 0.0f;

        raw[rawWrite].l = left;
        raw[rawWrite].r = right;
        rawWrite = (rawWrite + 1) & (rawSize - 1);

        Envelope& bucket = buckets[bucketWrite];
        bucket.minL = std::min (bucket.minL, left);
        bucket.maxL = std::max (bucket.maxL, left);
        bucket.minR = std::min (bucket.minR, right);
        bucket.maxR = std::max (bucket.maxR, right);

        if (++bucketFill >= bucketSamples)
        {
            bucketFill = 0;
            bucketWrite = (bucketWrite + 1) % numBuckets;
            buckets[bucketWrite] = Envelope {};
        }
    }

    /*  Copies the newest `count` frames, oldest first. */
    void copyRaw (int count, std::vector<float>& left, std::vector<float>& right) const
    {
        count = std::max (0, std::min (count, rawSize));
        left.resize (static_cast<std::size_t> (count));
        right.resize (static_cast<std::size_t> (count));
        int index = (rawWrite - count + rawSize * 2) & (rawSize - 1);
        for (int i = 0; i < count; ++i)
        {
            left[static_cast<std::size_t> (i)] = raw[index].l;
            right[static_cast<std::size_t> (i)] = raw[index].r;
            index = (index + 1) & (rawSize - 1);
        }
    }

    /*  Copies the envelope history, oldest first. */
    void copyEnvelope (std::vector<Envelope>& destination) const
    {
        destination.resize (numBuckets);
        int index = (bucketWrite + 1) % numBuckets;
        for (int i = 0; i < numBuckets; ++i)
        {
            destination[static_cast<std::size_t> (i)] = buckets[index];
            index = (index + 1) % numBuckets;
        }
    }

private:
    struct Frame
    {
        float l = 0.0f, r = 0.0f;
    };

    Frame raw[rawSize];
    Envelope buckets[numBuckets];
    int rawWrite = 0;
    int bucketWrite = 0;
    int bucketFill = 0;
    int bucketSamples = 256;
};

} // namespace mlofi
