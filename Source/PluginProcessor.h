#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <atomic>

#include "Parameters.h"
#include "ScopeData.h"
#include "dsp/LoFiEngine.h"

namespace mlofi
{

class MillenniumLoFiProcessor : public juce::AudioProcessor
{
public:
    MillenniumLoFiProcessor();
    ~MillenniumLoFiProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Millennium LoFi"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.6; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destinationData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    Params readParams() const;

    juce::AudioProcessorValueTreeState apvts;
    LoFiEngine engine;

    /*  Latest audio for the editor's oscilloscope, and the mode it shows
        (kept here so the choice survives closing and reopening the window). */
    ScopeData scopeData;
    std::atomic<int> scopeMode { 0 };

private:
    juce::AudioBuffer<float> dryBuffer;

    void pushScope (const juce::AudioBuffer<float>&) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MillenniumLoFiProcessor)
};

} // namespace mlofi
