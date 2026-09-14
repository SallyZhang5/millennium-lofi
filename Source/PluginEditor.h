#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "XpLookAndFeel.h"

#include <memory>
#include <vector>

namespace mlofi
{

class MillenniumLoFiEditor : public juce::AudioProcessorEditor
{
public:
    explicit MillenniumLoFiEditor (MillenniumLoFiProcessor&);
    ~MillenniumLoFiEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    struct SliderRow
    {
        juce::Label label;
        juce::Slider slider;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    };

    void addSliderRow (juce::Component& parent, std::vector<SliderRow*>& rows,
                       const juce::String& parameterId, const juce::String& label,
                       const juce::String& suffix, int decimals);
    void layoutRows (const juce::Component& group, const std::vector<SliderRow*>& rows,
                     int firstRowY, int rowHeight);

    void refreshPresetList();
    void applySelectedPreset();
    void saveCurrentAsPreset();
    void deleteSelectedPreset();
    void setStatus (const juce::String& text, bool isError = false);
    bool selectionIsUserPreset() const;

    MillenniumLoFiProcessor& processor;
    XpLookAndFeel lookAndFeel;

    juce::GroupComponent presetGroup, infoGroup, tapeGroup, digitalGroup, speakerGroup;

    juce::ComboBox presetBox;
    juce::TextEditor nameEditor;
    juce::TextButton saveButton { "保存为新预设" };
    juce::TextButton deleteButton { "删除预设" };
    juce::TextButton revealButton { "打开预设文件夹" };
    juce::Label statusLabel;
    juce::Label descriptionLabel;

    juce::Label codecLabel;
    juce::ComboBox codecBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> codecAttachment;

    std::vector<std::unique_ptr<SliderRow>> sliderStorage;
    std::vector<SliderRow*> tapeRows, digitalRows, speakerRows;

    juce::ToggleButton bypassButton { "旁通" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    juce::Label mixLabel, outputLabel;
    juce::Slider mixSlider, outputSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment, outputAttachment;

    juce::StringArray presetNames;
    bool updatingPresetBox = false;

    static constexpr int titleHeight = 26;
    static constexpr int rowHeight = 24;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MillenniumLoFiEditor)
};

} // namespace mlofi
