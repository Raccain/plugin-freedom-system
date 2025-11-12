#include "DrumRouletteVoice.h"
#include <juce_audio_formats/juce_audio_formats.h>

DrumRouletteVoice::DrumRouletteVoice(int slotNum)
    : slotNumber(slotNum)
{
}

void DrumRouletteVoice::setParameterPointers(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* pitch,
                                              std::atomic<float>* tilt, std::atomic<float>* volume)
{
    attackParam = attack;
    decayParam = decay;
    pitchParam = pitch;
    tiltFilterParam = tilt;
    volumeParam = volume;
}

void DrumRouletteVoice::setCurrentPlaybackSampleRate(double newRate)
{
    // Initialize ADSR with sample rate (Phase 4.2)
    envelope.setSampleRate(newRate);
}

bool DrumRouletteVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<DrumRouletteSound*>(sound) != nullptr;
}

void DrumRouletteVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int)
{
    juce::ignoreUnused(midiNoteNumber);

    currentPosition = 0.0;
    noteVelocity = velocity;
    isActive = true;

    // Read envelope parameters atomically (Phase 4.2)
    if (attackParam != nullptr && decayParam != nullptr)
    {
        float attackMs = attackParam->load();
        float decayMs = decayParam->load();

        juce::ADSR::Parameters adsrParams;
        adsrParams.attack = attackMs / 1000.0f;   // Convert ms to seconds
        adsrParams.decay = decayMs / 1000.0f;
        adsrParams.sustain = 0.0f;                // Fixed at 0 (decay-focused percussive)
        adsrParams.release = 0.05f;               // 50ms fixed (prevents clicks on note-off)

        envelope.setParameters(adsrParams);
        envelope.noteOn();
    }

    // Read pitch parameter and calculate playback rate (Phase 4.2)
    if (pitchParam != nullptr)
    {
        float pitchSemitones = pitchParam->load();
        pitchRatio = std::pow(2.0f, pitchSemitones / 12.0f);
    }
    else
    {
        pitchRatio = 1.0f;  // Default: no pitch shift
    }
}

void DrumRouletteVoice::stopNote(float, bool allowTailOff)
{
    if (allowTailOff)
    {
        // Trigger envelope release (50ms tail-off)
        envelope.noteOff();
    }
    else
    {
        // Immediate stop
        isActive = false;
        clearCurrentNote();
    }
}

void DrumRouletteVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isActive || sampleBuffer.getNumSamples() == 0)
        return;

    // Check if envelope finished (Phase 4.2)
    if (!envelope.isActive())
    {
        isActive = false;
        clearCurrentNote();
        return;
    }

    const int numChannels = juce::jmin(outputBuffer.getNumChannels(), sampleBuffer.getNumChannels());
    const int sampleLength = sampleBuffer.getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const int intPosition = static_cast<int>(currentPosition);

        // Check if sample finished playing
        if (intPosition >= sampleLength - 1)
        {
            isActive = false;
            clearCurrentNote();
            break;
        }

        // Get envelope value for this sample (Phase 4.2)
        const float envelopeValue = envelope.getNextSample();

        // Linear interpolation for pitch shifting (Phase 4.2)
        const float frac = currentPosition - static_cast<float>(intPosition);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float sample0 = sampleBuffer.getSample(channel, intPosition);
            const float sample1 = sampleBuffer.getSample(channel, intPosition + 1);

            // Interpolate between adjacent samples
            const float interpolatedSample = sample0 + frac * (sample1 - sample0);

            // Apply velocity and envelope
            const float outputValue = interpolatedSample * noteVelocity * envelopeValue;

            outputBuffer.addSample(channel, startSample + sample, outputValue);
        }

        // Advance position by pitch ratio (Phase 4.2)
        currentPosition += pitchRatio;
    }
}

void DrumRouletteVoice::loadSample(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader != nullptr)
    {
        const int numChannels = static_cast<int>(reader->numChannels);
        const int numSamples = static_cast<int>(reader->lengthInSamples);

        sampleBuffer.setSize(numChannels, numSamples);
        reader->read(&sampleBuffer, 0, numSamples, 0, true, true);
    }
    else
    {
        // Failed to load - clear sample buffer
        sampleBuffer.setSize(0, 0);
    }
}
