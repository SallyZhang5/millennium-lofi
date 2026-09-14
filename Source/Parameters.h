/*  Parameter definitions and factory presets, shared by processor and editor. */

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <array>
#include <vector>

namespace mlofi
{

/*  JUCE's String (const char*) constructor only accepts ASCII: any byte above
    127 is treated as a separate character. Every literal that contains
    non-ASCII text therefore has to be converted from UTF-8 explicitly. */
inline juce::String u8 (const char* utf8Text) { return juce::String::fromUTF8 (utf8Text); }

namespace ids
{
    static constexpr const char* bitDepth = "bitDepth";
    static constexpr const char* sampleRate = "sampleRate";
    static constexpr const char* tapeLowpass = "tapeLowpass";
    static constexpr const char* wow = "wow";
    static constexpr const char* drive = "drive";
    static constexpr const char* hissDb = "hissDb";
    static constexpr const char* humDb = "humDb";
    static constexpr const char* codecKbps = "codecKbps";
    static constexpr const char* speakerHighpass = "speakerHighpass";
    static constexpr const char* speakerLowpass = "speakerLowpass";
    static constexpr const char* resonance = "resonance";
    static constexpr const char* reverb = "reverb";
    static constexpr const char* width = "width";
    static constexpr const char* outputDb = "outputDb";
    static constexpr const char* mix = "mix";
    static constexpr const char* bypass = "bypass";
}

inline juce::StringArray codecChoices()
{
    juce::StringArray choices;
    choices.add (u8("关闭"));
    choices.add (u8("24 kbps · 电话音质"));
    choices.add (u8("32 kbps · 早期彩铃"));
    choices.add (u8("48 kbps · 廉价 MP3"));
    choices.add (u8("64 kbps · 千禧随身听"));
    choices.add (u8("96 kbps · 常见下载"));
    choices.add (u8("128 kbps · 当年标准"));
    choices.add (u8("192 kbps · 接近 CD"));
    choices.add (u8("320 kbps · 几乎无损"));
    return choices;
}

inline std::vector<int> codecValues()
{
    return { 0, 24, 32, 48, 64, 96, 128, 192, 320 };
}

inline juce::NormalisableRange<float> logRange (float min, float max)
{
    juce::NormalisableRange<float> range (min, max);
    range.setSkewForCentre (std::sqrt (min * max));
    return range;
}

inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::bitDepth, 1 }, u8("颗粒感（位深）"),
        NormalisableRange<float> (4.0f, 16.0f, 1.0f), 13.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::sampleRate, 1 }, u8("数码采样率"),
        logRange (4000.0f, 48000.0f), 22050.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::tapeLowpass, 1 }, u8("磁带高频衰减"),
        logRange (1500.0f, 12000.0f), 11000.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::wow, 1 }, u8("磁带抖晃"),
        NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.16f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::drive, 1 }, u8("过载失真"),
        NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.18f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::hissDb, 1 }, u8("底噪嘶声"),
        NormalisableRange<float> (-90.0f, -20.0f, 0.1f), -74.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::humDb, 1 }, u8("电流嗡声"),
        NormalisableRange<float> (-90.0f, -30.0f, 0.1f), -90.0f));

    layout.add (std::make_unique<AudioParameterChoice> (
        ParameterID { ids::codecKbps, 1 }, u8("MP3 码率"), codecChoices(), 4));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::speakerHighpass, 1 }, u8("低音削减"),
        logRange (20.0f, 700.0f), 90.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::speakerLowpass, 1 }, u8("小喇叭高频"),
        logRange (1500.0f, 12000.0f), 8200.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::resonance, 1 }, u8("喇叭共鸣"),
        NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.18f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::reverb, 1 }, u8("空间混响"),
        NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.10f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::width, 1 }, u8("立体声宽度"),
        NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.66f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::outputDb, 1 }, u8("输出音量"),
        NormalisableRange<float> (-12.0f, 6.0f, 0.1f), 0.0f));

    layout.add (std::make_unique<AudioParameterFloat> (
        ParameterID { ids::mix, 1 }, u8("干湿比"),
        NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f));

    layout.add (std::make_unique<AudioParameterBool> (
        ParameterID { ids::bypass, 1 }, u8("旁通"), false));

    return layout;
}

struct PresetValue
{
    const char* id;
    float value;
};

struct FactoryPreset
{
    juce::String name;
    juce::String description;
    std::vector<PresetValue> values;
};

inline const std::vector<FactoryPreset>& factoryPresets()
{
    static const std::vector<FactoryPreset> presets
    {
        { u8("千禧 MP3 随身听"), u8("2000 年前后最典型的声音：64 kbps 下载曲、塑料耳塞"),
          { { ids::bitDepth, 13 }, { ids::sampleRate, 22050 }, { ids::tapeLowpass, 11000 },
            { ids::wow, 0.16f }, { ids::drive, 0.18f }, { ids::hissDb, -74 },
            { ids::humDb, -90 }, { ids::codecKbps, 4 }, { ids::speakerHighpass, 90 },
            { ids::speakerLowpass, 8200 }, { ids::resonance, 0.18f }, { ids::reverb, 0.10f },
            { ids::width, 0.66f }, { ids::outputDb, 0.0f } } },

        { u8("磁带随身听"), u8("磁带复制品的味道：走带飘、高频软、有一层嘶声"),
          { { ids::bitDepth, 14 }, { ids::sampleRate, 32000 }, { ids::tapeLowpass, 6500 },
            { ids::wow, 0.62f }, { ids::drive, 0.34f }, { ids::hissDb, -56 },
            { ids::humDb, -84 }, { ids::codecKbps, 0 }, { ids::speakerHighpass, 150 },
            { ids::speakerLowpass, 6200 }, { ids::resonance, 0.26f }, { ids::reverb, 0.14f },
            { ids::width, 0.58f }, { ids::outputDb, 0.0f } } },

        { u8("老式收音机"), u8("窄频带加单声道，像隔着塑料机壳传来的广播"),
          { { ids::bitDepth, 12 }, { ids::sampleRate, 11025 }, { ids::tapeLowpass, 8000 },
            { ids::wow, 0.22f }, { ids::drive, 0.30f }, { ids::hissDb, -54 },
            { ids::humDb, -50 }, { ids::codecKbps, 3 }, { ids::speakerHighpass, 380 },
            { ids::speakerLowpass, 3200 }, { ids::resonance, 0.46f }, { ids::reverb, 0.12f },
            { ids::width, 0.10f }, { ids::outputDb, 0.5f } } },

        { u8("早期手机彩铃"), u8("8 kHz 单声道、8 位颗粒，就是当年那支彩铃"),
          { { ids::bitDepth, 8 }, { ids::sampleRate, 8000 }, { ids::tapeLowpass, 6000 },
            { ids::wow, 0.10f }, { ids::drive, 0.22f }, { ids::hissDb, -68 },
            { ids::humDb, -90 }, { ids::codecKbps, 2 }, { ids::speakerHighpass, 320 },
            { ids::speakerLowpass, 3400 }, { ids::resonance, 0.34f }, { ids::reverb, 0.06f },
            { ids::width, 0.0f }, { ids::outputDb, 1.0f } } },

        { u8("8-bit 游戏机"), u8("极低位深与低采样率，颗粒硬、棱角分明"),
          { { ids::bitDepth, 6 }, { ids::sampleRate, 11025 }, { ids::tapeLowpass, 9000 },
            { ids::wow, 0.04f }, { ids::drive, 0.12f }, { ids::hissDb, -80 },
            { ids::humDb, -90 }, { ids::codecKbps, 0 }, { ids::speakerHighpass, 120 },
            { ids::speakerLowpass, 6800 }, { ids::resonance, 0.20f }, { ids::reverb, 0.05f },
            { ids::width, 0.14f }, { ids::outputDb, 0.0f } } },

        { u8("电脑塑料小音箱"), u8("宿舍桌面音箱：中频突出、低频没有、稍微发糊"),
          { { ids::bitDepth, 14 }, { ids::sampleRate, 22050 }, { ids::tapeLowpass, 10000 },
            { ids::wow, 0.12f }, { ids::drive, 0.24f }, { ids::hissDb, -70 },
            { ids::humDb, -78 }, { ids::codecKbps, 5 }, { ids::speakerHighpass, 210 },
            { ids::speakerLowpass, 6000 }, { ids::resonance, 0.52f }, { ids::reverb, 0.16f },
            { ids::width, 0.48f }, { ids::outputDb, 0.0f } } },

        { u8("轻处理（保留细节）"), u8("只想加一点年代感、不想大改的时候用"),
          { { ids::bitDepth, 15 }, { ids::sampleRate, 32000 }, { ids::tapeLowpass, 12000 },
            { ids::wow, 0.10f }, { ids::drive, 0.12f }, { ids::hissDb, -78 },
            { ids::humDb, -90 }, { ids::codecKbps, 6 }, { ids::speakerHighpass, 60 },
            { ids::speakerLowpass, 10000 }, { ids::resonance, 0.10f }, { ids::reverb, 0.08f },
            { ids::width, 0.85f }, { ids::outputDb, 0.0f } } },
    };
    return presets;
}

} // namespace mlofi
