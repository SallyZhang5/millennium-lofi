/*
    The little oscilloscope in the left column.

    Clicking it cycles through three modes: triggered waveform, rolling peak
    envelope and a stereo vectorscope.
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <functional>
#include <vector>

#include "ScopeData.h"

namespace mlofi
{

class ScopeDisplay : public juce::Component,
                     public juce::SettableTooltipClient
{
public:
    enum class Mode
    {
        triggered = 0,
        rolling,
        vectorscope,
        numModes
    };

    explicit ScopeDisplay (ScopeData& sharedData);

    void setMode (Mode newMode);
    Mode getMode() const noexcept { return mode; }

    /*  Called on the message thread whenever the user clicks the display. */
    std::function<void (Mode)> onModeChanged;

    void paint (juce::Graphics&) override;
    void mouseDown (const juce::MouseEvent&) override;

private:
    void drawTriggered (juce::Graphics&, juce::Rectangle<float> screen);
    void drawRolling (juce::Graphics&, juce::Rectangle<float> screen);
    void drawVectorscope (juce::Graphics&, juce::Rectangle<float> screen);
    juce::String modeName() const;
    void drawFrame (juce::Graphics&, juce::Rectangle<float> screen, juce::Colour background);

    ScopeData& data;
    Mode mode = Mode::triggered;
    std::vector<float> left, right;
    std::vector<ScopeData::Envelope> envelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopeDisplay)
};

} // namespace mlofi
