# GrooveScout - Creative Brief

## Overview

**Type:** Analysis Utility (Audio Effect — passes audio through unchanged)
**Core Concept:** Load on any track, hit Analyze, and extract BPM, key, and per-drum MIDI groove patterns as draggable clips
**Status:** 💡 Ideated
**Created:** 2026-02-20

## Vision

GrooveScout fills a genuine gap in music production tools. Load it on any track — a drum loop, a sample, a full reference mix — hit the Analyze button, and it immediately extracts the track's tempo, harmonic key, and the individual MIDI groove patterns of the kick, snare, and hihat. Each result appears in the plugin UI as a draggable MIDI clip, ready to drop onto any track in the DAW.

The primary use case is reference track analysis: a producer wants to capture the feel of a sample or reference song. Instead of manually tapping tempo, guessing the key, or programming drums by ear, GrooveScout does it in one click. The extracted MIDI preserves the exact timing and velocity of each drum hit — the "groove" — not just a quantized approximation.

There are no existing VST plugins that combine all of this in one tool. Ableton has groove extraction (template only, no MIDI notes), Cubase has hit-point-to-MIDI conversion buried in its audio editor, and Melodyne handles pitched content only. GrooveScout is designed to be the missing utility.

## Analysis Engine

| Feature | Algorithm | Notes |
|---------|-----------|-------|
| BPM Detection | Onset detection + autocorrelation / beat tracking | Very reliable across tempos |
| Key Detection | Chroma feature extraction + Krumhansl-Schmuckler algorithm | Works best with harmonic content in the mix |
| Kick MIDI | Band-separated onset detection (40–120 Hz) | Captures timing + velocity |
| Snare MIDI | Band-separated onset detection (200–8000 Hz, noise component) | Captures groove + velocity |
| Hihat MIDI | Band-separated onset detection (5–16 kHz) | Captures open/closed feel via velocity |

## Outputs

| Output | Format | Interaction |
|--------|--------|-------------|
| BPM | Number display | Click to set DAW project tempo |
| Key | Text label (e.g. "F# minor") | Display only |
| Root chord | MIDI clip (root chord triad in detected key) | Drag to MIDI track |
| Kick pattern | MIDI clip | Drag to MIDI track |
| Snare pattern | MIDI clip | Drag to MIDI track |
| Hihat pattern | MIDI clip | Drag to MIDI track |

## Parameters / Controls

| Control | Type | Description |
|---------|------|-------------|
| Analyze | Button | Triggers one-shot analysis of current audio |
| BPM display | Clickable label | Shows detected BPM; click to sync DAW tempo |
| Key display | Label | Shows detected key (e.g. "A minor", "F# major") |
| Root chord clip | Draggable MIDI | Chord triad of detected key root |
| Kick clip | Draggable MIDI | Extracted kick groove pattern |
| Snare clip | Draggable MIDI | Extracted snare groove pattern |
| Hihat clip | Draggable MIDI | Extracted hihat groove pattern |

## Use Cases

- Load on a sample or drum loop to extract its groove and build a beat around it
- Analyze a reference track to match its tempo and key in a new production
- Rip the snare and hihat pattern from a record sample to use as MIDI in a drum machine
- Quick key detection while producing — faster than Cubase's built-in workflow

## Inspirations

- Cubase built-in key detection (inspiration for key output display)
- Ableton's Extract Groove (concept, but GrooveScout provides actual MIDI notes)
- Cubase Hit Points → MIDI (similar intent for drums, but buried in audio editor)
- The producer workflow of sampling records and flipping grooves

## Technical Notes

- Plugin passes audio through unmodified — it is an analyzer, not a processor
- Analysis is offline / one-shot: triggered by the Analyze button, not real-time streaming
- Should work on audio of any length: from 2-bar loops to full 5-minute tracks
- Band-separated onset detection is the primary drum separation approach (no ML required)
- MIDI drag-and-drop from plugin UI requires JUCE ExternalDragAndDropContainer — feasible on Windows and macOS
- Key detection accuracy improves with harmonic content; pure drum tracks will have low key confidence
- Re-analyze anytime by hitting the button again (handles audio changes on the track)

## Next Steps

- [ ] Create UI mockup (`/dream GrooveScout` → option 3)
- [ ] Start implementation (`/plan GrooveScout` then `/implement GrooveScout`)
