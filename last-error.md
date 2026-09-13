# 构建失败记录

时间: 2026-09-13T15:48:18.0698134+00:00
运行: 34766474813 (第 1 次)
提交: ab02885c20579b07fa497f94b67b0996d5ecc042

===== 配置日志 =====
-- Building for: Visual Studio 18 2026
-- The C compiler identification is MSVC 19.51.36256.0
-- The CXX compiler identification is MSVC 19.51.36256.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/18/Enterprise/VC/Tools/MSVC/14.51.36231/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/18/Enterprise/VC/Tools/MSVC/14.51.36231/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring juceaide
-- Building juceaide
-- Exporting juceaide
-- Testing juceaide
-- Finished setting up juceaide
-- Configuring done (113.0s)
-- Generating done (0.1s)
-- Build files have been written to: D:/a/millennium-lofi/millennium-lofi/build

===== 编译日志 =====
MSBuild version 18.9.1+a81b43525 for .NET Framework

  Checking File Globs
  1>Checking Build System
  Generating MillenniumLoFi_artefacts/JuceLibraryCode/MillenniumLoFi_resources.rc
  Building Custom Rule D:/a/millennium-lofi/millennium-lofi/CMakeLists.txt
  Building VST3 manifest helper for MillenniumLoFi
  MillenniumLoFi_rc_lib.vcxproj -> D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi_rc_lib.dir\Release\MillenniumLoFi_rc_lib.lib
  -- The CXX compiler identification is MSVC 19.51.36256.0
  -- Detecting CXX compiler ABI info
  -- Detecting CXX compiler ABI info - done
  -- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/18/Enterprise/VC/Tools/MSVC/14.51.36231/bin/Hostx64/x64/cl.exe - skipped
  -- Detecting CXX compile features
  -- Detecting CXX compile features - done
  -- Configuring done (1.8s)
  -- Generating done (0.0s)
  -- Build files have been written to: D:/a/millennium-lofi/millennium-lofi/build/vst3_helpers/MillenniumLoFi
  MSBuild version 18.9.1+a81b43525 for .NET Framework
  
    1>Checking Build System
    Building Custom Rule D:/a/millennium-lofi/millennium-lofi/juce/extras/Build/CMake/juce_vst3_helper/CMakeLists.txt
    juce_VST3ManifestHelper.cpp
    vst3_helper.vcxproj -> D:\a\millennium-lofi\millennium-lofi\build\vst3_helpers\MillenniumLoFi\vst3_helper.exe
    Building Custom Rule D:/a/millennium-lofi/millennium-lofi/juce/extras/Build/CMake/juce_vst3_helper/CMakeLists.txt
  Building Custom Rule D:/a/millennium-lofi/millennium-lofi/CMakeLists.txt
  Building Custom Rule D:/a/millennium-lofi/millennium-lofi/CMakeLists.txt
  PluginProcessor.cpp
  PluginEditor.cpp
  juce_audio_processors.cpp
  juce_gui_extra.cpp
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(53,31): error C3861: 'factoryPresets': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,26): error C2530: 'value': references must be initialized [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,26): error C3531: 'value': a symbol whose type contains 'auto' must have an initializer [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,32): error C2143: syntax error: missing ';' before ':' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,82): error C2143: syntax error: missing ';' before ')' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(57,13): error C2660: 'mlofi::PresetManager::setParameter': function does not take 1 arguments [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(43,17):
      see declaration of 'mlofi::PresetManager::setParameter'
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(57,13):
      while trying to match the argument list '(juce::AudioProcessorValueTreeState)'
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(74,27): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(53,31): error C3861: 'factoryPresets': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,26): error C2530: 'value': references must be initialized [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,26): error C3531: 'value': a symbol whose type contains 'auto' must have an initializer [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,32): error C2143: syntax error: missing ';' before ':' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(56,82): error C2143: syntax error: missing ';' before ')' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(57,13): error C2660: 'mlofi::PresetManager::setParameter': function does not take 1 arguments [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(43,17):
      see declaration of 'mlofi::PresetManager::setParameter'
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(57,13):
      while trying to match the argument list '(juce::AudioProcessorValueTreeState)'
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(74,27): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(74,32): error C2065: 'bypass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(74,32): error C2065: 'bypass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginProcessor.cpp')
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(12,44): error C3861: 'createLayout': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(37,25): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(37,30): error C2065: 'bitDepth': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(38,27): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(38,32): error C2065: 'sampleRate': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(39,28): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(39,33): error C2065: 'tapeLowpass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(40,20): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(40,25): error C2065: 'wow': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(41,22): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(41,27): error C2065: 'drive': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(42,23): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(42,28): error C2065: 'hissDb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(43,22): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(43,27): error C2065: 'humDb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(45,29): error C3861: 'codecValues': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(47,75): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(47,80): error C2065: 'codecKbps': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(50,32): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(50,37): error C2065: 'speakerHighpass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(51,31): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(51,36): error C2065: 'speakerLowpass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(52,26): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(52,31): error C2065: 'resonance': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(53,23): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(53,28): error C2065: 'reverb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(54,22): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(54,27): error C2065: 'width': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(55,25): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(55,30): error C2065: 'outputDb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(71,37): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(71,42): error C2065: 'bypass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,51): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PresetManager.h(16,7): error C2011: 'mlofi::PresetManager': 'class' type redefinition [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,29): error C2664: 'std::atomic<float> *juce::AudioProcessorValueTreeState::getRawParameterValue(juce::StringRef) noexcept const': cannot convert argument 1 from 'const float' to 'juce::StringRef' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
      'juce::StringRef::StringRef': no overloaded function could convert all the argument types
          D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_StringRef.h(151,1):
          could be 'juce::StringRef::StringRef(juce::StringRef &&)'
              D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
              'juce::StringRef::StringRef(juce::StringRef &&)': cannot convert argument 1 from 'const float' to 'juce::StringRef &&'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Reason: cannot convert from 'const float' to 'juce::StringRef'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Conversion requires a second user-defined-conversion operator or constructor
          D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_StringRef.h(151,1):
          or       'juce::StringRef::StringRef(const juce::StringRef &)'
              D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
              'juce::StringRef::StringRef(const juce::StringRef &)': cannot convert argument 1 from 'const float' to 'const juce::StringRef &'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Reason: cannot convert from 'const float' to 'const juce::StringRef'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Conversion requires a second user-defined-conversion operator or constructor
          D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_StringRef.h(101,5):
          or       'juce::StringRef::StringRef(const std::string &)'
              D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
              'juce::StringRef::StringRef(const std::string &)': cannot convert argument 1 from 'const float' to 'const std::string &'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Reason: cannot convert from 'const float' to 'const std::string'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Conversion requires a second user-defined-conversion operator or constructor
          D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_StringRef.h(94,5):
          or       'juce::StringRef::StringRef(const juce::String &) noexcept'
              D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
              'juce::StringRef::StringRef(const juce::String &) noexcept': cannot convert argument 1 from 'const float' to 'const juce::String &'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Reason: cannot convert from 'const float' to 'const juce::String'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Conversion requires a second user-defined-conversion operator or constructor
          D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_StringRef.h(87,5):
          or       'juce::StringRef::StringRef(juce::String::CharPointerType) noexcept'
              D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
              'juce::StringRef::StringRef(juce::String::CharPointerType) noexcept': cannot convert argument 1 from 'const float' to 'juce::String::CharPointerType'
                  D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
                  Conversion requires a second user-defined-conversion operator or constructor
          D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_StringRef.h(81,5):
          or       'juce::StringRef::StringRef(const char *) noexcept'
              D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
              'juce::StringRef::StringRef(const char *) noexcept': cannot convert argument 1 from 'const float' to 'const char *'
          D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,56):
          while trying to match the argument list '(const float)'
      D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_audio_processors\utilities\juce_AudioProcessorValueTreeState.h(346,25):
      see declaration of 'juce::AudioProcessorValueTreeState::getRawParameterValue'
      D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,29):
      while trying to match the argument list '(const float)'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginProcessor.cpp(74,17): error C2737: 'mix': const object must be initialized [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(41,24): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(41,24): error C3861: 'userPresetDirectory': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(42,58): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(42,58): error C3861: 'userPresetDirectory': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(54,31): error C3861: 'factoryPresets': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(57,40): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(57,45): error C2065: 'wow': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(58,40): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(58,45): error C2065: 'drive': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(59,40): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(59,45): error C2065: 'hissDb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(60,40): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(60,45): error C2065: 'humDb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(61,40): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(61,45): error C2065: 'tapeLowpass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(63,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(63,51): error C2065: 'bitDepth': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(64,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(64,51): error C2065: 'sampleRate': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(69,27): error C3861: 'codecChoices': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(71,16): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(71,21): error C2065: 'codecKbps': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(73,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(73,51): error C2065: 'speakerHighpass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(74,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(74,51): error C2065: 'speakerLowpass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(75,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(75,51): error C2065: 'resonance': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(76,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(76,51): error C2065: 'reverb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(77,46): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(77,51): error C2065: 'width': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(82,16): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(82,21): error C2065: 'bypass': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(99,16): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(99,21): error C2065: 'mix': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(101,16): error C2653: 'ids': is not a class or namespace name [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(101,21): error C2065: 'outputDb': undeclared identifier [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(251,27): error C3861: 'factoryPresets': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(255,43): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(255,43): error C3861: 'userPresetNames': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(291,32): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(291,32): error C3861: 'loadUserPreset': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(299,27): error C3861: 'factoryPresets': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(303,24): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(303,24): error C3861: 'applyFactoryPreset': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(320,24): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(320,24): error C3861: 'saveUserPreset': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(331,83): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(331,83): error C3861: 'userPresetDirectory': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(356,28): error C2027: use of undefined type 'mlofi::PresetManager' [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
      D:\a\millennium-lofi\millennium-lofi\Source\Parameters.h(16,7):
      see declaration of 'mlofi::PresetManager'
  
D:\a\millennium-lofi\millennium-lofi\Source\PluginEditor.cpp(356,28): error C3861: 'deleteUserPreset': identifier not found [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  juce_gui_basics.cpp
  juce_gui_basics_2.cpp
D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_gui_basics\juce_gui_basics.cpp(61): warning : This version of JUCE does not use the splash screen, the flag JUCE_DISPLAY_SPLASH_SCREEN is ignored [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  juce_gui_basics_3.cpp
  juce_gui_basics_4.cpp
  juce_gui_basics_5.cpp
  juce_graphics.cpp
  juce_graphics_Harfbuzz.cpp
  juce_events.cpp
  juce_core.cpp
  juce_core_CompilationTime.cpp
  juce_data_structures.cpp
  juce_audio_processors_headless.cpp
  juce_audio_processors_headless_ara.cpp
  juce_audio_processors_headless_lv2_libs.cpp
  juce_audio_basics.cpp
  juce_audio_utils.cpp
  juce_audio_formats.cpp
  juce_audio_devices.cpp

