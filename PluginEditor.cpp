// ==============================================================================
// DATEI 4: PluginEditor.cpp
// ==============================================================================
#include "PluginProcessor.h"
#include "PluginEditor.h"

TekkKickAudioProcessorEditor::TekkKickAudioProcessorEditor (TekkKickAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Plugin Fenstergröße setzen (breiter für Toggle-Buttons neben Sliders)
    setSize (600, 750);

    // Style der Slider definieren (Lineare Slider)
    auto setupSlider = [this](juce::Slider& slider) {
        slider.setSliderStyle(juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
        addAndMakeVisible(slider);
    };

    // Haupt-Slider
    setupSlider(driveSlider);
    setupSlider(punchSlider);
    setupSlider(lengthSlider);
    setupSlider(baseFreqSlider);

    // Click-Slider
    setupSlider(clickSlider);
    setupSlider(clickDecaySlider);

    // Stereo-Slider
    setupSlider(swirlSlider);
    setupSlider(spreadSlider);

    // Distortion-Slider
    setupSlider(foldSlider);

    // Layer-Slider
    setupSlider(subSlider);
    setupSlider(noiseSlider);

    // === FILTER GUI ===
    setupSlider(filterCutoffSlider);
    setupSlider(filterResSlider);

    // Filter-Type ComboBox
    filterTypeComboBox.addItem("Lowpass", 1);
    filterTypeComboBox.addItem("Highpass", 2);
    filterTypeComboBox.addItem("Bandpass", 3);
    filterTypeComboBox.setSelectedId(1);
    addAndMakeVisible(filterTypeComboBox);

    // === ON/OFF SWITCHES ===
    auto setupToggle = [this](juce::ToggleButton& toggle) {
        toggle.setClickingTogglesState(true);
        addAndMakeVisible(toggle);
    };

    setupToggle(driveToggle);
    setupToggle(punchToggle);
    setupToggle(lengthToggle);
    setupToggle(baseFreqToggle);
    setupToggle(clickToggle);
    setupToggle(clickDecayToggle);
    setupToggle(swirlToggle);
    setupToggle(spreadToggle);
    setupToggle(foldToggle);
    setupToggle(subToggle);
    setupToggle(noiseToggle);
    setupToggle(filterToggle);

    // Slider an die Parameter binden (APVTS)
    // Haupt
    driveAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DRIVE", driveSlider);
    punchAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "PUNCH", punchSlider);
    lengthAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "LENGTH", lengthSlider);
    baseFreqAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "BASE_FREQ", baseFreqSlider);
    // Click
    clickAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "CLICK", clickSlider);
    clickDecayAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "CLICK_DECAY", clickDecaySlider);
    // Stereo
    swirlAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SWIRL", swirlSlider);
    spreadAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SPREAD", spreadSlider);
    // Distortion
    foldAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FOLD", foldSlider);
    // Layer
    subAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUB", subSlider);
    noiseAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "NOISE", noiseSlider);
    // Filter
    filterCutoffAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FILTER_CUTOFF", filterCutoffSlider);
    filterResAttach = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FILTER_RES", filterResSlider);
    filterTypeAttach = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "FILTER_TYPE", filterTypeComboBox);

    // On/Off Switches binden
    driveToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "DRIVE_ENABLED", driveToggle);
    punchToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "PUNCH_ENABLED", punchToggle);
    lengthToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "LENGTH_ENABLED", lengthToggle);
    baseFreqToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "BASE_FREQ_ENABLED", baseFreqToggle);
    clickToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "CLICK_ENABLED", clickToggle);
    clickDecayToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "CLICK_DECAY_ENABLED", clickDecayToggle);
    swirlToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "SWIRL_ENABLED", swirlToggle);
    spreadToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "SPREAD_ENABLED", spreadToggle);
    foldToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "FOLD_ENABLED", foldToggle);
    subToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "SUB_ENABLED", subToggle);
    noiseToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "NOISE_ENABLED", noiseToggle);
    filterToggleAttach = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "FILTER_ENABLED", filterToggle);
}

TekkKickAudioProcessorEditor::~TekkKickAudioProcessorEditor() {}

void TekkKickAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dunkler, aggressiver Hintergrund passend zum Hardtekk Vibe
    g.fillAll (juce::Colour::fromRGB (20, 20, 20));

    g.setColour (juce::Colours::red);
    g.setFont (26.0f);
    g.drawFittedText ("TEKK KICK CORE ULTRA", getLocalBounds().removeFromTop(50), juce::Justification::centred, 1);

    // Sektion-Titel
    g.setFont(16.0f);
    g.setColour(juce::Colours::orange);
    g.drawText("MAIN", 25, 55, 400, 20, juce::Justification::left);
    g.drawText("CLICK", 25, 165, 400, 20, juce::Justification::left);
    g.drawText("STEREO", 25, 275, 400, 20, juce::Justification::left);
    g.drawText("DISTORTION", 25, 385, 400, 20, juce::Justification::left);
    g.drawText("LAYER", 25, 495, 400, 20, juce::Justification::left);
    g.drawText("FILTER", 25, 585, 400, 20, juce::Justification::left);

    // Slider-Beschriftungen
    g.setFont(13.0f);
    g.setColour(juce::Colours::white);
    // Main
    g.drawText("Drive", 30, 75, 100, 20, juce::Justification::left);
    g.drawText("Punch", 30, 100, 100, 20, juce::Justification::left);
    g.drawText("Length", 30, 125, 100, 20, juce::Justification::left);
    g.drawText("Sub Freq", 30, 150, 100, 20, juce::Justification::left);
    // Click
    g.drawText("Click Attack", 30, 185, 100, 20, juce::Justification::left);
    g.drawText("Click Decay", 30, 210, 100, 20, juce::Justification::left);
    g.drawText("Binaural Swirl", 30, 235, 100, 20, juce::Justification::left);
    g.drawText("Stereo Spread", 30, 260, 100, 20, juce::Justification::left);
    // Distortion
    g.drawText("Wavefold", 30, 395, 100, 20, juce::Justification::left);
    // Layer
    g.drawText("Sub Layer", 30, 505, 100, 20, juce::Justification::left);
    g.drawText("Noise Layer", 30, 530, 100, 20, juce::Justification::left);
    // Filter
    g.drawText("Filter Cutoff", 30, 615, 100, 20, juce::Justification::left);
    g.drawText("Filter Resonance", 30, 640, 100, 20, juce::Justification::left);
    g.drawText("Filter Type", 30, 665, 100, 20, juce::Justification::left);

    // Switch-Sektion Titel
    g.setFont(16.0f);
    g.setColour(juce::Colours::orange);
    g.drawText("ON / OFF", 500, 55, 80, 20, juce::Justification::centred);
}

void TekkKickAudioProcessorEditor::resized()
{
    // Positionierung der linearen Slider (x, y, breite, höhe)
    int xStart = 130;
    int width = 340;
    int height = 35;
    int toggleX = 490;
    int toggleSize = 20;

    // === ON/OFF SWITCHES (rechts neben Sliders) ===
    driveToggle.setBounds(toggleX, 80, toggleSize, toggleSize);
    punchToggle.setBounds(toggleX, 105, toggleSize, toggleSize);
    lengthToggle.setBounds(toggleX, 130, toggleSize, toggleSize);
    baseFreqToggle.setBounds(toggleX, 155, toggleSize, toggleSize);

    clickToggle.setBounds(toggleX, 190, toggleSize, toggleSize);
    clickDecayToggle.setBounds(toggleX, 215, toggleSize, toggleSize);
    swirlToggle.setBounds(toggleX, 240, toggleSize, toggleSize);
    spreadToggle.setBounds(toggleX, 265, toggleSize, toggleSize);

    foldToggle.setBounds(toggleX, 400, toggleSize, toggleSize);

    subToggle.setBounds(toggleX, 510, toggleSize, toggleSize);
    noiseToggle.setBounds(toggleX, 535, toggleSize, toggleSize);

    filterToggle.setBounds(toggleX, 615, toggleSize, toggleSize);

    // MAIN
    driveSlider.setBounds(xStart, 75, width, height);
    punchSlider.setBounds(xStart, 100, width, height);
    lengthSlider.setBounds(xStart, 125, width, height);
    baseFreqSlider.setBounds(xStart, 150, width, height);

    // CLICK & STEREO
    clickSlider.setBounds(xStart, 185, width, height);
    clickDecaySlider.setBounds(xStart, 210, width, height);
    swirlSlider.setBounds(xStart, 235, width, height);
    spreadSlider.setBounds(xStart, 260, width, height);

    // DISTORTION
    foldSlider.setBounds(xStart, 395, width, height);

    // LAYER
    subSlider.setBounds(xStart, 505, width, height);
    noiseSlider.setBounds(xStart, 530, width, height);

    // FILTER
    filterCutoffSlider.setBounds(xStart, 615, width, height);
    filterResSlider.setBounds(xStart, 640, width, height);
    filterTypeComboBox.setBounds(xStart, 665, width, height);
}
