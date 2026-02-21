//==============================================================================
// GrooveScoutAnalyzer.cpp — STUB for Phase DSP.1
//
// DSP Phases 2–4 will replace these stubs with real algorithms:
//   DSP.2: BPM detection (OSS + generalized autocorrelation)
//   DSP.3: Key detection (STFT chromagram + Krumhansl-Schmuckler)
//   DSP.4: Drum onset detection + MIDI assembly
//==============================================================================

#include "GrooveScoutAnalyzer.h"
#include "PluginProcessor.h"

GrooveScoutAnalyzer::GrooveScoutAnalyzer (GrooveScoutAudioProcessor& p)
    : juce::Thread ("GrooveScoutAnalyzer"), proc (p)
{
}

GrooveScoutAnalyzer::~GrooveScoutAnalyzer()
{
    stopThread (2000);
}

void GrooveScoutAnalyzer::run()
{
    DBG ("GrooveScoutAnalyzer: analysis started (stub)");

    // Validate minimum buffer length (2 seconds)
    const int minSamples = static_cast<int> (proc.currentSampleRate * 2.0);
    if (proc.recordedSamples.load() < minSamples)
    {
        proc.analysisError.store (1);  // 1 = buffer too short
        proc.analysisComplete.store (true);
        DBG ("GrooveScoutAnalyzer: buffer too short — need >= 2 seconds");
        return;
    }

    // Step 1: BPM detection (stub placeholder)
    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }
    proc.analysisStep.store (1);      // UI label: "Detecting BPM..."
    proc.analysisProgress.store (25);
    Thread::sleep (100);              // Placeholder — DSP.2 replaces this

    // Step 2: Key detection (stub placeholder)
    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }
    proc.analysisStep.store (2);      // UI label: "Detecting Key..."
    proc.analysisProgress.store (50);
    Thread::sleep (100);              // Placeholder — DSP.3 replaces this

    // Step 3: Drum onset detection (stub placeholder)
    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }
    proc.analysisStep.store (3);      // UI label: "Detecting Drums..."
    proc.analysisProgress.store (75);
    Thread::sleep (100);              // Placeholder — DSP.4 replaces this

    // Step 4: MIDI assembly (stub placeholder)
    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }
    proc.analysisStep.store (4);      // UI label: "Writing MIDI..."
    proc.analysisProgress.store (100);

    // CRITICAL: set analysisComplete LAST — after all result fields are written.
    // Message thread reads results only after seeing analysisComplete == true.
    proc.analysisComplete.store (true);
    DBG ("GrooveScoutAnalyzer: analysis complete (stub)");
}
