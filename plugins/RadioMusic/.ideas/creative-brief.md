# RadioMusic - Creative Brief

## Overview

**Type:** MIDI-triggered sampler / generative instrument
**Core Concept:** Creative audio file browser that continuously plays audio files from a folder, with MIDI-triggered re-sampling at user-defined start points
**Status:** 💡 Ideated
**Created:** 2025-01-10

## Vision

RadioMusic is inspired by the Eurorack module of the same name. It's designed for ignorant, creative discovery—load a folder of audio files and explore them through playback manipulation rather than traditional sampling workflows.

The plugin continuously plays audio from the selected folder. Users "tune" between files with a Station knob and choose re-trigger points with a Start knob. MIDI notes restart playback from the Start position, enabling rhythmic re-sampling without stopping the continuous playback nature of the source material.

This creates a hybrid between a sample player and a generative instrument—part radio scanner, part creative sampler. The waveform display adapts to show what's available to play as the Start position moves, visually reinforcing the "shaving off" of the beginning of files.

## Parameters

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| Station | 0-100% (maps to file index) | 0% | Selects which file from the loaded folder is playing |
| Start | 0-100% (maps to file position) | 0% | Defines the re-trigger point within the selected file |
| Volume | -60dB to +12dB | 0dB | Output gain control |
| Speed/Pitch | 0.25x to 4.0x | 1.0x | Playback speed and pitch (coupled, like tape) |

## UI Concept

**Layout:**
- Top: Waveform display (largest component)
  - Shows entire file waveform
  - As Start knob increases, darkens/fades the "unavailable" portion before the start point
  - Highlights the active playback region
  - Shows current playback position marker
- Middle: Folder path display + Browse button
- Bottom: Four knobs in a row (Station, Start, Volume, Speed/Pitch)
- File info: Display current file name and index (e.g., "5/127: track_name.wav")

**Visual Style:**
- Clean, functional design
- Waveform should be prominent and easy to read
- Visual feedback for Start position masking
- Eurorack-inspired simplicity

**Key Elements:**
- Adaptive waveform display that visually represents available playback region
- Browse button for folder selection
- Real-time playback position indicator
- File index/name display

## Use Cases

- **Creative sampling:** Drop a folder of breaks, melodies, or field recordings and discover unexpected start points through experimentation
- **Generative composition:** Let files play continuously while MIDI patterns create rhythmic re-triggering
- **Sound design:** Explore granular-like textures by triggering short segments of longer files
- **Live performance:** Quick access to a library of sounds without loading individual samples
- **Lazy sampling workflow:** Discover sounds through play rather than precise editing

## Inspirations

- **Music Thing Modular Radio Music** (Eurorack module) - Core concept and workflow
- Tape-style playback with Station "tuning" metaphor
- Generative/aleatoric music techniques

## Technical Notes

### Audio File Handling
- Support common formats: WAV, AIFF, MP3, FLAC, OGG
- Handle folders with 100+ files without performance degradation
- Lazy loading: Only load waveform data for currently selected file
- Stream large files from disk rather than loading entirely into RAM

### Playback Behavior
- **Continuous playback:** When transport is playing, audio plays continuously from the current file
- **MIDI triggering:** Any MIDI note restarts playback from the Start position
- **Station changes:** Switching files mid-playback queues the new file but doesn't change audio until next MIDI trigger (if MIDI is being used) or immediately if just free-running
- **Transport behavior:** Stop/start always resumes from Start position, not from where it left off
- **Looping:** Files loop seamlessly when reaching the end

### Waveform Display
- Render entire file waveform with downsampling for performance
- Dynamically fade/darken the region before Start position
- Show real-time playback position cursor
- Update waveform when Station knob changes files

### DSP Considerations
- Variable speed playback (0.25x - 4.0x) affects pitch (tape-style, not time-stretching)
- Smooth crossfading when changing Speed/Pitch to avoid clicks
- Click-free MIDI re-triggering with short fade-in (1-5ms)

### File Management
- Scan folder on load, cache file list
- Sort files alphabetically by default
- Handle missing files gracefully (skip to next valid file)
- Watch for folder changes? (stretch goal - optional rescan button)

## Next Steps

- [ ] Create UI mockup (`/dream RadioMusic` → option 3)
- [ ] Start implementation (`/implement RadioMusic`)
