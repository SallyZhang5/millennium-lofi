#include "PluginEditor.h"

#include "PresetManager.h"

namespace mlofi
{

MillenniumLoFiEditor::MillenniumLoFiEditor (MillenniumLoFiProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel (&lookAndFeel);
    setSize (760, 500);
    setResizable (false, false);

    auto& state = processor.apvts;

    /* ---- groups ---- */
    for (auto* group : { &presetGroup, &infoGroup, &tapeGroup, &digitalGroup, &speakerGroup })
        addAndMakeVisible (*group);
    presetGroup.setText ("预设");
    infoGroup.setText ("说明");
    tapeGroup.setText ("磁带");
    digitalGroup.setText ("数码");
    speakerGroup.setText ("小喇叭与空间");

    /* ---- preset panel ---- */
    addAndMakeVisible (presetBox);
    presetBox.onChange = [this] { applySelectedPreset(); };
    refreshPresetList();

    addAndMakeVisible (nameEditor);
    nameEditor.setTextToShowWhenEmpty ("新预设的名字", juce::Colours::grey);
    nameEditor.setSelectAllWhenFocused (true);

    for (auto* button : { &saveButton, &deleteButton, &revealButton })
        addAndMakeVisible (*button);
    saveButton.onClick = [this] { saveCurrentAsPreset(); };
    deleteButton.onClick = [this] { deleteSelectedPreset(); };
    revealButton.onClick = [this]
    {
        PresetManager::userPresetDirectory().revealToUser();
        setStatus ("预设保存在：" + PresetManager::userPresetDirectory().getFullPathName());
    };

    addAndMakeVisible (statusLabel);
    statusLabel.setColour (juce::Label::textColourId, juce::Colour (0xff4a4a42));
    statusLabel.setFont (xp::uiFont (12.0f));
    statusLabel.setJustificationType (juce::Justification::topLeft);

    addAndMakeVisible (descriptionLabel);
    descriptionLabel.setFont (xp::uiFont (13.0f));
    descriptionLabel.setColour (juce::Label::textColourId, juce::Colour (0xff2b2b25));
    descriptionLabel.setJustificationType (juce::Justification::topLeft);
    descriptionLabel.setText (factoryPresets().front().description, juce::dontSendNotification);

    /* ---- sliders ---- */
    addSliderRow (tapeGroup, tapeRows, ids::wow, "磁带抖晃", "", 2);
    addSliderRow (tapeGroup, tapeRows, ids::drive, "过载失真", "", 2);
    addSliderRow (tapeGroup, tapeRows, ids::hissDb, "底噪嘶声", " dB", 0);
    addSliderRow (tapeGroup, tapeRows, ids::humDb, "电流嗡声", " dB", 0);
    addSliderRow (tapeGroup, tapeRows, ids::tapeLowpass, "磁带高频衰减", " Hz", 0);

    addSliderRow (digitalGroup, digitalRows, ids::bitDepth, "颗粒感（位深）", " bit", 0);
    addSliderRow (digitalGroup, digitalRows, ids::sampleRate, "数码采样率", " Hz", 0);

    addAndMakeVisible (codecLabel);
    codecLabel.setText ("MP3 码率", juce::dontSendNotification);
    addAndMakeVisible (codecBox);
    codecBox.addItemList (codecChoices(), 1);
    codecAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        state, ids::codecKbps, codecBox);

    addSliderRow (speakerGroup, speakerRows, ids::speakerHighpass, "低音削减", " Hz", 0);
    addSliderRow (speakerGroup, speakerRows, ids::speakerLowpass, "小喇叭高频", " Hz", 0);
    addSliderRow (speakerGroup, speakerRows, ids::resonance, "喇叭共鸣", "", 2);
    addSliderRow (speakerGroup, speakerRows, ids::reverb, "空间混响", "", 2);
    addSliderRow (speakerGroup, speakerRows, ids::width, "立体声宽度", "", 2);

    /* ---- bottom row ---- */
    addAndMakeVisible (bypassButton);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        state, ids::bypass, bypassButton);

    for (auto* label : { &mixLabel, &outputLabel })
        addAndMakeVisible (*label);
    mixLabel.setText ("干湿比", juce::dontSendNotification);
    outputLabel.setText ("输出音量", juce::dontSendNotification);

    for (auto* slider : { &mixSlider, &outputSlider })
        addAndMakeVisible (*slider);
    mixSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    mixSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 58, 20);
    mixSlider.setNumDecimalPlacesToDisplay (2);
    outputSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    outputSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 58, 20);
    outputSlider.setNumDecimalPlacesToDisplay (1);
    outputSlider.setTextValueSuffix (" dB");
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        state, ids::mix, mixSlider);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        state, ids::outputDb, outputSlider);

    setStatus ("工厂预设可以直接用；调好后在左边起个名字保存成你自己的预设。");
}

MillenniumLoFiEditor::~MillenniumLoFiEditor()
{
    setLookAndFeel (nullptr);
}

/* ------------------------------------------------------------------ */

void MillenniumLoFiEditor::addSliderRow (juce::Component& parent, std::vector<SliderRow*>& rows,
                                         const juce::String& parameterId, const juce::String& name,
                                         const juce::String& suffix, int decimals)
{
    auto row = std::make_unique<SliderRow>();
    row->label.setText (name, juce::dontSendNotification);
    row->label.setFont (xp::uiFont (13.0f));
    row->label.setJustificationType (juce::Justification::centredLeft);

    row->slider.setSliderStyle (juce::Slider::LinearHorizontal);
    row->slider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 64, 20);
    row->slider.setNumDecimalPlacesToDisplay (decimals);
    if (suffix.isNotEmpty())
        row->slider.setTextValueSuffix (suffix);

    row->attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processor.apvts, parameterId, row->slider);

    parent.addAndMakeVisible (row->label);
    parent.addAndMakeVisible (row->slider);
    rows.push_back (row.get());
    sliderStorage.push_back (std::move (row));
}

void MillenniumLoFiEditor::layoutRows (juce::Component& parent, const std::vector<SliderRow*>& rows,
                                       int firstRowY, int height)
{
    const int rowTop = firstRowY;
    const int left = 12;
    const int totalWidth = parent.getWidth() - left * 2;
    for (std::size_t i = 0; i < rows.size(); ++i)
    {
        auto* row = rows[i];
        const int y = rowTop + static_cast<int> (i) * height;
        const int nameWidth = 104;
        row->label.setBounds (left, y, nameWidth, height);
        row->slider.setBounds (left + nameWidth + 4, y, totalWidth - nameWidth - 4, height);
    }
}

void MillenniumLoFiEditor::resized()
{
    auto area = getLocalBounds().reduced (7);
    area.removeFromTop (titleHeight - 4);

    auto left = area.removeFromLeft (226);
    left.removeFromRight (10);
    presetGroup.setBounds (left.removeFromTop (210));
    left.removeFromTop (8);
    infoGroup.setBounds (left);

    auto presetArea = presetGroup.getBounds().withTrimmedTop (18).reduced (10, 4);
    presetArea.removeFromBottom (4);
    presetBox.setBounds (presetArea.removeFromTop (22));
    presetArea.removeFromTop (5);
    nameEditor.setBounds (presetArea.removeFromTop (22));
    presetArea.removeFromTop (6);
    auto buttonRow = presetArea.removeFromTop (24);
    saveButton.setBounds (buttonRow.removeFromLeft (95));
    buttonRow.removeFromLeft (6);
    deleteButton.setBounds (buttonRow.removeFromLeft (95));
    presetArea.removeFromTop (6);
    revealButton.setBounds (presetArea.removeFromTop (24));
    presetArea.removeFromTop (6);
    statusLabel.setBounds (presetArea);

    auto infoArea = infoGroup.getBounds().withTrimmedTop (20).reduced (10, 4);
    descriptionLabel.setBounds (infoArea);

    auto right = area;
    tapeGroup.setBounds (right.removeFromTop (18 + 5 * rowHeight + 6));
    right.removeFromTop (8);
    digitalGroup.setBounds (right.removeFromTop (18 + 3 * rowHeight + 6));
    right.removeFromTop (8);
    speakerGroup.setBounds (right.removeFromTop (18 + 5 * rowHeight + 6));

    /*  row bounds are relative to their group component */
    layoutRows (tapeGroup, tapeRows, 20, rowHeight);
    layoutRows (digitalGroup, digitalRows, 20, rowHeight);
    layoutRows (speakerGroup, speakerRows, 20, rowHeight);

    /* the codec combo occupies the third row of the digital group */
    const int codecY = digitalGroup.getY() + 20 + 2 * rowHeight;
    codecLabel.setBounds (digitalGroup.getX() + 12, codecY, 104, rowHeight);
    codecBox.setBounds (digitalGroup.getX() + 12 + 108, codecY + 1,
                        digitalGroup.getWidth() - 24 - 108, rowHeight - 2);

    auto bottom = area.removeFromBottom (34);
    bottom.removeFromBottom (2);
    bypassButton.setBounds (bottom.removeFromLeft (80));
    bottom.removeFromLeft (10);
    mixLabel.setBounds (bottom.removeFromLeft (58));
    mixSlider.setBounds (bottom.removeFromLeft (juce::jmin (150, bottom.getWidth() / 3)));
    bottom.removeFromLeft (12);
    outputLabel.setBounds (bottom.removeFromLeft (62));
    outputSlider.setBounds (bottom.removeFromLeft (juce::jmin (170, bottom.getWidth())));
}

void MillenniumLoFiEditor::paint (juce::Graphics& g)
{
    g.fillAll (xp::face());

    /* XP window border */
    g.setColour (xp::windowBlue());
    g.drawRect (getLocalBounds(), 3);

    /* title strip */
    auto title = getLocalBounds().reduced (3).removeFromTop (titleHeight - 3);
    juce::ColourGradient gradient (xp::titleLight(), static_cast<float> (title.getX()),
                                   static_cast<float> (title.getY()),
                                   xp::titleDeep(), static_cast<float> (title.getX()),
                                   static_cast<float> (title.getBottom()), false);
    g.setGradientFill (gradient);
    g.fillRect (title);

    juce::ColourGradient gloss (juce::Colours::white.withAlpha (0.45f),
                                static_cast<float> (title.getX()), static_cast<float> (title.getY()),
                                juce::Colours::transparentWhite,
                                static_cast<float> (title.getX()),
                                static_cast<float> (title.getY() + title.getHeight() * 0.6f), false);
    g.setGradientFill (gloss);
    g.fillRect (title);

    g.setColour (juce::Colours::white);
    g.setFont (xp::uiFont (15.0f, true));
    g.drawText ("千禧年声音 · Millennium LoFi · 实时插件版",
                title.reduced (10, 0), juce::Justification::centredLeft, false);
}

/* ------------------------------------------------------------------ */

void MillenniumLoFiEditor::refreshPresetList()
{
    updatingPresetBox = true;
    presetBox.clear (juce::dontSendNotification);
    presetNames.clear();

    presetBox.addSectionHeading ("工厂预设");
    const auto& presets = factoryPresets();
    for (int i = 0; i < static_cast<int> (presets.size()); ++i)
        presetBox.addItem (presets[static_cast<std::size_t> (i)].name, i + 1);

    const auto userNames = PresetManager::userPresetNames();
    if (! userNames.isEmpty())
    {
        presetBox.addSeparator();
        presetBox.addSectionHeading ("我的预设");
        for (int i = 0; i < userNames.size(); ++i)
        {
            presetNames.add (userNames[i]);
            presetBox.addItem (userNames[i], 1000 + i);
        }
    }

    presetBox.setSelectedId (1, juce::dontSendNotification);
    updatingPresetBox = false;
}

bool MillenniumLoFiEditor::selectionIsUserPreset() const
{
    return presetBox.getSelectedId() >= 1000;
}

void MillenniumLoFiEditor::applySelectedPreset()
{
    if (updatingPresetBox)
        return;

    const int id = presetBox.getSelectedId();
    if (id <= 0)
        return;

    if (id >= 1000)
    {
        const int index = id - 1000;
        if (index < presetNames.size())
        {
            const auto name = presetNames[index];
            if (PresetManager::loadUserPreset (processor.apvts, name))
                setStatus ("已载入我的预设：" + name);
            else
                setStatus ("预设文件读不出来：" + name, true);
        }
        return;
    }

    const auto& presets = factoryPresets();
    const int index = id - 1;
    if (index >= 0 && index < static_cast<int> (presets.size()))
    {
        PresetManager::applyFactoryPreset (processor.apvts, index);
        descriptionLabel.setText (presets[static_cast<std::size_t> (index)].description,
                                  juce::dontSendNotification);
        setStatus (juce::String ("已载入工厂预设：") + presets[static_cast<std::size_t> (index)].name);
    }
}

void MillenniumLoFiEditor::saveCurrentAsPreset()
{
    const auto name = nameEditor.getText().trim();
    if (name.isEmpty())
    {
        setStatus ("请先在输入框里写一个预设名字。", true);
        nameEditor.grabKeyboardFocus();
        return;
    }

    if (PresetManager::saveUserPreset (processor.apvts, name))
    {
        refreshPresetList();
        const int index = presetNames.indexOf (name);
        if (index >= 0)
        {
            updatingPresetBox = true;
            presetBox.setSelectedId (1000 + index, juce::dontSendNotification);
            updatingPresetBox = false;
        }
        nameEditor.clear();
        setStatus ("已保存预设：" + name + "（保存在 " + PresetManager::userPresetDirectory().getFullPathName() + "）");
    }
    else
    {
        setStatus ("保存失败，请换一个名字再试。", true);
    }
}

void MillenniumLoFiEditor::deleteSelectedPreset()
{
    if (! selectionIsUserPreset())
    {
        setStatus ("只有「我的预设」里的条目可以删除，工厂预设不能删。", true);
        return;
    }

    const int index = presetBox.getSelectedId() - 1000;
    if (index < 0 || index >= presetNames.size())
        return;

    const auto name = presetNames[index];
    if (juce::AlertWindow::showOkCancelBox (juce::MessageBoxIconType::QuestionIcon,
                                            "删除预设", "确定要删除「" + name + "」吗？",
                                            "删除", "取消", this))
    {
        if (PresetManager::deleteUserPreset (name))
        {
            refreshPresetList();
            setStatus ("已删除预设：" + name);
        }
        else
        {
            setStatus ("删除失败：" + name, true);
        }
    }
}

void MillenniumLoFiEditor::setStatus (const juce::String& text, bool isError)
{
    statusLabel.setColour (juce::Label::textColourId,
                           isError ? juce::Colour (0xffa02020) : juce::Colour (0xff4a4a42));
    statusLabel.setText (text, juce::dontSendNotification);
}

} // namespace mlofi
