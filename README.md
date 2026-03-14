# TekkKick VST - Hardtekk Kick Drum Plugin

Ein aggressiver Synthesizer-Kick-Generator für Hardtekk-Stile.

## Abhängigkeiten

### JUCE Framework

Dieses Projekt benötigt **JUCE 8.0.1.2**.

1. Lade JUCE 8.0.1.2 von https://github.com/juce-framework/JUCE/releases herunter
2. Entpacke es in den Projektordner als `JUCE 8.0.1.2/`

Die Ordnerstruktur sollte so aussehen:
```
HardTekk VST/
├── CMakeLists.txt
├── PluginProcessor.h
├── PluginProcessor.cpp
├── PluginEditor.h
├── PluginEditor.cpp
└── JUCE 8.0.1.2/    <-- JUCE Framework hierher entpacken
```

## Build (Windows)

```bash
# CMake Projekt generieren
cmake -B build -S .

# Plugin kompilieren (VST3)
cmake --build build --config Release

# Das VST3 wird erstellt unter:
# build/TekkKick_artefacts/Release/VST3/TekkKick.vst3/
```

Das Plugin wird automatisch nach `C:\Program Files\Common Files\VST3\` kopiert.

## Parameter

### Main
- **Drive**: Distortion Gain
- **Punch**: Pitch Decay
- **Length**: Amp Decay
- **Base Freq**: Sub Frequency (Hz)

### Click
- **Click Attack**: Transient Click
- **Click Decay**: Click am Decay-Ende
- **Binaural Swirl**: Psychoakustische Tiefe
- **Stereo Spread**: Stereo Breite

### Distortion
- **Wavefold**: Wavefolding Distortion

### Layer
- **Sub Layer**: Sub-OSC Layer
- **Noise Layer**: Noise-Transient Layer

### Filter
- **Filter Cutoff**: Filter Frequenz
- **Filter Resonance**: Filter Resonanz
- **Filter Type**: Lowpass / Highpass / Bandpass

### On/Off Switches
Jeder Parameter hat einen eigenen Toggle-Switch, um ihn ein- oder auszuschalten.
Alle Switches sind standardmäßig eingeschaltet.
