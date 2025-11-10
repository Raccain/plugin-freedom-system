# TapeAge - Research

**Date:** 2025-11-10
**Plugin Type:** Audio Effect (Tape Saturator)

## Similar Plugins

- **PSP Saturator**: Pristine analog saturation with waveshaping and filtering
- **BPB Saturator**: Uses waveshaping to simulate tape and tube saturation
- **apShaper**: Harmonic distortion plugin with tape/tube saturation, wave folding
- **Valhalla Delay**: Tape mode with wow/flutter modulation
- **Commercial tape emulation plugins**: Typically combine saturation + modulation + filtering

## Technical Approach

**Processing Type:** Time-domain waveshaping with delay line modulation

**JUCE Modules:**
- `juce::dsp::Gain` - For drive control and mixing
- `juce::dsp::ProcessorChain` - For signal routing
- `juce::dsp::Oversampling` - To reduce aliasing from nonlinear processing
- Custom waveshaper - For tape saturation (tanh-based)
- Custom modulated delay - For wow/flutter/dropout effects
- Custom noise generator - For tape hiss

**Custom DSP Required:**

1. **Tape Saturation Algorithm**
   - Waveshaping function: `y(x) = tanh(g * x)` where g = drive parameter
   - Symmetric tanh produces odd harmonics (3rd, 5th, 7th)
   - Adding asymmetry yields even harmonics (2nd, 4th) - more "tape-like"
   - Consider soft-knee compression characteristics of tape
   - Oversampling (2x or 4x) to reduce aliasing artifacts

2. **Wow Modulation (< 1 Hz)**
   - Low-frequency sinusoidal LFO modulating delay time
   - Simulates motor speed variations
   - Typical range: 0.1-0.8 Hz
   - Depth controlled by AGE parameter

3. **Flutter Modulation (5-10 Hz)**
   - Higher-frequency LFO or filtered noise
   - Simulates capstan/pinch roller eccentricity
   - Typical range: 5-10 Hz
   - Depth controlled by AGE parameter
   - Can use triangle or smoothed random waveform

4. **Tape Dropout**
   - Random intermittent gain reduction events
   - Triggered by random probability scaled by AGE
   - Short duration (5-50ms)
   - Smooth attack/release to avoid clicks

5. **Tape Noise**
   - Filtered pink/brown noise
   - High-shelf filtered (emphasize high frequencies)
   - Level scaled by AGE parameter
   - Mix with processed signal

## DSP Signal Flow

```
Input → [Dry Buffer Save] →
  [Pre-emphasis Filter?] →
  [Tape Saturation (tanh)] →
  [Modulated Delay (wow + flutter)] →
  [Dropout (random gain reduction)] →
  [Tape Noise Mix] →
  [De-emphasis Filter?] →
  [Dry/Wet Mix] → Output
```

## Parameter Research

| Parameter | Industry Range | Our Range | Default | Notes |
|-----------|---------------|-----------|---------|-------|
| DRIVE | 0-100% | 0.0-1.0 | 0.5 | Controls saturation intensity (tanh gain) |
| AGE | 0-100% | 0.0-1.0 | 0.25 | Scales all degradation artifacts proportionally |
| MIX | 0-100% | 0.0-1.0 | 1.0 | Equal-power crossfade, no phase issues |

**AGE Parameter Scaling (0.0 to 1.0):**
- Wow depth: 0 to ~0.5ms modulation depth
- Flutter depth: 0 to ~0.3ms modulation depth
- Dropout probability: 0 to ~5% chance per second
- Noise level: -inf dB to ~-40dB relative to signal

**DRIVE Parameter Scaling (0.0 to 1.0):**
- Map to tanh gain: 1.0 to ~10.0 (logarithmic feels more natural)
- At 0.0: minimal saturation (g=1.0, nearly linear)
- At 0.5: moderate warmth (g=3.0)
- At 1.0: heavy saturation (g=10.0, still musical)

## Implementation Notes

### Saturation (DRIVE)
- Use oversampling to reduce aliasing (2x minimum, 4x ideal)
- Asymmetric waveshaping for even harmonics: `y = tanh(g*x + a*x^2)` where a is small
- Consider pre/post filtering to shape frequency response
- Must remain musical at maximum drive - avoid harsh clipping

### Modulation (AGE - Wow/Flutter)
- Implement as delay line with time-varying read pointer
- Wow LFO: sine wave, 0.3 Hz default, smooth
- Flutter: smoothed triangle or filtered random, 7 Hz default
- Keep delay buffer small (10-20ms max) to minimize latency
- Use interpolation for fractional delay (linear or cubic)

### Dropout (AGE)
- Random number generator with probability check each buffer
- Smooth envelope (10ms attack, 30ms release) to avoid clicks
- Depth: 6-20dB reduction when active
- Duration: 10-50ms random

### Noise (AGE)
- Pink noise generator (1/f spectrum)
- High-shelf filter (+6dB above 2kHz) for "hiss" character
- Mix at low level (-50dB to -30dB scaled by AGE)

### Mix Control
- Equal-power crossfade: `wet_gain = sqrt(mix), dry_gain = sqrt(1-mix)`
- OR linear: `wet_gain = mix, dry_gain = 1-mix` (simpler, acceptable for this use case)
- Ensure no latency issues between dry and wet paths

## References

**DSP Theory:**
- Signal Processing Stack Exchange: Tape saturation modeling (tanh waveshaping, hysteresis)
- KVR Audio forums: Tape emulation techniques, wow/flutter implementation
- Baby Audio blog: Wow and flutter characteristics and implementation

**JUCE Documentation:**
- `juce::dsp::Oversampling` - Aliasing reduction for nonlinear processing
- `juce::dsp::Gain` - Clean gain/mixing
- `juce::dsp::ProcessorChain` - Serial signal routing
- `juce::AudioBuffer` - Buffer manipulation for delay line

**Example Projects:**
- JUCE DSP demo plugins (gain, filter, oversampling examples)
- Open-source tape emulation plugins on GitHub

**Papers/Articles:**
- Tape saturation = even-order harmonic distortion + hysteresis
- Wow: < 1 Hz (motor variations)
- Flutter: 5-10 Hz (mechanical variations)
- Musical degradation: all artifacts must scale smoothly with AGE parameter

## Key Design Constraints

1. **AGE must remain musical at 100%** - Heavy degradation but never "broken" or "stupid"
2. **Low latency** - Minimize delay buffer size, avoid FFT processing
3. **CPU efficient** - Simple algorithms, avoid expensive operations in processBlock
4. **Smooth parameter changes** - No zipper noise, use smoothing for DRIVE/AGE/MIX
5. **Zero drift from parameter-spec.md** - Implementation must match spec exactly

## Next Steps

- Proceed to Stage 1 (Planning) to calculate complexity and create implementation plan
- Stage 4 (DSP) will likely be phased due to multiple algorithms (saturation, modulation, dropout, noise)
- Consider creating simple test signals to validate each component independently
