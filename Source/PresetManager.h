/*  Factory and user presets.

    User presets are plain JSON files in
        %APPDATA%\Millennium LoFi\Presets
    so they survive plugin updates and can be copied around by hand. */

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Parameters.h"

namespace mlofi
{

class PresetManager
{
public:
    static juce::File userPresetDirectory()
    {
        auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                       .getChildFile ("Millennium LoFi")
                       .getChildFile ("Presets");
        if (! dir.isDirectory())
            dir.createDirectory();
        return dir;
    }

    static juce::File fileForName (const juce::String& name)
    {
        return userPresetDirectory().getChildFile (juce::File::createLegalFileName (name) + ".mlofi");
    }

    static juce::StringArray userPresetNames()
    {
        juce::StringArray names;
        for (const auto& file : userPresetDirectory().findChildFiles (juce::File::findFiles, false, "*.mlofi"))
            names.add (file.getFileNameWithoutExtension());
        names.sort (true);
        return names;
    }

    static void setParameter (juce::AudioProcessorValueTreeState& state,
                              const juce::String& id, float value)
    {
        if (auto* parameter = state.getParameter (id))
            if (auto* ranged = dynamic_cast<juce::RangedAudioParameter*> (parameter))
                ranged->setValueNotifyingHost (ranged->convertTo0to1 (value));
    }

    static void applyFactoryPreset (juce::AudioProcessorValueTreeState& state, int index)
    {
        const auto& presets = factoryPresets();
        if (index < 0 || index >= static_cast<int> (presets.size()))
            return;
        for (const auto& value : presets[static_cast<std::size_t> (index)].values)
            setParameter (state, value.id, value.value);
    }

    static bool saveUserPreset (juce::AudioProcessorValueTreeState& state, const juce::String& name)
    {
        const auto trimmed = name.trim();
        if (trimmed.isEmpty())
            return false;

        auto* object = new juce::DynamicObject();
        object->setProperty ("name", trimmed);
        object->setProperty ("format", 1);

        for (auto* parameter : state.processor.getParameters())
            if (auto* ranged = dynamic_cast<juce::RangedAudioParameter*> (parameter))
            {
                const auto id = ranged->paramID;
                if (id == ids::bypass)   // bypass is a session state, not part of a sound
                    continue;
                object->setProperty (id, ranged->convertFrom0to1 (ranged->getValue()));
            }

        const juce::var tree (object);
        return fileForName (trimmed).replaceWithText (juce::JSON::toString (tree, false));
    }

    static bool loadUserPreset (juce::AudioProcessorValueTreeState& state, const juce::String& name)
    {
        const auto file = fileForName (name);
        if (! file.existsAsFile())
            return false;

        const auto tree = juce::JSON::parse (file.loadFileAsString());
        if (auto* object = tree.getDynamicObject())
        {
            for (const auto& property : object->getProperties())
            {
                /*  compare as String: comparing an Identifier with a string
                    literal is ambiguous for MSVC */
                const auto key = property.name.toString();
                if (key == "name" || key == "format")
                    continue;
                setParameter (state, key,
                              static_cast<float> (static_cast<double> (property.value)));
            }
            return true;
        }
        return false;
    }

    static bool deleteUserPreset (const juce::String& name)
    {
        const auto file = fileForName (name);
        return file.existsAsFile() && file.deleteFile();
    }
};

} // namespace mlofi
