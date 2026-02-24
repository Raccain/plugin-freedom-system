# GrooveScout - Implementation Plan

**Date:** 2026-02-20
**Complexity Score:** 5.0 (capped) — Complex
**Strategy:** Phase-based implementation

---

## Complexity Factors

- **Parameters:** 10 parameters (10 / 5 = 2.0, capped at 2.0) = **2.0**
  - captureDuration (Float)
  - kickSensitivity, kickFreqLow, kickFreqHigh
  - snareSensitivity, snareFreqLow, snareFreqHigh
  - hihatSensitivity, hihatFreqLow, hihatFreqHigh

- **Algorithms:** 5 distinct analysis components = **5**
  - Audio Buffer Recorder (offline accumulation)
  - BPM Detection Engine (OSS + generalized autocorrelation)
  - Key Detection Engine (STFT chromagram + Krumhansl-Schmuckler)
  - Band-Separated Onset Detector (3 parallel bandpass + onset detection)
  - MIDI Pattern Assembler (MidiFile + MidiMessageSequence)

- **Features (complexity modifiers):**
  - Background thread analysis with partial-result STOP (+1) — `juce::Thread`, `threadShouldExit()`
  - FFT/frequency domain processing (+1) — `juce::dsp::FFT` for BPM autocorrelation + chroma
  - Explicit REC/STOP recording model with preview mode (+1) — non-standard plugin pattern, real-time buffer playback with live IIR filtering
  - Waveform JS bridge (+0.5) — 200ms Timer push of RMS data to WebView

- **Total (before cap):** 2.0 + 5 + 3.5 = **10.5 → capped at 5.0**

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

### Phase DSP.1: Audio Passthrough + Buffer Capture + Preview Infrastructure

**Goal:** Plugin loads in DAW, audio passes through unmodified, explicit REC/STOP recording works, preview mode plays back filtered buffer, and waveform data is pushed to JS.

**Components:**
- Stereo in → stereo out passthrough (default mode — audio is already in output buffer, no copy needed)
- Pre-allocated `juce::AudioBuffer<float>` recording buffer in `prepareToPlay()` (max 30s × sampleRate × 2 channels)
- `std::atomic<bool>` flags: `isCapturing`, `recordingComplete`, `isPreviewActive`, `analysisComplete`
- `std::atomic<int>` counters: `recordedSamples`, `analysisProgress`, `analysisStep`, `previewPlayhead`, `previewBand`
- `PluginProcessor::startRecording()` — resets `recordedSamples = 0`, sets `isCapturing = true`
- `PluginProcessor::stopCurrentOperation()` — clears `isCapturing` OR stops analysis thread; buffer always preserved
- `PluginProcessor::startAnalysis()` — starts `GrooveScoutAnalyzer` thread (only if `recordedSamples > 0`)
- `PluginProcessor::togglePreview(band)` — sets `previewBand`, `isPreviewActive`, resets `previewPlayhead`
- Audio thread `processBlock()` logic:
  - Preview mode: read buffer at `previewPlayhead`, apply live IIR filter for active band, write to output, advance + wrap playhead
  - Capture mode: append stereo samples, check auto-stop vs `captureDuration`, update `waveformRms` accumulator
  - Default: passthrough
- Stub `GrooveScoutAnalyzer` thread class (just logs "analysis started/stopped")
- `juce::Timer` at 60ms: polls analysis state, pushes updates to JS via `evaluateJavascript()`
- `juce::Timer` at 200ms: serializes `waveformRms` → `evaluateJavascript("updateWaveform([...])")`
- `captureDuration` APVTS parameter wired to auto-stop check

**Test Criteria:**
- [ ] Plugin loads in DAW without crashes
- [ ] Default mode: audio passes through unmodified (verify with sine tone — output = input)
- [ ] Recording buffer allocates correctly at `prepareToPlay()` (no crash on init)
- [ ] Sample rate change triggers buffer reallocation without crash
- [ ] REC press: `isCapturing = true`, `recordedSamples` increments during playback
- [ ] Auto-stop: recording stops automatically when `recordedSamples >= captureDuration * sampleRate`
- [ ] STOP press during recording: `isCapturing = false`, buffer contains partial recording
- [ ] REC press again: clears buffer, starts fresh recording
- [ ] Analyze press: starts stub thread (verify with log output)
- [ ] STOP press during analysis: thread stops cleanly (verify with log output)
- [ ] Preview mode: output switches from passthrough to filtered buffer playback
- [ ] Preview loops: playhead wraps at end of recorded samples
- [ ] Preview cancelled by REC, STOP, or Analyze
- [ ] Waveform RMS data pushed to JS every 200ms (verify via JS console log)
- [ ] No memory leaks or crashes on plugin close while thread is running or preview is active

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
- [ ] STOP during BPM detection stops cleanly without crash; BPM shows `?` in UI
- [ ] Thread exits cleanly when `threadShouldExit()` returns true
- [ ] BPM of 0 / `?` returned for silent audio (graceful no-result, no crash)

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
- [ ] STOP during key detection stops cleanly; key shows `?`, BPM result (if completed) still shows
- [ ] Silent audio returns `?` gracefully (no crash)

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
- [ ] Setting `kickFreqLow > kickFreqHigh` skips kick band gracefully (no crash, no kick MIDI)
- [ ] MIDI velocity varies with onset strength (loud hit → higher velocity)
- [ ] Cancel during drum detection stops cleanly without crash
- [ ] Analysis runs cleanly for both short (4-bar) and long (32-bar) recordings

---

## Stage 3: GUI Phases

### Phase GUI.1: Layout and Basic Controls

**Goal:** Render the plugin UI with all controls visible and laid out correctly per the v3 mockup.

**Components:**
- HTML/CSS v3 mockup integrated as WebView
- Record controls row: REC button, STOP button, `captureDuration` knob (1–30s), `≈ X bars` label (JS-computed)
- Waveform display: canvas element, filled left-to-right via `updateWaveform()` JS function
- Three drum cards (Kick, Snare, Hihat), each with: dual-handle frequency range slider, sensitivity slider, Preview toggle button
- Analyze button (disabled until buffer has content)
- STOP button (visible only when recording or analyzing)
- Progress bar + step label (hidden when idle)
- BPM display (shows `--` until analyzed, `?` on failure)
- Key display (shows `--` until analyzed, `?` on failure)
- MIDI clip drag areas: Root Chord, Kick, Snare, Hihat (grayed out until analysis complete)

**Test Criteria:**
- [ ] Plugin editor opens without crash
- [ ] All drum card sliders visible and styled correctly
- [ ] REC button visible and clickable
- [ ] captureDuration knob shows correct value (default 15s)
- [ ] `≈ X bars` label hidden until BPM is detected
- [ ] Waveform canvas visible (empty on first open)
- [ ] Analyze button disabled on first open (no buffer yet)
- [ ] STOP button hidden at rest
- [ ] BPM display shows `--` on first open
- [ ] Key display shows `--` on first open
- [ ] MIDI clip areas visible but grayed out

---

### Phase GUI.2: Parameter Binding, Recording Flow, and Analysis State

**Goal:** Full parameter binding, REC/STOP/Analyze wiring, waveform updates, and analysis state display.

**Components:**
- APVTS bindings for all 10 sliders/knobs via native function bridge
- REC button → `getNativeFunction("startRecording")()` → `startRecording()`
- STOP button → `getNativeFunction("stopCurrentOperation")()` → `stopCurrentOperation()`
- STOP button: visible only when recording or analyzing
- Analyze button → `getNativeFunction("startAnalysis")()` → `startAnalysis()`; disabled when no buffer content
- Preview buttons → `getNativeFunction("togglePreview")(band)` → toggles per-band preview; button shows active state
- 200ms Timer: `updateWaveform()` JS call fills waveform canvas
- `≈ X bars` label: JS calculates from `detectedBpm` + `captureDuration` slider value
- 60ms Timer: polls `analysisProgress`, `analysisStep`, `detectedBpm`, `detectedKey` → pushes to JS
- BPM display: shows float or `?`; updates after each completed step
- Key display: shows key string or `?`; updates after key detection completes
- MIDI clips: transition from grayed → draggable as each clip's MIDI file becomes available

**Test Criteria:**
- [ ] Kick sensitivity slider changes value and persists across reload
- [ ] kickFreqLow / kickFreqHigh sliders work correctly
- [ ] captureDuration knob persists correctly
- [ ] REC button starts recording; waveform fills during playback
- [ ] Auto-stop fires at correct time (verify captureDuration = 5s stops after ~5s)
- [ ] STOP during recording preserves partial buffer
- [ ] REC again clears waveform and starts fresh
- [ ] Analyze button disabled before first recording, enabled after
- [ ] Analyze starts analysis; progress bar appears with step label
- [ ] STOP mid-analysis: progress disappears; completed results shown, incomplete show `?`
- [ ] Preview button plays filtered buffer through plugin output
- [ ] Preview loops; adjusting freq sliders changes audio in real-time
- [ ] Preview cancelled by REC, STOP, or Analyze
- [ ] BPM and Key update in UI after analysis completes
- [ ] MIDI clips become draggable after analysis

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

1. **Explicit REC model — user must actively initiate recording:** The user presses REC, then plays their track. Audio only enters the buffer while recording is active. This is a feature (user control) but must be clearly communicated in the UI ("Press REC, then play your track").

2. **BPM accuracy on full mixes:** Full mixes have complex transient profiles. Drum loops = high accuracy (±1 BPM). Full mixes with melody = moderate accuracy (±3 BPM). Ambient music without clear beat = poor accuracy. Document this limitation.

3. **Key detection on full mixes:** Key detection accuracy degrades when multiple instruments are present and competing. Works best on piano, guitar, or melodic solo recordings. Drum-heavy sections may confuse the chromagram. Recommend analyzing a melodic section rather than a drum-only section.

4. **MIDI drag-and-drop across plugin boundary:** The plugin editor is embedded in the DAW's plugin window. Mouse drag events initiated inside the plugin window must escape to the DAW timeline. This works via `performExternalDragDropOfFiles()` on Windows and macOS, but behavior varies by DAW. Test extensively.

5. **Frequency constraint (freqLow < freqHigh):** Parameters are stored independently in APVTS. Validation must happen at analysis time AND at preview filter setup. Consider adding UI warning if constraint is violated.

6. **"Click BPM to set project tempo":** Requires host `AudioPlayHead` support. Support varies. Implement with feature detection and graceful fallback (tooltip: "Host does not support tempo change").

7. **MiniBPM as fallback:** If the custom BPM detection proves inaccurate or takes too long to implement, MiniBPM (https://github.com/breakfastquay/minibpm) is a single-file drop-in. It is GPL-3, so check licensing compatibility before embedding.

8. **Preview mode filter recomputation:** IIR filter coefficients are recomputed every `processBlock()` during preview (to enable live slider adjustment). This is safe on the audio thread (no allocation — only coefficient value updates) but adds a small per-block cost. If performance issues arise, throttle recomputation to only when parameter values change.

---

## References

- Creative brief: `plugins/GrooveScout/.ideas/creative-brief.md`
- Parameter spec: `plugins/GrooveScout/.ideas/parameter-spec-draft.md`
- DSP architecture: `plugins/GrooveScout/.ideas/architecture.md`

Similar plugins for reference patterns:
- DrumRoulette — background thread for sample loading, lock-free audio communication pattern
- Scatter — `juce::AudioBuffer<float>` management patterns
- FlutterVerb — `juce::dsp::IIR::Filter` usage pattern (offline filter application)
