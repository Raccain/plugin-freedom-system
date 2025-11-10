# RadioMusic - Parameter Specification

**Status:** 🔒 Locked Contract (v1)
**Created:** 2025-01-10
**Plugin Type:** MIDI-triggered Sampler

---

## Total Parameter Count

**Total:** 4 parameters

---

## Parameter Definitions

### station

- **Type:** Float
- **Range:** 0.0 to 100.0 (percentage)
- **Default:** 0.0
- **Skew Factor:** Linear
- **Unit:** %
- **Center Detent:** No
- **UI Control:** Rotary knob, leftmost position
- **DSP Usage:** Selects which file from loaded folder is currently playing (maps to file index)
- **Behavior:**
  - 0% = first file in folder
  - 100% = last file in folder
  - Interpolates linearly across file list
  - Station changes queue new file (applied on next MIDI trigger or immediately if free-running)

---

### start

- **Type:** Float
- **Range:** 0.0 to 100.0 (percentage)
- **Default:** 0.0
- **Skew Factor:** Linear
- **Unit:** %
- **Center Detent:** No
- **UI Control:** Rotary knob, second from left
- **DSP Usage:** Defines re-trigger point within selected audio file
- **Behavior:**
  - 0% = beginning of file
  - 100% = end of file
  - All MIDI note triggers restart playback from this position
  - Transport start/restart also begins from this position
  - Waveform display darkens region before this position

---

### speed_pitch

- **Type:** Float
- **Range:** -1.0 to +1.0 (bipolar)
- **Default:** 0.0 (center)
- **Skew Factor:** Linear
- **Unit:** Multiplier (displayed as "x.xxX")
- **Center Detent:** Yes (snaps to 0.0 within ±0.05 threshold)
- **UI Control:** Rotary knob with center marker, third from left
- **DSP Usage:** Playback speed and pitch control (tape-style, coupled)
- **Behavior:**
  - -1.0 = 0.5x speed (half speed, one octave down)
  - 0.0 = 1.0x speed (normal playback, center position)
  - +1.0 = 2.0x speed (double speed, one octave up)
  - Mapping formula: `multiplier = pow(2, speed_pitch)`
  - Affects both speed and pitch (no time-stretching)
  - Smooth crossfading when changing to avoid clicks

---

### volume

- **Type:** Float
- **Range:** -60.0 to +12.0 dB
- **Default:** 0.0 (center)
- **Skew Factor:** Linear
- **Unit:** dB
- **Center Detent:** Yes (snaps to 0.0dB within ±0.5dB threshold)
- **UI Control:** Rotary knob with center marker, rightmost position
- **DSP Usage:** Output gain control
- **Behavior:**
  - -60dB = effectively silence (practical -infinity)
  - 0dB = unity gain (center position)
  - +12dB = 4x amplitude
  - Applied after all other processing
  - Display shows "-∞dB" when value ≤ -60dB

---

## UI Layout Specification

**Window Size:** 700px × 500px (landscape)

**Layout:**
- **Top Section (604×280px):** Waveform display with browse button
- **Bottom Section:** Four rotary knobs horizontally aligned
- **Spacing:** 48px padding, 36px gap between sections

**Knob Alignment:**
- Left edge of Station knob aligns with left edge of waveform
- Right edge of Volume knob aligns with right edge of waveform
- Start and Speed/Pitch distributed evenly between

**Visual Style:**
- Soft neomorphic design
- Warm cream gradient background (#e8dcc8 to #d9cbb5)
- Dark grey waveform background (#3a3a3a)
- Burnt orange waveform (#c96a3d)
- Cream-colored UI elements

---

## Parameter Interaction Rules

### Station Changes
- Changing Station knob queues new file
- New file loads on next MIDI trigger (if using MIDI)
- If free-running (no MIDI), file changes immediately
- Waveform display updates to show new file

### Start Position
- Changes take effect immediately on waveform display (masking)
- Next MIDI trigger or transport restart begins from new position
- Does NOT interrupt currently playing audio

### Speed/Pitch
- Changes apply immediately to playback
- Smooth crossfading to avoid clicks
- Center snap provides tactile feedback at 1.0x

### Volume
- Changes apply immediately to output
- Center snap provides tactile feedback at unity gain
- Applied post-DSP (final stage)

---

## Non-Interactive UI Elements

### Waveform Display
- Shows downsampled representation of current file (~800 points)
- Region before Start position darkened (#5a5a5a)
- Active region shown in burnt orange (#c96a3d)
- Real-time playback cursor (cream vertical line)
- Updates at 60fps

### Browse Button
- SVG folder icon in top-right of waveform
- Cream color (#e8dcc8)
- Opens system folder chooser
- Scans folder for audio files (WAV, AIFF, MP3, FLAC, OGG)

### File Info Display
- Bottom-left of waveform
- Format: "[index]/[total]: [filename]"
- Example: "5/127: track_name.wav"
- Cream text on dark background

---

## Implementation Notes

### Parameter Value Tree State

```cpp
AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>(
        "station",
        "Station",
        NormalisableRange<float>(0.0f, 100.0f),
        0.0f
    ));

    params.push_back(std::make_unique<AudioParameterFloat>(
        "start",
        "Start",
        NormalisableRange<float>(0.0f, 100.0f),
        0.0f
    ));

    params.push_back(std::make_unique<AudioParameterFloat>(
        "speed_pitch",
        "Speed",
        NormalisableRange<float>(-1.0f, 1.0f),
        0.0f
    ));

    params.push_back(std::make_unique<AudioParameterFloat>(
        "volume",
        "Volume",
        NormalisableRange<float>(-60.0f, 12.0f),
        0.0f
    ));

    return { params.begin(), params.end() };
}
```

### DAW Automation Support

All parameters support:
- Real-time automation recording
- Automation playback
- Parameter smoothing (internally)
- State save/restore

---

## Validation Rules

### Station
- Clamp to 0-100%
- Map to file index: `fileIndex = floor((station / 100.0) * totalFiles)`
- Handle edge cases (empty folder, single file)

### Start
- Clamp to 0-100%
- Map to sample position: `samplePos = floor((start / 100.0) * fileLengthInSamples)`
- Ensure position doesn't exceed file length

### Speed/Pitch
- Clamp to -1.0 to +1.0
- Apply center snap in UI layer only (DSP receives exact value)
- Convert to multiplier: `rate = pow(2, speed_pitch)`
- Range results in: 0.5x to 2.0x playback rate

### Volume
- Clamp to -60dB to +12dB
- Convert to linear gain: `gain = pow(10, volume / 20.0)`
- Special case: volume ≤ -60dB → gain = 0.0
- Apply center snap in UI layer only

---

## Testing Checklist

- [ ] All parameters appear in DAW automation lanes
- [ ] Parameter values persist across project save/load
- [ ] Center snap works for Speed and Volume
- [ ] Station correctly cycles through all files
- [ ] Start position accurately masks waveform
- [ ] Speed/Pitch smoothly adjusts playback
- [ ] Volume control affects output level
- [ ] No crashes with extreme parameter values
- [ ] Automation recording/playback works
- [ ] Preset save/load preserves all parameters

---

**This specification is immutable during implementation. Any changes require creating a new version via `/improve`.**
