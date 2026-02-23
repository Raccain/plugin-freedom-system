# GrooveScout Changelog

## [1.1.0] - 2026-02-23

### Fixed
- **Key detection returning random results on drum-heavy audio** — Three root causes:
  1. No confidence threshold: algorithm always reported a key even when correlation was near-random (e.g. 0.1). Now requires Pearson correlation >= 0.5, otherwise reports "Unknown".
  2. Weak pre-filter: was 1st-order IIR high-pass at 100 Hz (~6 dB/oct). Kick drum fundamentals (40-80 Hz) leaked through and contaminated the chromagram. Upgraded to 4th-order Butterworth at 150 Hz (24 dB/oct).
  3. No amplitude floor: every FFT bin contributed to pitch class profile, including noise-floor bins that flattened the PCP and reduced discriminative power. Now skips bins below 1% of frame peak magnitude.

### Changed
- Windows builds now enable WebView2 (`JUCE_USE_WIN_WEBVIEW2=1`) with the Microsoft.Web.WebView2 NuGet SDK. This was required for the resource provider to work on Windows.

## [1.0.9] - 2026-02-23

- Precision fix: strength floor now sensitivity-controlled
- Adaptive threshold multiplier raised 4x to 6x

## [1.0.0] - 2026-02-21

- Initial release: BPM detection, key detection, drum onset detection, MIDI drag-to-DAW
