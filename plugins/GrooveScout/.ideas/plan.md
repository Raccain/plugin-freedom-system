# GrooveScout - Implementation Plan

**Date:** 2026-02-20
**Complexity Score:** 5.0 (capped) — Complex
**Strategy:** Phase-based implementation

---

## Complexity Factors

- **Parameters:** 15 parameters (15 / 5 = 3.0, capped at 2.0) = **2.0**
  - kickSensitivity, kickFreqLow, kickFreqHigh
  - snareSensitivity, snareFreqLow, snareFreqHigh
  - hihatSensitivity, hihatFreqLow, hihatFreqHigh
  - bpmMultiplier (Choice)
  - analyzeBPM, analyzeKey, analyzeKick, analyzeSnare, analyzeHihat (5 Bool)

- **Algorithms:** 5 distinct analysis components = **5**
  - Audio Buffer Recorder (offline accumulation)
  - BPM Detection Engine (OSS + generalized autocorrelation)
  - Key Detection Engine (STFT chromagram + Krumhansl-Schmuckler)
  - Band-Separated Onset Detector (3 parallel bandpass + onset detection)
  - MIDI Pattern Assembler (MidiFile + MidiMessageSequence)

- **Features (complexity modifiers):**
  - Background thread analysis with cancellation (+1) — `juce::Thread`, `threadShouldExit()`
  - FFT/frequency domain processing (+1) — `juce::dsp::FFT` for BPM autocorrelation + chroma
  - Offline analysis trigger (non-standard plugin pattern) — not a standard modifier, but significantly adds complexity

- **Total (before cap):** 2.0 + 5 + 2 = **9.0 → capped at 5.0**

**Complexity Tier:** 6 (Real-time analysis + FFT + file I/O + background threading)

---

## Stages

- Stage 0: Research & Planning — COMPLETE (this document)
- Stage 1: Foundation (project structure)
- Stage 1: Shell (APVTS parameters)
- Stage 3: DSP — 4 phases
- Stage 3: GUI — 3 phases
- Stage 3: Validation

---

## Stage 3: DSP Phases

### Phase DSP.1: Audio Passthrough + Buffer Capture Infrastructure

**Goal:** Plugin loads in DAW, audio passes through unmodified, and the recording buffer accumulator works correctly.

**Components:**
- Stereo in → stereo out passthrough (`processBlock()` copies no data — audio is already in the output buffer)
- Pre-allocated `juce::AudioBuffer<float>` recording buffer in `prepareToPlay()` (max 30s × sampleRate × 2 channels)
- `std::atomic<bool>` flags: `isCapturing`, `analyzeTriggered`, `analysisComplete`, `analysisCancelled`
- `std::atomic<int>` counters: `recordedSamples`, `analysisProgress`
- Audio thread: on each `processBlock()`, if `isCapturing`, append stereo samples to recording buffer
- `PluginProcessor::analyzeButtonPressed()` method (called from UI thread) that sets `analyzeTriggered`, stops capture, starts background thread
- Stub `GrooveScoutAnalyzer` thread class (just logs "analysis started/stopped")
- `juce::Timer` in processor/editor that polls `analysisComplete`

**Test Criteria:**
- [ ] Plugin loads in DAW without crashes
- [ ] Audio passes through unmodified (verify with sine tone — output = input)
- [ ] Recording buffer allocates correctly at `prepareToPlay()` (no crash on init)
- [ ] Sample rate change triggers buffer reallocation without crash
- [ ] `isCapturing` flag controls whether samples are appended
- [ ] `recordedSamples` counter increments during capture
- [ ] Analyze button triggers thread start (verify with log output)
- [ ] Cancel button triggers thread stop (verify with log output)
- [ ] No memory leaks or crashes on plugin close while thread is running

---

### Phase DSP.2: BPM Detection

**Goal:** Detect BPM from the accumulated recording buffer.

**Components:**
- Stereo-to-mono mix of recording buffer
- Onset Strength Signal (OSS) computation: 2048-sample frames, 512-sample hop, RMS energy delta with half-wave rectification
- Hann window function using `juce::dsp::WindowingFunction<float>`
- Generalized autocorrelation via `juce::dsp::FFT`: `GAC = IFFT(|FFT(OSS^0.5)|^2)`, FFT order 11 (2048-point)
- Lag-to-BPM conversion restricted to 60–200 BPM range (300ms–1000ms lags)
- Peak-picking on GAC with pulse-train cross-correlation scoring
- `bpmMultiplier` applied post-detection (0.5x / 1x / 2x)
- Thread cancellation checks between each sub-step
- Result stored in thread-local `float detectedBPM` then atomically published

**Test Criteria:**
- [ ] BPM detected within ±2 BPM for 120 BPM drum loop
- [ ] BPM detected within ±2 BPM for 90 BPM loop
- [ ] bpmMultiplier 2x doubles the displayed BPM
- [ ] bpmMultiplier 0.5x halves the displayed BPM
- [ ] Cancel during BPM detection stops cleanly without crash
- [ ] Thread exits cleanly when `threadShouldExit()` returns true
- [ ] BPM of 0 returned for silent audio (graceful no-result)

---

### Phase DSP.3: Key Detection

**Goal:** Detect musical key from accumulated recording buffer.

**Components:**
- STFT: 4096-point FFT (`juce::dsp::FFT` order 12), 50% overlap (2048-sample hop), Hann window
- Magnitude spectrum computation from complex FFT output
- Pitch class profile (PCP / chromagram) accumulation: map FFT bins to 12 pitch classes
  - `freq = k * sampleRate / 4096`, `midiPitch = 12 * log2(freq / 440.0) + 69`, `pitchClass = round(midiPitch) % 12`
- Normalize accumulated PCP to unit length
- Krumhansl-Schmuckler correlation against 24 key profiles (hardcoded constants)
- Key index → key string (e.g., "F# minor")
- Root chord MIDI derivation: major (root, +4, +7) or minor (root, +3, +7) triad
- Thread cancellation checks at STFT completion and post-correlation

**Test Criteria:**
- [ ] "A minor" detected for an A minor scale recording (verify manually)
- [ ] "C major" detected for a C major scale recording
- [ ] Key string formatted correctly ("F# minor", not "F#minor" or "Fsharp minor")
- [ ] Root chord MIDI contains correct notes for detected key
- [ ] Cancel during key detection stops cleanly without crash
- [ ] Silent audio returns "Unknown" gracefully (no crash)

---

### Phase DSP.4: Drum Onset Detection + MIDI Assembly

**Goal:** Detect kick, snare, hihat onset events from band-filtered audio and assemble MIDI files.

**Components:**
- Per-band IIR bandpass filter chain using `juce::dsp::IIR::Filter<float>`:
  - HP filter: `juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freqLow, 0.707)`
  - LP filter: `juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freqHigh, 0.707)`
  - Applied in series to the entire recording buffer (offline)
- Frequency constraint validation: `freqLow < freqHigh` check before each band
- Per-band onset detection: 256-sample windows, 128-sample hop
  - RMS energy delta (half-wave rectified)
  - Adaptive threshold: `mean + (1 - sensitivity) * 4 * stdDev` over 40-frame window
  - Minimum inter-onset interval: 80ms
- Onset timing → beat position conversion using `detectedBPM`
- Onset strength → MIDI velocity (1–127)
- MIDI Pattern Assembler:
  - `juce::MidiFile` (format 0, 480 PPQ)
  - `juce::MidiMessageSequence` per drum
  - Tempo meta-event at tick 0
  - Note-on/note-off pairs (GM drum notes: kick=36, snare=38, hihat=42)
  - Write to `juce::File::getSpecialLocation(juce::File::tempDirectory) / "GrooveScout/" / "groovescout_kick.mid"` etc.
  - Root chord MIDI file: 3-note chord, duration 4 beats
- Set `analysisComplete` atomic after files written
- Thread cancellation checks between filter passes and after each band

**Test Criteria:**
- [ ] Kick MIDI file written to temp directory after analysis
- [ ] Snare MIDI file written to temp directory after analysis
- [ ] Hihat MIDI file written to temp directory after analysis
- [ ] Root chord MIDI file written with correct notes for detected key
- [ ] Kick detected on beat 1 and 3 for a standard 4/4 beat (verify timing)
- [ ] Snare detected on beat 2 and 4 for a standard 4/4 beat
- [ ] Hihat detected on every 8th note for a standard 8th-note hat pattern
- [ ] Setting `analyzeKick = false` skips kick detection and leaves no kick MIDI
- [ ] Setting `kickFreqLow > kickFreqHigh` skips kick band gracefully
- [ ] MIDI velocity varies with onset strength (loud hit → higher velocity)
- [ ] Cancel during drum detection stops cleanly without crash
- [ ] Analysis runs cleanly for both short (4-bar) and long (32-bar) recordings

---

## Stage 3: GUI Phases

### Phase GUI.1: Layout and Basic Controls

**Goal:** Render the plugin UI with all parameter controls visible and laid out correctly.

**Components:**
- HTML/CSS mockup integrated as WebView (or native JUCE components if WebView not used)
- Parameter controls for all 9 frequency/sensitivity sliders (kick/snare/hihat bands)
- bpmMultiplier dropdown (3-way choice)
- 5 analysis toggle checkboxes (analyzeBPM, analyzeKey, analyzeKick, analyzeSnare, analyzeHihat)
- Analyze button (non-parameter trigger)
- Cancel button (non-parameter trigger, hidden when not analyzing)
- BPM display area (read-only text, shows "--" until analyzed)
- Key display area (read-only text, shows "--" until analyzed)
- MIDI clip drag areas: Root Chord, Kick, Snare, Hihat (shown grayed out until analyzed)
- Progress indicator (shown during analysis)

**Test Criteria:**
- [ ] Plugin editor opens without crash
- [ ] All parameter sliders visible and styled correctly
- [ ] bpmMultiplier dropdown shows correct options
- [ ] Analysis toggles visible with correct default state (all enabled)
- [ ] Analyze button visible and clickable
- [ ] Cancel button hidden at rest
- [ ] BPM display shows "--" on first open
- [ ] Key display shows "--" on first open
- [ ] MIDI clip areas visible but grayed out

---

### Phase GUI.2: Parameter Binding and Analysis Flow

**Goal:** Two-way parameter binding for all controls, Analyze/Cancel button wiring, and analysis state display.

**Components:**
- APVTS parameter bindings for all sliders, toggles, bpmMultiplier
- Analyze button → calls `analyzeButtonPressed()` on processor → starts background thread
- Cancel button → calls `cancelAnalysis()` on processor → `thread.stopThread()`
- Cancel button: show only during analysis (`analysisComplete == false && threadIsRunning`)
- Progress display updates from `analysisProgress` atomic (via Timer polling)
- BPM display updates after analysis (Timer polls `analysisComplete` flag)
- Key display updates after analysis
- MIDI clip areas change from "grayed out" to "draggable" state after analysis

**Test Criteria:**
- [ ] Kick sensitivity slider changes value and is persisted
- [ ] kickFreqLow / kickFreqHigh sliders work correctly
- [ ] bpmMultiplier dropdown changes value and is persisted
- [ ] analyzeKick toggle enables/disables kick detection
- [ ] Analyze button starts analysis (progress indicator appears)
- [ ] Cancel button stops analysis mid-run (progress indicator disappears)
- [ ] BPM number updates in UI after analysis completes
- [ ] Key string updates in UI after analysis completes
- [ ] MIDI clip areas become draggable after analysis
- [ ] Analysis toggle controls properly gate sub-analyses
- [ ] Progress indicator counts up during analysis

---

### Phase GUI.3: MIDI Drag-and-Drop Interaction

**Goal:** Users can drag MIDI clip areas to DAW timeline to import MIDI patterns.

**Components:**
- `mouseDrag()` override on each MIDI clip component
- Drag guard: only activate drag if MIDI file exists on disk (clip is in "available" state)
- Minimum drag distance before `performExternalDragDropOfFiles()` called (avoid accidental triggers)
- `performExternalDragDropOfFiles(juce::StringArray{midiFilePath}, true)` call
- Visual feedback: clip area highlights on hover when draggable
- Post-drag behavior: clip remains available (file not deleted, user can drag multiple times)
- BPM display "click to set tempo" (if host supports it): `getPlayHead()->updatePosition()` — implement as best-effort

**Test Criteria:**
- [ ] Dragging Kick MIDI clip to Ableton Live MIDI track creates MIDI clip with kick pattern
- [ ] Dragging Snare MIDI clip to Ableton Live MIDI track creates MIDI clip
- [ ] Dragging Hihat MIDI clip works correctly
- [ ] Dragging Root Chord MIDI clip works correctly
- [ ] Clip drag does nothing if analysis has not run yet
- [ ] Clip can be dragged multiple times (file persists)
- [ ] Drag works in at least: Ableton Live, Logic Pro (macOS), Reaper
- [ ] Visual hover state appears correctly on draggable clips
- [ ] BPM display click sets project tempo (at least in Ableton/Logic if supported)

---

## Implementation Flow

- Stage 1: Foundation - project structure (CMakeLists.txt, PluginProcessor.h/cpp, PluginEditor.h/cpp)
- Stage 1: Shell - APVTS parameters (all 15 parameters registered, default values)
- Stage 3: DSP - 4 phases
  - Phase DSP.1: Audio passthrough + buffer capture infrastructure
  - Phase DSP.2: BPM detection (OSS + autocorrelation)
  - Phase DSP.3: Key detection (chromagram + Krumhansl-Schmuckler)
  - Phase DSP.4: Drum onset detection + MIDI assembly
- Stage 3: GUI - 3 phases
  - Phase GUI.1: Layout and basic controls
  - Phase GUI.2: Parameter binding and analysis flow
  - Phase GUI.3: MIDI drag-and-drop interaction
- Stage 3: Validation - pluginval, preset testing, changelog

---

## Implementation Notes

### Thread Safety
- All audio→background communication via `std::atomic<>` — no mutexes in `processBlock()`
- Recording buffer: exclusively written by audio thread during capture, exclusively read by background thread after capture stops — no concurrent access window
- Analysis results (BPM, key string, file paths): set by background thread, read by message thread only after `analysisComplete` flag is true
- Background thread: check `threadShouldExit()` after EVERY major analysis step (not just in inner loops)
- Plugin destructor: always call `analyzerThread.stopThread(2000)` before `PluginProcessor` destruction

### Performance
- Audio passthrough: ~0% CPU (no processing, just sample copy to recording buffer)
- Recording buffer copy: O(blockSize) per block — negligible
- Analysis (background): 1–5 seconds for 30s recording, not time-critical
- FFT (background): `juce::dsp::FFT` is not SIMD-optimized for all platforms, but offline timing is irrelevant

### Latency
- Audio passthrough: 0 samples latency (`getLatencySamples()` returns 0)
- Analysis: not reported as plugin latency (it is offline user-triggered)

### Denormal Protection
- `juce::ScopedNoDenormals` in `processBlock()` for any real-time code paths
- IIR filter state reset (`filter.reset()`) before each offline processing pass to avoid denormal accumulation in filter state

### Known Challenges

1. **Audio buffer recording without host API access:** Plugin only receives real-time `processBlock()` blocks — it cannot request the full track audio. The user MUST play back the track while the plugin is recording. This is a fundamental limitation that must be clearly communicated in the UI ("Play your track first, then press Analyze").

2. **BPM accuracy on full mixes:** Full mixes have complex transient profiles. Drum loops = high accuracy (±1 BPM). Full mixes with melody = moderate accuracy (±3 BPM). Ambient music without clear beat = poor accuracy. Document this limitation.

3. **Key detection on full mixes:** Key detection accuracy degrades when multiple instruments are present and competing. Works best on piano, guitar, or melodic solo recordings. Drum-heavy sections may confuse the chromagram. Recommend analyzing a melodic section rather than a drum-only section.

4. **MIDI drag-and-drop across plugin boundary:** The plugin editor is embedded in the DAW's plugin window. Mouse drag events initiated inside the plugin window must escape to the DAW timeline. This works via `performExternalDragDropOfFiles()` on Windows and macOS, but behavior varies by DAW. Test extensively.

5. **Frequency constraint (freqLow < freqHigh):** Parameters are stored independently in APVTS. There is no built-in JUCE mechanism to enforce relative constraints between parameters. Validation must happen at analysis time, not in the UI. Consider adding UI warnings if the constraint is violated.

6. **"Click BPM to set project tempo":** This requires the host to support tempo setting via `AudioPlayHead`. Support varies: Logic Pro and Ableton support this in recent versions, others may not. Implement with feature detection and graceful fallback (log "Host does not support tempo change" and show a tooltip).

7. **MiniBPM as fallback:** If the custom BPM detection proves inaccurate or takes too long to implement, MiniBPM (https://github.com/breakfastquay/minibpm) is a single-file drop-in. It is GPL-3, so check licensing compatibility before embedding.

---

## References

- Creative brief: `plugins/GrooveScout/.ideas/creative-brief.md`
- Parameter spec: `plugins/GrooveScout/.ideas/parameter-spec-draft.md`
- DSP architecture: `plugins/GrooveScout/.ideas/architecture.md`

Similar plugins for reference patterns:
- DrumRoulette — background thread for sample loading, lock-free audio communication pattern
- Scatter — `juce::AudioBuffer<float>` management patterns
- FlutterVerb — `juce::dsp::IIR::Filter` usage pattern (offline filter application)
