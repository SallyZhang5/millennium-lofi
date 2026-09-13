# 构建失败记录

时间: 2026-09-13T15:36:22.3000712+00:00
运行: 34765878559 (第 1 次)
提交: 47ed706cfa75dd67d6f1f319c01a653eee93798f

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
-- Configuring done (115.7s)
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
  -- Configuring done (1.9s)
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
D:\a\millennium-lofi\millennium-lofi\Source\PresetManager.h(94,29): error C2593: 'operator ==' is ambiguous [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
      D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_Identifier.h(96,17):
      could be 'bool juce::Identifier::operator ==(juce::StringRef) noexcept const'
      D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_Identifier.h(90,17):
      or       'bool juce::Identifier::operator ==(const juce::Identifier &) noexcept const'
      D:\a\millennium-lofi\millennium-lofi\Source\PresetManager.h(94,29):
      while trying to match the argument list '(const juce::Identifier, const char [5])'
  
D:\a\millennium-lofi\millennium-lofi\Source\PresetManager.h(94,56): error C2593: 'operator ==' is ambiguous [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
  (compiling source file '../Source/PluginEditor.cpp')
      D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_Identifier.h(96,17):
      could be 'bool juce::Identifier::operator ==(juce::StringRef) noexcept const'
      D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_core\text\juce_Identifier.h(90,17):
      or       'bool juce::Identifier::operator ==(const juce::Identifier &) noexcept const'
      D:\a\millennium-lofi\millennium-lofi\Source\PresetManager.h(94,56):
      while trying to match the argument list '(const juce::Identifier, const char [7])'
  
  juce_gui_basics.cpp
  juce_gui_basics_2.cpp
  juce_gui_basics_3.cpp
D:\a\millennium-lofi\millennium-lofi\juce\modules\juce_gui_basics\juce_gui_basics.cpp(61): warning : This version of JUCE does not use the splash screen, the flag JUCE_DISPLAY_SPLASH_SCREEN is ignored [D:\a\millennium-lofi\millennium-lofi\build\MillenniumLoFi.vcxproj]
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

