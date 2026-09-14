#include "ScopeDisplay.h"

#include "Parameters.h"

namespace mlofi
{

ScopeDisplay::ScopeDisplay (ScopeData& sharedData)
    : data (sharedData)
{
    left.reserve (ScopeData::rawSize);
    right.reserve (ScopeData::rawSize);
    envelope.reserve (ScopeData::numBuckets);
    setInterceptsMouseClicks (true, false);
    setMouseCursor (juce::MouseCursor::PointingHandCursor);
    setTooltip (u8("点击切换示波器模式"));
}

void ScopeDisplay::setMode (Mode newMode)
{
    mode = newMode;
    repaint();
}

juce::String ScopeDisplay::modeName() const
{
    switch (mode)
    {
        case Mode::rolling:     return u8("1.5 秒滚动");
        case Mode::vectorscope: return u8("立体声向量");
        default:                return u8("20 ms 触发");
    }
}

void ScopeDisplay::mouseDown (const juce::MouseEvent&)
{
    const int next = (static_cast<int> (mode) + 1) % static_cast<int> (Mode::numModes);
    mode = static_cast<Mode> (next);
    repaint();
    if (onModeChanged != nullptr)
        onModeChanged (mode);
}

/* ------------------------------------------------------------------ */

void ScopeDisplay::drawFrame (juce::Graphics& g, juce::Rectangle<float> screen, juce::Colour background)
{
    g.setColour (juce::Colours::white);
    g.fillRect (screen);
    auto inner = screen.reduced (1.0f);
    g.setColour (background);
    g.fillRect (inner);
    g.setColour (juce::Colour (0xff3a3a35));
    g.drawRect (screen, 1.0f);
}

void ScopeDisplay::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    if (area.getWidth() < 8.0f || area.getHeight() < 8.0f)
        return;

    switch (mode)
    {
        case Mode::rolling:     drawRolling (g, area); break;
        case Mode::vectorscope: drawVectorscope (g, area); break;
        default:                drawTriggered (g, area); break;
    }
}

void ScopeDisplay::drawTriggered (juce::Graphics& g, juce::Rectangle<float> screen)
{
    drawFrame (g, screen, juce::Colour (0xff04120a));
    auto area = screen.reduced (2.0f);

    g.setColour (juce::Colour (0xff0e3524));
    for (int i = 1; i < 6; ++i)
        g.drawVerticalLine (juce::roundToInt (area.getX() + area.getWidth() * i / 6.0f),
                            area.getY(), area.getBottom());
    for (int i = 1; i < 4; ++i)
        g.drawHorizontalLine (juce::roundToInt (area.getY() + area.getHeight() * i / 4.0f),
                              area.getX(), area.getRight());

    const float centre = area.getCentreY();
    g.setColour (juce::Colour (0xff1d5c3c));
    g.drawHorizontalLine (juce::roundToInt (centre), area.getX(), area.getRight());

    data.copyRaw (2048, left, right);
    const int count = static_cast<int> (left.size());
    if (count < 32)
        return;

    /* trigger on a rising zero crossing a little way into the buffer */
    int trigger = 0;
    for (int i = count / 3; i > 1; --i)
    {
        if (left[static_cast<std::size_t> (i - 1)] < 0.0f && left[static_cast<std::size_t> (i)] >= 0.0f)
        {
            trigger = i;
            break;
        }
    }

    const int visible = count - trigger;
    if (visible < 8)
        return;

    juce::Path path;
    const float halfHeight = area.getHeight() * 0.42f;
    for (int i = 0; i < visible; ++i)
    {
        const float x = area.getX() + area.getWidth() * static_cast<float> (i) / static_cast<float> (visible - 1);
        const float value = juce::jlimit (-1.2f, 1.2f, left[static_cast<std::size_t> (trigger + i)]);
        const float y = centre - value * halfHeight;
        if (i == 0)
            path.startNewSubPath (x, y);
        else
            path.lineTo (x, y);
    }

    g.setColour (juce::Colour (0x4650ff8c));
    g.strokePath (path, juce::PathStrokeType (4.0f));
    g.setColour (juce::Colour (0xff7dffb0));
    g.strokePath (path, juce::PathStrokeType (1.3f));

    g.setColour (juce::Colour (0xff4faa78));
    g.setFont (10.0f);
    g.drawText (modeName(), area.reduced (4.0f, 2.0f), juce::Justification::topLeft, false);
}

void ScopeDisplay::drawRolling (juce::Graphics& g, juce::Rectangle<float> screen)
{
    drawFrame (g, screen, juce::Colour (0xff0b1420));
    auto area = screen.reduced (2.0f);

    g.setColour (juce::Colour (0xff1b3049));
    for (int i = 1; i < 6; ++i)
        g.drawVerticalLine (juce::roundToInt (area.getX() + area.getWidth() * i / 6.0f),
                            area.getY(), area.getBottom());
    const float centre = area.getCentreY();
    g.setColour (juce::Colour (0xff24486d));
    g.drawHorizontalLine (juce::roundToInt (centre), area.getX(), area.getRight());

    data.copyEnvelope (envelope);
    const int count = static_cast<int> (envelope.size());
    if (count < 2)
        return;

    juce::Path upper, lower;
    const float scale = area.getHeight() * 0.44f;
    for (int i = 0; i < count; ++i)
    {
        const float x = area.getX() + area.getWidth() * static_cast<float> (i) / static_cast<float> (count - 1);
        const float top = centre - juce::jlimit (0.0f, 1.2f, envelope[static_cast<std::size_t> (i)].maxL) * scale;
        const float bottom = centre - juce::jlimit (0.0f, 1.2f, envelope[static_cast<std::size_t> (i)].minL) * scale;
        if (i == 0)
        {
            upper.startNewSubPath (x, top);
            lower.startNewSubPath (x, bottom);
        }
        else
        {
            upper.lineTo (x, top);
            lower.lineTo (x, bottom);
        }
    }

    g.setColour (juce::Colour (0xffe08820));
    g.strokePath (upper, juce::PathStrokeType (1.1f));
    g.strokePath (lower, juce::PathStrokeType (1.1f));

    g.setColour (juce::Colour (0xff8a6a3a));
    g.setFont (10.0f);
    g.drawText (modeName(), area.reduced (4.0f, 2.0f), juce::Justification::topLeft, false);
}

void ScopeDisplay::drawVectorscope (juce::Graphics& g, juce::Rectangle<float> screen)
{
    drawFrame (g, screen, juce::Colour (0xff050f0e));
    auto area = screen.reduced (3.0f);
    const auto centre = area.getCentre();
    const float radius = juce::jmin (area.getWidth(), area.getHeight()) * 0.5f - 2.0f;
    if (radius < 4.0f)
        return;

    g.setColour (juce::Colour (0xff12403c));
    g.drawLine (centre.x - radius, centre.y, centre.x + radius, centre.y, 1.0f);
    g.drawLine (centre.x, centre.y - radius, centre.x, centre.y + radius, 1.0f);
    g.drawEllipse (centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f, 1.0f);
    g.setColour (juce::Colour (0xff0d332f));
    g.drawEllipse (centre.x - radius * 0.5f, centre.y - radius * 0.5f,
                   radius, radius, 1.0f);

    data.copyRaw (1400, left, right);
    const int count = static_cast<int> (left.size());
    if (count < 16)
        return;

    /*  Draw the trace in a few chunks so the older part fades out without
        needing one draw call per sample. */
    const int chunks = 8;
    for (int chunk = 0; chunk < chunks; ++chunk)
    {
        const int start = count * chunk / chunks;
        const int end = count * (chunk + 1) / chunks;
        juce::Path path;
        for (int i = start; i < end; ++i)
        {
            const float x = centre.x + juce::jlimit (-1.2f, 1.2f, right[static_cast<std::size_t> (i)]) * radius * 0.95f;
            const float y = centre.y - juce::jlimit (-1.2f, 1.2f, left[static_cast<std::size_t> (i)]) * radius * 0.95f;
            if (i == start)
                path.startNewSubPath (x, y);
            else
                path.lineTo (x, y);
        }
        const float alpha = 0.18f + 0.82f * static_cast<float> (chunk) / static_cast<float> (chunks - 1);
        g.setColour (juce::Colour (0xff4ec9c0).withAlpha (alpha));
        g.strokePath (path, juce::PathStrokeType (1.5f, juce::PathStrokeType::curved,
                                                  juce::PathStrokeType::rounded));
    }

    g.setColour (juce::Colour (0xff3f8f89));
    g.setFont (10.0f);
    g.drawText (modeName(), area.reduced (2.0f, 0.0f), juce::Justification::topLeft, false);
}

} // namespace mlofi
