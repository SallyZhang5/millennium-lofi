#include "PluginProcessor.h"

#include "PluginEditor.h"

namespace mlofi
{

MillenniumLoFiProcessor::MillenniumLoFiProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createLayout())
{
}

bool MillenniumLoFiProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;
    const auto& in = layouts.getMainInputChannelSet();
    return in == out || in.isDisabled();
}

void MillenniumLoFiProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    dryBuffer.setSize (std::max (1, getTotalNumOutputChannels()), std::max (1, samplesPerBlock), false, true, true);
    scopeData.prepare (sampleRate);
    setLatencySamples (engine.latencySamples());
}

void MillenniumLoFiProcessor::pushScope (const juce::AudioBuffer<float>& buffer) noexcept
{
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    if (numSamples <= 0 || numChannels <= 0)
        return;

    const float* left = buffer.getReadPointer (0);
    const float* right = numChannels > 1 ? buffer.getReadPointer (1) : left;
    for (int i = 0; i < numSamples; ++i)
        scopeData.push (left[i], right[i]);
}

Params MillenniumLoFiProcessor::readParams() const
{
    Params p;
    auto value = [this] (const char* id) { return apvts.getRawParameterValue (id)->load(); };

    p.bitDepth = value (ids::bitDepth);
    p.sampleRate = value (ids::sampleRate);
    p.tapeLowpass = value (ids::tapeLowpass);
    p.wow = value (ids::wow);
    p.drive = value (ids::drive);
    p.hissDb = value (ids::hissDb);
    p.humDb = value (ids::humDb);

    const auto kbpsValues = codecValues();
    const int index = juce::jlimit (0, static_cast<int> (kbpsValues.size()) - 1,
                                    static_cast<int> (std::lround (value (ids::codecKbps))));
    p.codecKbps = kbpsValues[static_cast<std::size_t> (index)];

    p.speakerHighpass = value (ids::speakerHighpass);
    p.speakerLowpass = value (ids::speakerLowpass);
    p.resonance = value (ids::resonance);
    p.reverb = value (ids::reverb);
    p.width = value (ids::width);
    p.outputDb = value (ids::outputDb);
    return p;
}

void MillenniumLoFiProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    for (int ch = getTotalNumInputChannels(); ch < numChannels; ++ch)
        buffer.clear (ch, 0, numSamples);

    if (numSamples <= 0)
        return;

    if (apvts.getRawParameterValue (ids::bypass)->load() > 0.5f)
    {
        pushScope (buffer);   // bypassed: the scope shows the untouched input
        return;
    }

    const float mix = apvts.getRawParameterValue (ids::mix)->load();
    const bool needsDry = mix < 0.999f && dryBuffer.getNumSamples() >= numSamples;
    if (needsDry)
        for (int ch = 0; ch < std::min (numChannels, dryBuffer.getNumChannels()); ++ch)
            dryBuffer.copyFrom (ch, 0, buffer, ch, 0, numSamples);

    engine.process (buffer.getArrayOfWritePointers(), numChannels, numSamples, readParams());

    if (needsDry)
    {
        buffer.applyGain (mix);
        for (int ch = 0; ch < std::min (numChannels, dryBuffer.getNumChannels()); ++ch)
            buffer.addFrom (ch, 0, dryBuffer, ch, 0, numSamples, 1.0f - mix);
    }

    pushScope (buffer);
}

juce::AudioProcessorEditor* MillenniumLoFiProcessor::createEditor()
{
    return new MillenniumLoFiEditor (*this);
}

void MillenniumLoFiProcessor::getStateInformation (juce::MemoryBlock& destinationData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destinationData);
}

void MillenniumLoFiProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

} // namespace mlofi

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new mlofi::MillenniumLoFiProcessor();
}
