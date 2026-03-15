// ==============================================================================
// DATEI 3: PluginEditor.h
// ==============================================================================
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class TekkKickAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TekkKickAudioProcessorEditor (TekkKickAudioProcessor&);
    ~TekkKickAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TekkKickAudioProcessor& audioProcessor;

    // GUI Elemente (Slider)
    juce::Slider driveSlider;
    juce::Slider punchSlider;
    juce::Slider lengthSlider;
    juce::Slider baseFreqSlider;
    juce::Slider clickSlider;
    juce::Slider clickDecaySlider;
    juce::Slider swirlSlider;
    juce::Slider spreadSlider;
    juce::Slider foldSlider;
    juce::Slider subSlider;
    juce::Slider noiseSlider;

    // === FILTER GUI ===
    juce::Slider filterCutoffSlider;
    juce::Slider filterResSlider;
    juce::ComboBox filterTypeComboBox;

    // === ON/OFF SWITCHES ===
    juce::ToggleButton driveToggle;
    juce::ToggleButton punchToggle;
    juce::ToggleButton lengthToggle;
    juce::ToggleButton baseFreqToggle;
    juce::ToggleButton clickToggle;
    juce::ToggleButton clickDecayToggle;
    juce::ToggleButton swirlToggle;
    juce::ToggleButton spreadToggle;
    juce::ToggleButton foldToggle;
    juce::ToggleButton subToggle;
    juce::ToggleButton noiseToggle;
    juce::ToggleButton filterToggle;

    // === EXPORT BUTTON ===
    juce::TextButton exportButton;
    std::unique_ptr<juce::FileChooser> fileChooser; // Wichtig für das Datei-Speichern-Fenster

    // Verbindungen zwischen Slider und der Audio-Engine
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<SliderAttachment> driveAttach;
    std::unique_ptr<SliderAttachment> punchAttach;
    std::unique_ptr<SliderAttachment> lengthAttach;
    std::unique_ptr<SliderAttachment> baseFreqAttach;
    std::unique_ptr<SliderAttachment> clickAttach;
    std::unique_ptr<SliderAttachment> clickDecayAttach;
    std::unique_ptr<SliderAttachment> swirlAttach;
    std::unique_ptr<SliderAttachment> spreadAttach;
    std::unique_ptr<SliderAttachment> foldAttach;
    std::unique_ptr<SliderAttachment> subAttach;
    std::unique_ptr<SliderAttachment> noiseAttach;
    // Filter
    std::unique_ptr<SliderAttachment> filterCutoffAttach;
    std::unique_ptr<SliderAttachment> filterResAttach;
    std::unique_ptr<ComboBoxAttachment> filterTypeAttach;

    // On/Off Switches
    std::unique_ptr<ButtonAttachment> driveToggleAttach;
    std::unique_ptr<ButtonAttachment> punchToggleAttach;
    std::unique_ptr<ButtonAttachment> lengthToggleAttach;
    std::unique_ptr<ButtonAttachment> baseFreqToggleAttach;
    std::unique_ptr<ButtonAttachment> clickToggleAttach;
    std::unique_ptr<ButtonAttachment> clickDecayToggleAttach;
    std::unique_ptr<ButtonAttachment> swirlToggleAttach;
    std::unique_ptr<ButtonAttachment> spreadToggleAttach;
    std::unique_ptr<ButtonAttachment> foldToggleAttach;
    std::unique_ptr<ButtonAttachment> subToggleAttach;
    std::unique_ptr<ButtonAttachment> noiseToggleAttach;
    std::unique_ptr<ButtonAttachment> filterToggleAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TekkKickAudioProcessorEditor)
};
