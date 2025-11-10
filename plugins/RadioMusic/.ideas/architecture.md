# RadioMusic - Research

**Date:** 2025-01-10
**Plugin Type:** MIDI-triggered Sampler

## Similar Plugins

### Music Thing Modular Radio Music (Eurorack Hardware)
**Key Features:**
- Continuous playback from SD card folders (16 banks of 75 channels)
- Files loop endlessly in background - don't re-trigger on station change
- When switching stations and returning, playback continues from real-time position
- Behaves like a radio scanner, not a traditional sampler
- .raw headerless WAV files on SD card

**Parameter Ranges:**
- Station select: 16 banks × 75 channels
- No explicit start position control (hardware version)
- Simple CV control for station selection

**Relevance:** Direct inspiration - we're creating software version with enhanced control

### JUCE Sampler Examples (General Sampler Architecture)
**Key Features:**
- AudioFormatManager + AudioFormatReader for file loading
- Synthesizer class manages SamplerVoices and SamplerSounds
- AudioTransportSource for playback control
- Sample rate conversion support

**Parameter Ranges:**
- Typically: Volume (-∞ to +12dB), Pitch (±12 semitones)
- MIDI velocity sensitivity (0-100%)

**Relevance:** Standard JUCE sampler architecture we'll build upon

### Variable Speed/Pitch Samplers (Industry Standard)
**Typical Features:**
- Speed multipliers: 0.25x - 4.0x (2 octaves down/up)
- Tape-style coupling (speed affects pitch)
- No time-stretching (simpler, more musical for this use case)

**Parameter Ranges:**
- Speed: Often -200% to +200% or 0.25x to 4.0x
- Default: 1.0x (100%, normal speed)

## Technical Approach

**Processing Type:** Time-domain sample playback with variable speed resampling

**JUCE Modules Identified** (from Context7: `/juce-framework/juce`):

- **`juce::AudioFormatManager`** - File format detection and reader creation
  - Supports WAV, AIFF, FLAC, OGG, MP3 via `registerBasicFormats()`
  - Creates `AudioFormatReader` instances for files

- **`juce::AudioFormatReader`** - Low-level file reading
  - Properties: `sampleRate`, `numChannels`, `lengthInSamples`
  - `read()` method for loading audio data into buffers
  - Streaming capability for large files

- **`juce::AudioFormatReaderSource`** - Streaming playback wrapper
  - Wraps `AudioFormatReader` for continuous playback
  - Implements `AudioSource` interface
  - Handles buffering and streaming automatically

- **`juce::Resampler`** (Oboe integration) - Variable speed playback
  - MultiChannelResampler for speed/pitch control
  - Quality settings: Low/Medium/High
  - Sample rate conversion for tape-style speed control

- **`juce::MidiBuffer`** - MIDI message handling
  - `isNoteOn()`, `isNoteOff()` for trigger detection
  - `samplePosition` for accurate timing
  - Process MIDI in `processBlock()`

- **`juce::AudioBuffer<float>`** - Audio data storage
  - Used for waveform display data (downsampled)
  - Playback buffer management

- **`juce::File`** - File system operations
  - Folder scanning for audio files
  - Path management
  - `findChildFiles()` for directory listing

**Custom DSP Required:**
1. **File queueing system** - Station changes queue next file, don't interrupt playback
2. **Start position handling** - Calculate sample offset from percentage
3. **Seamless looping** - Detect end of file, restart from Start position
4. **Speed-based resampling** - Map -1.0 to +1.0 to 0.5x-2.0x multiplier
5. **Waveform downsampling** - Generate ~800 point waveform for UI display
6. **MIDI re-trigger logic** - Reset playback position on note-on events

## Parameter Research

| Parameter | Typical Range | Default | Skew | Notes (Industry Standard) |
|-----------|---------------|---------|------|--------------------------|
| Station | 0-100% | 0% | Linear | Maps to file index in folder |
| Start | 0-100% | 0% | Linear | Sample position within file |
| Speed/Pitch | -1.0 to +1.0 | 0.0 | Linear | Bipolar: 0.0 = 1.0x, maps to 0.5x-2.0x via pow(2, value) |
| Volume | -60dB to +12dB | 0dB | Linear | Standard plugin gain range, -60dB = silence |

**Speed/Pitch Mapping:**
```
parameter = -1.0 → multiplier = 0.5x (half speed, octave down)
parameter =  0.0 → multiplier = 1.0x (normal)
parameter = +1.0 → multiplier = 2.0x (double speed, octave up)
Formula: multiplier = pow(2, speed_pitch_param)
```

**Center Snap Thresholds:**
- Speed/Pitch: ±0.05 snap to 0.0
- Volume: ±0.5dB snap to 0.0dB

## Technical Feasibility

**JUCE Support:** ✓ All core modules available in JUCE 8
- Audio file loading: `AudioFormatManager`, `AudioFormatReader`
- MIDI processing: `MidiBuffer`, `MidiMessage`
- Resampling: Oboe MultiChannelResampler (built-in)
- File I/O: `juce::File` and directory operations

**Complexity Factors:**
1. **File Management:**
   - Scan folder, cache file list (100+ files)
   - Lazy loading (only load current file's waveform)
   - Handle missing/corrupt files gracefully

2. **Playback State:**
   - Continuous playback even when changing stations
   - Queue next file, apply on MIDI trigger or immediately
   - Maintain separate playback position vs start position

3. **MIDI Triggering:**
   - Sample-accurate re-trigger on note-on
   - Click-free fade-in (1-5ms)
   - Independent from transport state

4. **Variable Speed:**
   - Real-time resampling without clicks
   - Smooth parameter changes
   - Efficient CPU usage with quality trade-offs

5. **UI Communication:**
   - Waveform data updates (downsampled to ~800 points)
   - Real-time playback cursor position (60fps)
   - File info display (name, index)
   - Parameter binding (4 knobs)

**Implementation Challenges:**
- **Seamless looping with variable speed:** Need to handle end-of-file detection at any playback rate
- **Queue vs immediate file changes:** Logic depends on MIDI activity state
- **Large file streaming:** Avoid loading entire files into RAM
- **Waveform generation:** Downsample efficiently for UI without blocking audio thread
- **Thread safety:** UI updates vs audio thread file playback

**Estimated Complexity:** Medium-High
- 4 parameters (simple)
- File I/O and folder management (moderate)
- MIDI triggering + variable speed (moderate)
- Continuous playback state machine (complex)
- WebView UI with waveform display (moderate)

## Design Sync Results

**UI Mockup:** ✓ Exists (`v3-ui.yaml`, `v3-ui.html`)

**Brief ↔ Mockup Consistency:** ✓ Pass

**Verified alignment:**
- 4 parameters match exactly (Station, Start, Speed/Pitch, Volume)
- Ranges match specification
- Bipolar controls (Speed, Volume) have center detent
- Waveform display with adaptive masking matches vision
- Browse button for folder selection present
- File info display included

**No conflicts found** - UI mockup accurately implements creative brief vision.

## References

- **Context7 JUCE Library:** `/juce-framework/juce` (476 code snippets, trust score 8.3)
- **Professional Hardware:** Music Thing Modular Radio Music (Eurorack module) - discontinued but well-documented
- **JUCE Documentation:**
  - AudioFormatReader: https://docs.juce.com/master/classAudioFormatReader.html
  - Build an audio player tutorial: https://docs.juce.com/master/tutorial_playing_sound_files.html
  - Looping audio sample buffer: https://docs.juce.com/master/tutorial_looping_audio_sample_buffer.html
- **JUCE Forum Discussions:**
  - Simple sampler implementation: https://doomy.org/creating-a-very-simple-sampler-audio-plugin-in-juce-5/
  - Variable speed/pitch techniques: JUCE forum discussions on resampling

## Architecture Notes (for architecture.md creation)

**Core Components Needed:**

1. **File Manager**
   - Scan folder for audio files (WAV/AIFF/MP3/FLAC/OGG)
   - Cache file list, sort alphabetically
   - Provide file access by index (Station parameter)

2. **Playback Engine**
   - AudioFormatReaderSource for streaming
   - Variable speed resampler (Oboe MultiChannelResampler)
   - Looping logic with Start position offset
   - MIDI trigger detection and position reset

3. **Parameter System**
   - APVTS with 4 parameters
   - Center snap logic for bipolar controls
   - Parameter smoothing for Speed/Volume

4. **Waveform Provider**
   - Background thread for waveform generation
   - Downsample to ~800 points for UI
   - Update on Station parameter change

5. **UI Communication**
   - WebView component
   - JavaScript ↔ C++ message bridge
   - Real-time playback position updates (60fps)
   - Folder browser integration

**Signal Flow:**
```
Folder → File Manager → AudioFormatReader → Resampler → Volume → Output
                           ↑                    ↑          ↑
                        Station             Speed/Pitch  Volume
                        parameter           parameter    parameter

MIDI In → Note On Detection → Reset Playback Position to Start parameter
```

This modular architecture matches JUCE best practices and supports all required features.
