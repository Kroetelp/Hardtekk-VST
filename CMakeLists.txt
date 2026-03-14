# HardTekk Kick Synth CMakeLists.txt

cmake_minimum_required(VERSION 3.22)

project(TekkKick VERSION 1.0.0)

add_subdirectory("JUCE 8.0.1.2")

juce_add_plugin(TekkKick
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Tkkt
    PLUGIN_CODE Kick
    FORMATS VST3 Standalone
    PRODUCT_NAME "TekkKick")

target_sources(TekkKick
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp)

target_compile_definitions(TekkKick
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(TekkKick
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
