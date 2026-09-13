/*  Windows XP (Luna) inspired look and feel for the plugin editor. */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace mlofi
{

namespace xp
{
    inline juce::Colour face()          { return juce::Colour (0xffece9d8); }
    inline juce::Colour faceLight()     { return juce::Colour (0xffffffff); }
    inline juce::Colour bevelDark()     { return juce::Colour (0xffaca899); }
    inline juce::Colour fieldBorder()   { return juce::Colour (0xff7f9db9); }
    inline juce::Colour selectBlue()    { return juce::Colour (0xff316ac5); }
    inline juce::Colour windowBlue()    { return juce::Colour (0xff0054e3); }
    inline juce::Colour titleDeep()     { return juce::Colour (0xff0a5be0); }
    inline juce::Colour titleLight()    { return juce::Colour (0xff5aa6ff); }
    inline juce::Colour accentText()    { return juce::Colour (0xff0a3d91); }

    /*  Built through a single helper so a font API change only touches this line. */
    inline juce::Font uiFont (float height, bool bold = false)
    {
        return juce::Font ("SimSun", height, bold ? juce::Font::bold : juce::Font::plain);
    }
}

class XpLookAndFeel : public juce::LookAndFeel_V4
{
public:
    XpLookAndFeel()
    {
        setColour (juce::ResizableWindow::backgroundColourId, xp::face());
        setColour (juce::DocumentWindow::textColourId, juce::Colours::white);

        setColour (juce::Label::textColourId, juce::Colours::black);
        setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);

        setColour (juce::TextButton::buttonColourId, xp::face());
        setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc9d9f0));
        setColour (juce::TextButton::textColourOffId, juce::Colours::black);
        setColour (juce::TextButton::textColourOnId, juce::Colours::black);

        setColour (juce::ComboBox::backgroundColourId, xp::faceLight());
        setColour (juce::ComboBox::textColourId, juce::Colours::black);
        setColour (juce::ComboBox::outlineColourId, xp::fieldBorder());
        setColour (juce::ComboBox::arrowColourId, juce::Colours::black);
        setColour (juce::ComboBox::buttonColourId, juce::Colour (0xffd9d5c6));

        setColour (juce::PopupMenu::backgroundColourId, juce::Colours::white);
        setColour (juce::PopupMenu::textColourId, juce::Colours::black);
        setColour (juce::PopupMenu::highlightedBackgroundColourId, xp::selectBlue());
        setColour (juce::PopupMenu::highlightedTextColourId, juce::Colours::white);

        setColour (juce::Slider::backgroundColourId, juce::Colours::white);
        setColour (juce::Slider::trackColourId, xp::selectBlue());
        setColour (juce::Slider::thumbColourId, juce::Colours::white);
        setColour (juce::Slider::textBoxTextColourId, xp::accentText());
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

        setColour (juce::TextEditor::backgroundColourId, juce::Colours::white);
        setColour (juce::TextEditor::textColourId, juce::Colours::black);
        setColour (juce::TextEditor::outlineColourId, xp::fieldBorder());
        setColour (juce::TextEditor::highlightColourId, xp::selectBlue());
        setColour (juce::TextEditor::highlightedTextColourId, juce::Colours::white);
        setColour (juce::CaretComponent::caretColourId, juce::Colours::black);

        setColour (juce::ToggleButton::textColourId, juce::Colours::black);
        setColour (juce::GroupComponent::textColourId, juce::Colour (0xff003c74));
        setColour (juce::GroupComponent::outlineColourId, juce::Colour (0xffd0cfbf));
    }

    /* ---------------- fonts ---------------- */
    juce::Font getLabelFont (juce::Label&) override { return xp::uiFont (14.0f); }
    juce::Font getTextButtonFont (juce::TextButton&, int) override { return xp::uiFont (14.0f); }
    juce::Font getComboBoxFont (juce::ComboBox&) override { return xp::uiFont (14.0f); }
    juce::Font getPopupMenuFont() override { return xp::uiFont (14.0f); }

    /* ---------------- widgets ---------------- */
    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour&, bool highlighted, bool down) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
        const bool on = button.getToggleState();
        auto top = on ? juce::Colour (0xffc9d9f0) : juce::Colour (0xffffffff);
        auto bottom = on ? juce::Colour (0xffe8f0fb) : juce::Colour (0xffdcd8c8);
        if (down)
            std::swap (top, bottom);
        if (! button.isEnabled())
        {
            top = xp::face();
            bottom = xp::face();
        }

        juce::ColourGradient gradient (top, bounds.getX(), bounds.getY(),
                                       bottom, bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill (gradient);
        g.fillRoundedRectangle (bounds, 3.0f);
        g.setColour (down ? juce::Colour (0xff4a7bbe)
                          : (highlighted ? juce::Colour (0xffc8a24a) : juce::Colour (0xff7ba2d0)));
        g.drawRoundedRectangle (bounds, 3.0f, 1.0f);
    }

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float, float,
                           juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style != juce::Slider::LinearHorizontal)
        {
            LookAndFeel_V4::drawLinearSlider (g, x, y, width, height, sliderPos,
                                              static_cast<float> (x), static_cast<float> (x + width),
                                              style, slider);
            return;
        }

        const float centre = static_cast<float> (y) + static_cast<float> (height) * 0.5f;
        juce::Rectangle<float> groove (static_cast<float> (x) + 6.0f, centre - 2.0f,
                                       static_cast<float> (width) - 12.0f, 4.0f);

        g.setColour (juce::Colours::white);
        g.fillRect (groove);
        g.setColour (xp::fieldBorder());
        g.drawRect (groove, 1.0f);

        auto filled = groove.withRight (juce::jmax (groove.getX(), sliderPos));
        g.setColour (xp::selectBlue());
        g.fillRect (filled);

        const float thumbWidth = 11.0f, thumbHeight = 19.0f;
        juce::Rectangle<float> thumb (sliderPos - thumbWidth * 0.5f, centre - thumbHeight * 0.5f,
                                      thumbWidth, thumbHeight);
        juce::ColourGradient thumbFill (juce::Colours::white, thumb.getX(), thumb.getY(),
                                        juce::Colour (0xffcfcbbb), thumb.getRight(), thumb.getBottom(), false);
        g.setGradientFill (thumbFill);
        g.fillRoundedRectangle (thumb, 2.0f);
        g.setColour (juce::Colour (0xff8e8b7b));
        g.drawRoundedRectangle (thumb, 2.0f, 1.0f);
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool,
                       int, int, int, int, juce::ComboBox& box) override
    {
        juce::Rectangle<float> bounds (0.0f, 0.0f, static_cast<float> (width), static_cast<float> (height));
        g.setColour (juce::Colours::white);
        g.fillRect (bounds);
        g.setColour (xp::fieldBorder());
        g.drawRect (bounds, 1.0f);

        auto buttonArea = bounds.removeFromRight (16.0f).reduced (1.0f);
        juce::ColourGradient gradient (juce::Colours::white, buttonArea.getX(), buttonArea.getY(),
                                       juce::Colour (0xffd9d5c6), buttonArea.getX(), buttonArea.getBottom(), false);
        g.setGradientFill (gradient);
        g.fillRect (buttonArea);
        g.setColour (juce::Colour (0xffa8bfd6));
        g.drawRect (buttonArea, 1.0f);

        juce::Path triangle;
        const float cx = buttonArea.getCentreX(), cy = buttonArea.getCentreY();
        triangle.addTriangle (cx - 4.0f, cy - 2.0f, cx + 4.0f, cy - 2.0f, cx, cy + 3.0f);
        g.setColour (box.isEnabled() ? juce::Colours::black : juce::Colours::grey);
        g.fillPath (triangle);
    }

    void drawGroupComponentOutline (juce::Graphics& g, int width, int height,
                                    const juce::String& text, const juce::Justification&,
                                    juce::GroupComponent&) override
    {
        const float lineY = 9.0f;
        g.setColour (juce::Colour (0xffd0cfbf));
        g.drawRoundedRectangle (1.0f, lineY, static_cast<float> (width) - 2.0f,
                                static_cast<float> (height) - lineY - 1.0f, 3.0f, 1.0f);

        if (text.isNotEmpty())
        {
            g.setFont (xp::uiFont (14.0f, true));
            juce::AttributedString attributed (text);
            attributed.setFont (g.getCurrentFont());
            juce::TextLayout layout;
            layout.createLayout (attributed, 400.0f);
            const int textWidth = static_cast<int> (std::ceil (layout.getWidth())) + 10;
            juce::Rectangle<int> textArea (8, 0, textWidth, 18);
            g.setColour (xp::face());
            g.fillRect (textArea);
            g.setColour (juce::Colour (0xff003c74));
            g.drawText (text, textArea, juce::Justification::centredLeft);
        }
    }

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool highlighted, bool) override
    {
        juce::Rectangle<int> box (0, (button.getHeight() - 13) / 2, 13, 13);
        g.setColour (juce::Colours::white);
        g.fillRect (box);
        g.setColour (highlighted ? juce::Colour (0xff2e6ed9) : xp::fieldBorder());
        g.drawRect (box, 1);

        if (button.getToggleState())
        {
            g.setColour (juce::Colour (0xff1f3f1f));
            g.drawLine (static_cast<float> (box.getX() + 3), static_cast<float> (box.getY() + 7),
                        static_cast<float> (box.getX() + 5), static_cast<float> (box.getY() + 10), 2.0f);
            g.drawLine (static_cast<float> (box.getX() + 5), static_cast<float> (box.getY() + 10),
                        static_cast<float> (box.getX() + 10), static_cast<float> (box.getY() + 3), 2.0f);
        }

        g.setColour (juce::Colours::black);
        g.setFont (xp::uiFont (14.0f));
        g.drawText (button.getButtonText(), box.getRight() + 6, 0,
                    juce::jmax (0, button.getWidth() - box.getWidth() - 6), button.getHeight(),
                    juce::Justification::centredLeft, false);
    }
};

} // namespace mlofi
