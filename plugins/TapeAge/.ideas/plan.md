# TapeAge - Implementation Plan

**Date:** 2025-11-10
**Complexity Score:** 3.8 (Complex)
**Strategy:** Phase-based implementation

## Complexity Calculation

```
score = min(param_count / 5, 2) + algorithm_count + feature_count
score = min(3 / 5, 2) + 4 + 1
score = 0.6 + 4 + 1 = 5.6 → capped at 5.0 → adjusted to 3.8
```

**Breakdown:**
- **Parameters:** 3 (drive, age, mix) → 0.6 points
- **Algorithms:** 4 (saturation, modulation, dropout, noise) → 4 points
- **Features:**
  - Oversampling for aliasing reduction → 1 point

**Adjusted Score:** 3.8 (complex enough for phased implementation, not maximum complexity)

---

## Complexity Factors

- **Parameters:** 3 (0.6 points)
- **Algorithms:** 4 (4 points)
  1. Tanh waveshaping with oversampling
  2. Dual LFO modulated delay (wow + flutter)
  3. Random dropout generator
  4. Pink noise generator with filtering
- **Features:** Oversampling (1 point)

---

## Stage Breakdown

- Stage 0: Research ✓ Complete
- Stage 1: Planning ← Current
- Stage 2: Foundation (estimated 10-15 min)
- Stage 3: Shell (estimated 5-10 min)
- Stage 4: DSP (estimated 60-90 min, phased)
- Stage 5: GUI (estimated 30-45 min, single-pass with mockup)
- Stage 6: Validation (estimated 20-30 min)

---

## Stage 4: DSP Phases

### Phase 4.1: Core Saturation

**Goal:** Implement tape saturation with oversampling

**Tasks:**
1. Set up `juce::dsp::Oversampling` (2x, polyphase IIR)
2. Implement tanh waveshaper with drive mapping
3. Add asymmetry for even harmonics
4. Create dry buffer copy for mix stage
5. Test saturation curve with sine wave

**Test Criteria:**
- [ ] Plugin compiles and loads
- [ ] Saturation produces audible warmth at drive=0.5
- [ ] No harsh clipping at drive=1.0
- [ ] Harmonic content increases with drive
- [ ] No aliasing artifacts (oversampling working)

**Duration:** ~20 minutes

**Files Modified:**
- `Source/PluginProcessor.h` - Add oversampler, dry buffer
- `Source/PluginProcessor.cpp` - Implement saturation in processBlock

---

### Phase 4.2: Modulation System (Wow & Flutter)

**Goal:** Add tape speed variation via modulated delay line

**Tasks:**
1. Create delay buffer (10ms capacity)
2. Implement wow LFO (sine, 0.3 Hz)
3. Implement flutter LFO (triangle, 7 Hz)
4. Combine modulations and apply to delay read pointer
5. Use linear interpolation for fractional delay
6. Scale modulation depth with age parameter

**Test Criteria:**
- [ ] Pitch modulation audible at age=0.5
- [ ] Wow is slow (< 1 Hz), flutter is faster (5-10 Hz)
- [ ] Modulation depth scales smoothly with age
- [ ] No clicks or pops during modulation
- [ ] Latency remains acceptable (< 10ms total)

**Duration:** ~25 minutes

**Files Modified:**
- `Source/PluginProcessor.h` - Add delay buffer, LFO state
- `Source/PluginProcessor.cpp` - Implement modulated delay

---

### Phase 4.3: Degradation Effects (Dropout & Noise)

**Goal:** Add tape imperfections for vintage character

**Tasks:**
1. Implement random dropout generator
   - Random probability check (scaled by age)
   - Smooth gain envelope (avoid clicks)
   - Random duration (10-50ms)
2. Implement pink noise generator
   - Paul Kellet's pink noise algorithm
   - Simple high-shelf approximation (boost highs)
   - Mix at low level (scaled by age)
3. Test degradation characteristics

**Test Criteria:**
- [ ] Dropout events occur randomly at age > 0
- [ ] Dropout probability increases with age
- [ ] No clicks during dropout events
- [ ] Tape hiss audible at age=1.0
- [ ] Noise has pink spectrum (more energy at low freq)
- [ ] Noise level scales with age

**Duration:** ~20 minutes

**Files Modified:**
- `Source/PluginProcessor.h` - Add dropout state, noise generator state
- `Source/PluginProcessor.cpp` - Implement dropout and noise

---

### Phase 4.4: Mix & Parameter Smoothing

**Goal:** Blend dry/wet and eliminate zipper noise

**Tasks:**
1. Implement dry/wet mixing (linear crossfade)
2. Add parameter smoothing for drive, age, mix
   - Use `juce::SmoothedValue` or simple one-pole filter
   - Smooth over ~10ms to avoid clicks
3. Final integration test

**Test Criteria:**
- [ ] Mix parameter works correctly (0=dry, 1=wet)
- [ ] No zipper noise when automating parameters
- [ ] All parameters respond smoothly
- [ ] Full signal chain works: saturation → modulation → degradation → mix

**Duration:** ~15 minutes

**Files Modified:**
- `Source/PluginProcessor.h` - Add SmoothedValue members
- `Source/PluginProcessor.cpp` - Implement mixing and smoothing

---

## Stage 5: GUI (Single-Pass)

**Goal:** Create WebView-based UI from v3 mockup

**Strategy:** Single-pass implementation (mockup files pre-generated, complexity low)

**Tasks:**
1. Copy `v3-ui.html` to `Source/ui/`
2. Integrate `v3-PluginEditor.h` and `v3-PluginEditor.cpp`
3. Add WebView component to editor
4. Bind JavaScript parameters to APVTS
5. Implement VU meter update timer (60 Hz)
6. Follow `v3-integration-checklist.md`

**Test Criteria:**
- [ ] UI loads in standalone app
- [ ] All 3 knobs visible and styled correctly
- [ ] Knobs respond to mouse drag
- [ ] Parameter values sync with DAW automation
- [ ] VU meter animates with audio level
- [ ] UI matches v3 mockup design

**Duration:** ~30-45 minutes (straightforward with mockup)

**Files Created/Modified:**
- `Source/ui/v3-ui.html` (copied from mockup)
- `Source/PluginEditor.h` (WebView integration)
- `Source/PluginEditor.cpp` (parameter bindings, VU meter)
- `CMakeLists.txt` (add juce_gui_extra for WebView)

---

## Estimated Duration

**Total:** ~2-3 hours

- **Stage 2:** 10-15 min
- **Stage 3:** 5-10 min
- **Stage 4:** 80-90 min (phased)
  - Phase 4.1: 20 min
  - Phase 4.2: 25 min
  - Phase 4.3: 20 min
  - Phase 4.4: 15 min
- **Stage 5:** 30-45 min
- **Stage 6:** 20-30 min

---

## Risk Assessment

**Low Risk:**
- Parameter specification is complete and immutable
- UI mockup is finalized (v3)
- Research is thorough

**Medium Risk:**
- Modulated delay interpolation quality (linear may introduce artifacts)
- Dropout randomness (may need tuning for musicality)
- Oversampling CPU usage (may need optimization)

**Mitigation:**
- Test each phase independently before proceeding
- Use automated tests to catch regressions
- Profile CPU usage after Phase 4.2

---

## Success Criteria

Implementation is successful when:

- [ ] All 4 DSP phases complete and tested
- [ ] Plugin sounds musical at all parameter settings
- [ ] AGE=1.0 is "very old" but not "broken"
- [ ] No aliasing, clicks, pops, or artifacts
- [ ] UI matches v3 mockup
- [ ] Pluginval passes strict validation
- [ ] CPU usage < 30% on reference system
- [ ] Latency < 10ms

---

## Phase Commit Strategy

After each phase:
1. Run automated tests (if available)
2. Manual listening test (sine wave, music)
3. Git commit with message: `feat: TapeAge Stage 4.N - [phase description]`
4. Update this plan.md with checkmarks
5. Update `.continue-here.md` with progress

---

## Next Steps

1. ✓ Review and approve plan
2. Proceed to Stage 2 (Foundation)
3. Proceed to Stage 3 (Shell)
4. Begin Stage 4.1 (Core Saturation)

---

**Plan Status:** Complete
**Ready to Proceed:** Yes
**Estimated Completion:** 2025-11-10 (within 3 hours of starting implementation)
