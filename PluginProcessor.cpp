// ==============================================================================
// DATEI 2: PluginProcessor.cpp
// ==============================================================================
#include "PluginProcessor.h"
#include "PluginEditor.h"

TekkKickAudioProcessor::TekkKickAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ), apvts(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    // Verknüpfe die internen Variablen mit den Reglern
    driveParam = apvts.getRawParameterValue("DRIVE");
    punchParam = apvts.getRawParameterValue("PUNCH");
    lengthParam = apvts.getRawParameterValue("LENGTH");
    baseFreqParam = apvts.getRawParameterValue("BASE_FREQ");
    clickParam = apvts.getRawParameterValue("CLICK");
    clickDecayParam = apvts.getRawParameterValue("CLICK_DECAY");
    swirlParam = apvts.getRawParameterValue("SWIRL");
    foldParam = apvts.getRawParameterValue("FOLD");
    subParam = apvts.getRawParameterValue("SUB");
    noiseParam = apvts.getRawParameterValue("NOISE");
    spreadParam = apvts.getRawParameterValue("SPREAD");
    filterCutoffParam = apvts.getRawParameterValue("FILTER_CUTOFF");
    filterResParam = apvts.getRawParameterValue("FILTER_RES");
    filterTypeParam = apvts.getRawParameterValue("FILTER_TYPE");

    // Pre-Distortion EQ Parameter (NEU)
    preEqFreqParam = apvts.getRawParameterValue("PRE_EQ_FREQ");
    preEqGainParam = apvts.getRawParameterValue("PRE_EQ_GAIN");
    preEqEnabledParam = apvts.getRawParameterValue("PRE_EQ_ENABLED");

    // On/Off Switches verknüpfen
    driveEnabledParam = apvts.getRawParameterValue("DRIVE_ENABLED");
    punchEnabledParam = apvts.getRawParameterValue("PUNCH_ENABLED");
    lengthEnabledParam = apvts.getRawParameterValue("LENGTH_ENABLED");
    baseFreqEnabledParam = apvts.getRawParameterValue("BASE_FREQ_ENABLED");
    clickEnabledParam = apvts.getRawParameterValue("CLICK_ENABLED");
    clickDecayEnabledParam = apvts.getRawParameterValue("CLICK_DECAY_ENABLED");
    swirlEnabledParam = apvts.getRawParameterValue("SWIRL_ENABLED");
    spreadEnabledParam = apvts.getRawParameterValue("SPREAD_ENABLED");
    foldEnabledParam = apvts.getRawParameterValue("FOLD_ENABLED");
    subEnabledParam = apvts.getRawParameterValue("SUB_ENABLED");
    noiseEnabledParam = apvts.getRawParameterValue("NOISE_ENABLED");
    filterEnabledParam = apvts.getRawParameterValue("FILTER_ENABLED");
}

TekkKickAudioProcessor::~TekkKickAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout TekkKickAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // === HAUPT-PARAMETER ===
    // Drive: 1.0 bis 100.0 (erhöht für mehr Distortion)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Distortion Drive", 1.0f, 100.0f, 25.0f));

    // Punch: Pitch Decay (0.0 bis 1.0 -> 0.90 bis 0.9999)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PUNCH", "Punch", 0.0f, 1.0f, 0.5f));

    // Length: Amp Decay (0.0 bis 1.0 -> 0.990 bis 0.99999)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LENGTH", "Length", 0.0f, 1.0f, 0.5f));

    // Base Freq: 30.0 bis 100.0 Hz
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BASE_FREQ", "Sub Freq (Hz)", 30.0f, 100.0f, 50.0f));

    // === CLICK PARAMETER ===
    // Click am Attack: 0.0 bis 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CLICK", "Click Attack", 0.0f, 1.0f, 0.5f));

    // Click am Decay-Ende: 0.0 bis 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CLICK_DECAY", "Click Decay", 0.0f, 1.0f, 0.3f));

    // === STEREO & TEXTURE ===
    // Binaural Swirl: 0.0 bis 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SWIRL", "Binaural Swirl", 0.0f, 1.0f, 0.4f));

    // Stereo-Spread: 0.0 bis 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SPREAD", "Stereo Spread", 0.0f, 1.0f, 0.3f));

    // === DISTORTION ===
    // Wavefolding: 0.0 bis 10.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FOLD", "Wavefold", 0.0f, 10.0f, 3.0f));

    // === LAYER ===
    // Sub-Layer Mix: 0.0 bis 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUB", "Sub Layer", 0.0f, 1.0f, 0.5f));

    // Noise-Layer: 0.0 bis 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("NOISE", "Noise Layer", 0.0f, 1.0f, 0.3f));

    // === FILTER ===
    // Filter Cutoff: 20.0 bis 20000.0 Hz
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff (Hz)", 20.0f, 20000.0f, 20000.0f));

    // Filter Resonance: 0.0 bis 10.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_RES", "Filter Resonance", 0.0f, 10.0f, 0.0f));

    // Filter Type: 0=Lowpass, 1=Highpass, 2=Bandpass
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTER_TYPE", "Filter Type",
        juce::StringArray("Lowpass", "Highpass", "Bandpass"), 0));

    // === PRE-DISTORTION EQ (NEU - für den "Tok" Sound) ===
    // Pre-EQ Frequenz: 800 bis 2000 Hz
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_EQ_FREQ", "Pre-EQ Freq (Hz)", 800.0f, 2000.0f, 1200.0f));

    // Pre-EQ Gain: 0.0 bis 20.0 dB (Boost)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_EQ_GAIN", "Pre-EQ Boost (dB)", 0.0f, 20.0f, 12.0f));

    // Pre-EQ Enable Switch
    params.push_back(std::make_unique<juce::AudioParameterBool>("PRE_EQ_ENABLED", "Pre-EQ Enabled", true));

    // === ON/OFF SWITCHES ===
    params.push_back(std::make_unique<juce::AudioParameterBool>("DRIVE_ENABLED", "Drive Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("PUNCH_ENABLED", "Punch Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("LENGTH_ENABLED", "Length Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("BASE_FREQ_ENABLED", "Base Freq Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("CLICK_ENABLED", "Click Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("CLICK_DECAY_ENABLED", "Click Decay Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SWIRL_ENABLED", "Swirl Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SPREAD_ENABLED", "Spread Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FOLD_ENABLED", "Fold Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SUB_ENABLED", "Sub Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("NOISE_ENABLED", "Noise Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FILTER_ENABLED", "Filter Enabled", true));

    return { params.begin(), params.end() };
}

void TekkKickAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    // 1. Init Oversampling
    oversampler.reset();
    oversampler.initProcessing(static_cast<size_t>(samplesPerBlock));

    double osSampleRate = sampleRate * oversampler.getOversamplingFactor();
    juce::dsp::ProcessSpec osSpec { osSampleRate, static_cast<uint32_t>(samplesPerBlock), 2 };

    // 2. Init Pre-EQ
    preDistortionEQ.prepare(osSpec);
    preDistortionEQ.reset();

    // 3. Init WaveShapers
    wavefolder.prepare(osSpec);
    hardClipper.prepare(osSpec);

    // 4. Init Post-Filters (Stereo)
    for (auto& filter : postFilters)
    {
        filter.prepare(osSpec);
        filter.reset();
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    }
}

void TekkKickAudioProcessor::releaseResources()
{
    // Oversampling zurücksetzen
    oversampler.reset();
}

bool TekkKickAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void TekkKickAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    // === KONSTANTEN FÜR HARDDTEKK KICKS ===
    constexpr float PITCH_START_FREQ = 2000.0f;  // Startfrequenz für Pitch Drop
    constexpr float PITCH_DECAY_MIN_MS = 10.0f;    // Minimaler Pitch Decay
    constexpr float PITCH_DECAY_MAX_MS = 50.0f;    // Maximaler Pitch Decay

    // 1. MIDI Auswerten: Wenn eine Note gespielt wird, starte die Envelopes neu
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        if (message.isNoteOn())
        {
            isPlaying = true;
            phase = 0.0f;
            subPhase = 0.0f;
            envAmpPos = 1.0f;
            envPitchPos = 1.0f;
            sampleCount = 0.0f;
            noisePhase = 0.0f;
        }
    }

    // Aktuelle Parameterwerte abrufen
    float punchValue = punchParam->load();
    float lengthValue = lengthParam->load();
    float baseFreq = baseFreqParam->load();
    float click = clickParam->load();
    float clickDecay = clickDecayParam->load();
    float swirl = swirlParam->load();
    float sub = subParam->load();
    float noise = noiseParam->load();
    float spread = spreadParam->load();

    // On/Off Switch-Werte abrufen
    bool punchEnabled = punchEnabledParam->load() > 0.5f;
    bool lengthEnabled = lengthEnabledParam->load() > 0.5f;
    bool baseFreqEnabled = baseFreqEnabledParam->load() > 0.5f;
    bool clickEnabled = clickEnabledParam->load() > 0.5f;
    bool clickDecayEnabled = clickDecayEnabledParam->load() > 0.5f;
    bool swirlEnabled = swirlEnabledParam->load() > 0.5f;
    bool spreadEnabled = spreadEnabledParam->load() > 0.5f;
    bool subEnabled = subEnabledParam->load() > 0.5f;
    bool noiseEnabled = noiseEnabledParam->load() > 0.5f;

    // === EXPONENTIELLE ENVELOPE KOEFFIZIENTEN BERECHNEN ===
    // Pitch Decay: 10ms bis 50ms für extrem schnellen, punchigen Drop
    float pitchDecayTimeMs = punchEnabled ? (PITCH_DECAY_MIN_MS + (1.0f - punchValue) * (PITCH_DECAY_MAX_MS - PITCH_DECAY_MIN_MS)) : 1.0f;
    pitchDecayCoeff = static_cast<float>(std::exp(-1000.0 / (pitchDecayTimeMs * currentSampleRate)));

    // Amplitude Decay: Mappe 0.0-1.0 auf längere Decay-Zeiten (50ms bis 2000ms)
    float ampDecayTimeMs = lengthEnabled ? (50.0f + lengthValue * 1950.0f) : 1.0f;
    ampDecayCoeff = static_cast<float>(std::exp(-1000.0 / (ampDecayTimeMs * currentSampleRate)));

    // Binaural Swirl berechnen
    float swirlAmount = swirlEnabled ? (swirl * 10.0f) : 0.0f;

    // 2. GENERIERE RAW SYNTH (Sample-by-Sample)
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        if (!isPlaying) continue;

        sampleCount += 1.0f;

        // === EXPONENTIELLES PITCH ENVELOPE ===
        envPitchPos *= pitchDecayCoeff;

        // === CLICK AM ATTACK ===
        float clickAttack = 0.0f;
        if (clickEnabled && sampleCount < 500.0f)
        {
            float clickEnv = std::exp(-sampleCount / 100.0f);
            clickAttack = click * clickEnv * 0.8f;
        }

        // === CLICK AM DECAY-ENDE ===
        float clickDecayValue = 0.0f;
        if (clickDecayEnabled)
        {
            float decayClickPoint = 0.01f + (clickDecay * 0.05f);
            if (envAmpPos < decayClickPoint && envAmpPos > 0.001f)
            {
                clickDecayValue = clickDecay * envAmpPos * 5.0f;
                clickDecayValue *= ((static_cast<int>(sampleCount) % 2 == 0) ? 1.0f : -0.5f);
            }
        }

        // === HAUPTKICK OSZILLATOR ===
        float currentBaseFreq = baseFreqEnabled ? baseFreq : 50.0f;
        float leftFreq = currentBaseFreq + ((PITCH_START_FREQ - currentBaseFreq) * envPitchPos) + swirlAmount;
        [[maybe_unused]] float rightFreq = currentBaseFreq + ((PITCH_START_FREQ - currentBaseFreq) * envPitchPos) - swirlAmount; // Für zukünftige Stereo-Implementierung

        phase += static_cast<float>(leftFreq / currentSampleRate);
        if (phase >= 1.0f) phase -= 1.0f;

        float mainKick = std::sin(phase * juce::MathConstants<float>::twoPi);

        // === SUB-LAYER ===
        float subFreq = baseFreq * 0.5f;
        subPhase += static_cast<float>(subFreq / currentSampleRate);
        if (subPhase >= 1.0f) subPhase -= 1.0f;
        float subLayer = std::sin(subPhase * juce::MathConstants<float>::twoPi);

        // Mix Main Kick + Sub
        float currentSub = subEnabled ? sub : 0.0f;
        mainKick = mainKick * (1.0f - currentSub * 0.5f) + subLayer * currentSub;

        // === NOISE-LAYER ===
        float noiseValue = 0.0f;
        if (noiseEnabled && sampleCount < 1000.0f)
        {
            noiseValue = ((float)((rand() % 2000) - 1000) / 1000.0f);
            float noiseEnv = std::exp(-sampleCount / 200.0f);
            noiseValue = noiseValue * noise * noiseEnv * 0.3f;
        }

        // === EXPONENTIELLES AMP ENVELOPE ===
        envAmpPos *= ampDecayCoeff;

        // Basis-Signal
        float combined = mainKick + clickAttack + clickDecayValue + noiseValue;

        // === BINAURAL SWIRL ===
        float leftSample, rightSample;
        float currentSwirl = swirlEnabled ? swirl : 0.0f;
        if (currentSwirl > 0.0f)
        {
            float leftSwirl = std::sin((phase + spread * 0.1f) * juce::MathConstants<float>::twoPi);
            float rightSwirl = std::sin((phase - spread * 0.1f) * juce::MathConstants<float>::twoPi);
            leftSample = combined * 0.7f + leftSwirl * 0.3f * currentSwirl;
            rightSample = combined * 0.7f + rightSwirl * 0.3f * currentSwirl;
        }
        else
        {
            leftSample = combined;
            rightSample = combined;
        }

        // === STEREO SPREAD + AMP ENVELOPE ===
        float currentSpread = spreadEnabled ? spread : 0.0f;
        leftSample = leftSample * envAmpPos * (1.0f - currentSpread * 0.3f);
        rightSample = rightSample * envAmpPos * (1.0f + currentSpread * 0.3f);

        // Output in Buffer schreiben (CLEANES SYNTH - KEINE DISTORTION HIER!)
        buffer.setSample(0, sample, leftSample);
        if (buffer.getNumChannels() > 1)
            buffer.setSample(1, sample, rightSample);

        // Stoppe Berechnung, wenn Sound praktisch unhörbar ist
        if (envAmpPos < 0.0001f) isPlaying = false;
    }

    if (!isPlaying) return;

    // 3. BLOCK-BASED DSP EFFECTS (OVERSAMPLED)
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::AudioBlock<float> osBlock = oversampler.processSamplesUp(block);
    juce::dsp::ProcessContextReplacing<float> context(osBlock);

    double osSampleRate = currentSampleRate * oversampler.getOversamplingFactor();

    // A. Apply Pre-EQ ("Tok" Sound)
    if (preEqEnabledParam->load() > 0.5f)
    {
        *preDistortionEQ.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            osSampleRate, preEqFreqParam->load(), 2.5f, juce::Decibels::decibelsToGain(preEqGainParam->load()));
        preDistortionEQ.process(context);
    }

    // B. Apply Drive (Gain)
    if (driveEnabledParam->load() > 0.5f)
    {
        osBlock.multiplyBy(driveParam->load());
    }

    // C. Apply Wavefolder
    if (foldEnabledParam->load() > 0.5f)
    {
        auto& wf = wavefolder;
        wf.functionToUse.foldAmount = foldParam->load();
        wavefolder.process(context);
    }

    // D. Apply Hard Clipper
    hardClipper.process(context);

    // E. Apply Post-Filter
    if (filterEnabledParam->load() > 0.5f)
    {
        float cut = filterCutoffParam->load();
        float res = filterResParam->load();
        int type = static_cast<int>(filterTypeParam->load());

        for (size_t ch = 0; ch < osBlock.getNumChannels(); ++ch)
        {
            postFilters[ch].setCutoffFrequency(cut);
            postFilters[ch].setResonance(res > 0.0f ? res : 0.707f);

            if (type == 0)
                postFilters[ch].setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            else if (type == 1)
                postFilters[ch].setType(juce::dsp::StateVariableTPTFilterType::highpass);
            else
                postFilters[ch].setType(juce::dsp::StateVariableTPTFilterType::bandpass);

            auto* channelData = osBlock.getChannelPointer(ch);
            for (size_t i = 0; i < osBlock.getNumSamples(); ++i)
            {
                channelData[i] = postFilters[ch].processSample(static_cast<int>(ch), channelData[i]);
            }
        }
    }

    // 4. DOWNSAMPLE
    oversampler.processSamplesDown(block);
}

void TekkKickAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TekkKickAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

// ==============================================================================
// WAV EXPORT FUNKTION
// ==============================================================================
#include <juce_audio_formats/juce_audio_formats.h> // Wichtig für WAV Export

void TekkKickAudioProcessor::exportKickToWav(const juce::File& outputFile)
{
    // Pausiere die Audio-Ausgabe der DAW für einen Bruchteil einer Sekunde,
    // damit sich Offline-Render und Echtzeit-Wiedergabe nicht in die Quere kommen.
    suspendProcessing(true);

    double exportSampleRate = 44100.0;
    double durationSeconds = 2.0; // 2 Sekunden reichen locker für eine Kick
    int numSamples = static_cast<int>(exportSampleRate * durationSeconds);
    int numChannels = 2; // Stereo

    // Buffer für den fertigen Sound
    juce::AudioBuffer<float> exportBuffer(numChannels, numSamples);
    exportBuffer.clear();

    // Setup für Offline Rendering
    double oldSampleRate = currentSampleRate;
    prepareToPlay(exportSampleRate, 512);

    // Simuliere einen MIDI "Note On" Befehl, um die Envelopes zu starten
    isPlaying = true;
    phase = 0.0f;
    subPhase = 0.0f;
    envAmpPos = 1.0f;
    envPitchPos = 1.0f;
    sampleCount = 0.0f;
    noisePhase = 0.0f;

    // Render den Sound in kleinen Blöcken (wie es die DAW auch tun würde)
    int exportBlockSize = 512;
    juce::AudioBuffer<float> tempBuffer(numChannels, exportBlockSize);
    juce::MidiBuffer dummyMidi;

    for (int startSample = 0; startSample < numSamples; startSample += exportBlockSize)
    {
        int numSamplesToProcess = juce::jmin(exportBlockSize, numSamples - startSample);
        tempBuffer.setSize(numChannels, numSamplesToProcess, false, false, true);
        tempBuffer.clear();

        // Audio-Engine den Block berechnen lassen
        processBlock(tempBuffer, dummyMidi);

        // Den berechneten Block in den großen Export-Buffer kopieren
        for (int ch = 0; ch < numChannels; ++ch)
        {
            exportBuffer.copyFrom(ch, startSample, tempBuffer, ch, 0, numSamplesToProcess);
        }
    }

    // Altes Setup der DAW wiederherstellen
    prepareToPlay(oldSampleRate, 512);
    suspendProcessing(false);

    // === IN DATEI SCHREIBEN ===
    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::FileOutputStream> outStream(outputFile.createOutputStream());

    if (outStream != nullptr)
    {
        // Verwende die vorhandene API (deprecated in JUCE 8 aber noch funktional)
        std::unique_ptr<juce::AudioFormatWriter> writer(
            wavFormat.createWriterFor(outStream.get(), exportSampleRate, numChannels, 24, {}, 0));

        if (writer != nullptr)
        {
            outStream.release(); // Writer übernimmt die Kontrolle über den Stream
            writer->writeFromAudioSampleBuffer(exportBuffer, 0, numSamples);
        }
    }
}

juce::AudioProcessorEditor* TekkKickAudioProcessor::createEditor()
{
    return new TekkKickAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TekkKickAudioProcessor();
}
