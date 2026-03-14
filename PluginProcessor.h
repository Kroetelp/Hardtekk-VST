// ==============================================================================
// DATEI 1: PluginProcessor.h
// ==============================================================================
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_dsp/juce_dsp.h>

class TekkKickAudioProcessor  : public juce::AudioProcessor
{
public:
    TekkKickAudioProcessor();
    ~TekkKickAudioProcessor() override;

    void prepareToPlay (double sampleRate, [[maybe_unused]] int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "TekkKick"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram ([[maybe_unused]] int index) override { (void)index; }
    const juce::String getProgramName ([[maybe_unused]] int index) override { (void)index; return {}; }
    void changeProgramName ([[maybe_unused]] int index, [[maybe_unused]] const juce::String& newName) override { (void)index; (void)newName; }
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Unser Parameter-Baum, der mit FL Studio kommuniziert
    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    // Synthesizer Variablen
    double currentSampleRate = 44100.0;
    bool isPlaying = false;
    float phase = 0.0f;           // Haupt-Kick Phase
    float subPhase = 0.0f;         // Sub-Layer Phase
    float envAmpPos = 0.0f;       // Amplitude Envelope
    float envPitchPos = 0.0f;      // Pitch Envelope
    float sampleCount = 0.0f;       // Zähler für Click am Decay-Ende
    float noisePhase = 0.0f;        // Noise Phase

    // Cache für Parameter
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* punchParam = nullptr;
    std::atomic<float>* lengthParam = nullptr;
    std::atomic<float>* baseFreqParam = nullptr;
    std::atomic<float>* clickParam = nullptr;      // Click am Attack
    std::atomic<float>* clickDecayParam = nullptr; // Click am Decay-Ende
    std::atomic<float>* swirlParam = nullptr;       // Binaural Swirl
    std::atomic<float>* foldParam = nullptr;        // Wavefolding
    std::atomic<float>* subParam = nullptr;        // Sub-Layer Mix
    std::atomic<float>* noiseParam = nullptr;      // Noise-Layer
    std::atomic<float>* spreadParam = nullptr;      // Stereo-Spread

    // === FILTER PARAMETER ===
    std::atomic<float>* filterCutoffParam = nullptr;  // Filter Cutoff (Hz)
    std::atomic<float>* filterResParam = nullptr;     // Filter Resonance
    std::atomic<float>* filterTypeParam = nullptr;     // Filter Type (0=LP, 1=HP, 2=BP)

    // === FILTER STATUS ===
    float z1L = 0.0f, z2L = 0.0f;  // Linker Filter Zustand
    float z1R = 0.0f, z2R = 0.0f;  // Rechter Filter Zustand

    // === ON/OFF SWITCHES ===
    std::atomic<float>* driveEnabledParam = nullptr;
    std::atomic<float>* punchEnabledParam = nullptr;
    std::atomic<float>* lengthEnabledParam = nullptr;
    std::atomic<float>* baseFreqEnabledParam = nullptr;
    std::atomic<float>* clickEnabledParam = nullptr;
    std::atomic<float>* clickDecayEnabledParam = nullptr;
    std::atomic<float>* swirlEnabledParam = nullptr;
    std::atomic<float>* spreadEnabledParam = nullptr;
    std::atomic<float>* foldEnabledParam = nullptr;
    std::atomic<float>* subEnabledParam = nullptr;
    std::atomic<float>* noiseEnabledParam = nullptr;
    std::atomic<float>* filterEnabledParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TekkKickAudioProcessor)
};
