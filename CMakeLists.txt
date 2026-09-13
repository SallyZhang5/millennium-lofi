cmake_minimum_required(VERSION 3.22)

project(MillenniumLoFi VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/MP /utf-8 /permissive-)
endif()

# ---------------------------------------------------------------------------
# JUCE is fetched at configure time, so the repository stays small.
# Change MLOFI_JUCE_TAG to move to another JUCE release.
# ---------------------------------------------------------------------------
set(MLOFI_JUCE_TAG "8.0.15" CACHE STRING "JUCE release tag")

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG ${MLOFI_JUCE_TAG}
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

# ---------------------------------------------------------------------------
# the plugin
# ---------------------------------------------------------------------------
juce_add_plugin(MillenniumLoFi
    COMPANY_NAME "Millennium LoFi"
    BUNDLE_ID "com.millenniumlofi.plugin"
    PLUGIN_MANUFACTURER_CODE Mlof
    PLUGIN_CODE Mlf1
    PRODUCT_NAME "Millennium LoFi"
    DESCRIPTION "Millennium era lo-fi processor for the mix bus"
    FORMATS VST3 Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES "Fx")

target_sources(MillenniumLoFi PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)

target_compile_definitions(MillenniumLoFi PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_MODAL_LOOPS_PERMITTED=1)

target_include_directories(MillenniumLoFi PRIVATE Source)

target_link_libraries(MillenniumLoFi
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags)

# ---------------------------------------------------------------------------
# offline renderer, built from the same engine, no JUCE required
# ---------------------------------------------------------------------------
option(MLOFI_BUILD_TESTS "Build the offline render/self test tool" ON)

if (MLOFI_BUILD_TESTS)
    add_executable(render_test Test/render_test.cpp)
    if (MSVC)
        target_compile_options(render_test PRIVATE /utf-8)
    endif()
endif()
