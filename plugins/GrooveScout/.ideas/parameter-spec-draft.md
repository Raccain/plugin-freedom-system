# GrooveScout - Draft Parameter Specification

**Status:** Draft (for Stage 0 DSP research)
**Created:** 2026-02-20
**Plugin Type:** Analysis Utility

---

## Parameter List

### Kick Detection

| Parameter ID | Type | Min | Max | Default | Units | Description |
|-------------|------|-----|-----|---------|-------|-------------|
| `kickSensitivity` | Float | 0.0 | 1.0 | 0.5 | - | Onset detection threshold for kick drum. Lower = more sensitive (detects quieter hits). |
| `kickFreqLow` | Float | 20.0 | 500.0 | 40.0 | Hz | Low frequency cutoff of kick detection band. |
| `kickFreqHigh` | Float | 50.0 | 1000.0 | 120.0 | Hz | High frequency cutoff of kick detection band. |

### Snare Detection

| Parameter ID | Type | Min | Max | Default | Units | Description |
|-------------|------|-----|-----|---------|-------|-------------|
| `snareSensitivity` | Float | 0.0 | 1.0 | 0.5 | - | Onset detection threshold for snare drum. Lower = more sensitive. |
| `snareFreqLow` | Float | 100.0 | 2000.0 | 200.0 | Hz | Low frequency cutoff of snare detection band. |
| `snareFreqHigh` | Float | 1000.0 | 20000.0 | 8000.0 | Hz | High frequency cutoff of snare detection band. |

### Hihat Detection

| Parameter ID | Type | Min | Max | Default | Units | Description |
|-------------|------|-----|-----|---------|-------|-------------|
| `hihatSensitivity` | Float | 0.0 | 1.0 | 0.5 | - | Onset detection threshold for hihat. Lower = more sensitive. |
| `hihatFreqLow` | Float | 1000.0 | 20000.0 | 5000.0 | Hz | Low frequency cutoff of hihat detection band. |
| `hihatFreqHigh` | Float | 5000.0 | 20000.0 | 16000.0 | Hz | High frequency cutoff of hihat detection band. |

### BPM Control

| Parameter ID | Type | Options | Default | Description |
|-------------|------|---------|---------|-------------|
| `bpmMultiplier` | Choice | "½x", "1x", "2x" | "1x" | Multiplier applied to detected BPM. Use ½x when plugin detects double-time, 2x when it detects half-time. |

---

## Non-Parameter Controls (UI Actions / State)

These are UI interactions and plugin state — not APVTS parameters:

| Control | Type | Description |
|---------|------|-------------|
| Analyze button | Trigger | Initiates one-shot analysis of current audio buffer |
| BPM display | Clickable output | Shows detected BPM × multiplier; click to set DAW project tempo |
| Key display | Read-only label | Shows detected key (e.g. "F# minor", "A major") |
| Root chord MIDI | Draggable clip | Root chord triad of detected key (e.g. F#-A-C# for F# minor) |
| Kick MIDI | Draggable clip | Extracted kick groove pattern with timing + velocity |
| Snare MIDI | Draggable clip | Extracted snare groove pattern with timing + velocity |
| Hihat MIDI | Draggable clip | Extracted hihat groove pattern with timing + velocity |

---

## Analysis Pipeline (DSP Research Context)

1. **Audio capture** — Record audio buffer when Analyze is triggered
2. **BPM detection** — Onset detection + autocorrelation / beat tracking
3. **Key detection** — Chroma feature extraction → Krumhansl-Schmuckler key-finding
4. **Drum separation** — Three parallel bandpass filters (kick/snare/hihat frequency bands)
5. **Per-band onset detection** — Detect hits within each band, capture timing + velocity
6. **MIDI assembly** — Convert onset list to MIDI note events (pitch = GM drum note, velocity = onset strength)
7. **Output** — BPM value, key string, three MIDI patterns, root chord MIDI

---

## Notes for Stage 0 Research

- **Key challenge:** Drum separation on full mixes (band filtering alone may have cross-contamination)
- **Key challenge:** MIDI drag-and-drop from JUCE plugin UI (requires ExternalDragAndDropContainer)
- **Key challenge:** Accessing the full audio buffer of the DAW track (plugin only receives real-time blocks — may need to record buffer during playback before analyzing)
- **Frequency constraints:** kickFreqLow must always be < kickFreqHigh (same for snare, hihat). Validation needed.
- **BPM multiplier** applies after detection: displayedBPM = detectedBPM × multiplier (½, 1, or 2)
