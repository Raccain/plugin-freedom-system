#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout TapeAgeAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // drive - Tape saturation amount
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "drive", 1 },
        "Drive",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),  // 0-100%, linear
        0.5f  // Default: 50%
    ));

    // age - Tape degradation amount
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "age", 1 },
        "Age",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),  // 0-100%, linear
        0.25f  // Default: 25%
    ));

    // mix - Dry/wet blend
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "mix", 1 },
        "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f),  // 0-100%, linear
        1.0f  // Default: 100% wet
    ));

    return layout;
}

TapeAgeAudioProcessor::TapeAgeAudioProcessor()
    : AudioProcessor(BusesProperties()
                        .withInput("Input", juce::AudioChannelSet::stereo(), true)
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , oversampler(2, 1, juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple)  // 2x oversampling, 1 stage, FIR filters
    , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

TapeAgeAudioProcessor::~TapeAgeAudioProcessor()
{
}

void TapeAgeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Prepare DSP spec
    currentSpec.sampleRate = sampleRate;
    currentSpec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    currentSpec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());
    currentSampleRate = sampleRate;

    // Phase 4.1: Prepare oversampling engine
    oversampler.initProcessing(static_cast<size_t>(samplesPerBlock));
    oversampler.reset();

    // Phase 4.2: Prepare wow/flutter modulation
    // 200ms delay line buffer for pitch modulation (architecture.md line 28)
    int delaySamples = static_cast<int>(sampleRate * 0.2);
    delayLine.setMaximumDelayInSamples(delaySamples);
    delayLine.prepare(currentSpec);
    delayLine.reset();

    // Initialize random phase offsets per channel for stereo width
    lfoPhase[0] = random.nextFloat() * juce::MathConstants<float>::twoPi;
    lfoPhase[1] = random.nextFloat() * juce::MathConstants<float>::twoPi;

    // Phase 4.3: Prepare degradation features
    // Initialize dropout state (no dropout at start)
    dropoutCountdown = static_cast<int>(sampleRate * 0.1);  // 100ms until first check
    inDropout = false;
    dropoutSamplesRemaining = 0;
    dropoutEnvelope = 1.0f;

    // Initialize noise filter state to zero
    noiseFilterState[0] = 0.0f;
    noiseFilterState[1] = 0.0f;

    // Phase 4.4: Prepare dry/wet mixer
    dryWetMixer.prepare(currentSpec);
    dryWetMixer.reset();

    // Set wet latency to compensate for oversampler latency (architecture.md line 134)
    int oversamplerLatency = static_cast<int>(oversampler.getLatencyInSamples());
    dryWetMixer.setWetLatency(static_cast<float>(oversamplerLatency));
}

void TapeAgeAudioProcessor::releaseResources()
{
    // Phase 4.1: Reset DSP components
    oversampler.reset();

    // Phase 4.2: Reset wow/flutter modulation
    delayLine.reset();
}

void TapeAgeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);

    // Clear unused channels
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Phase 4.4: Store dry signal BEFORE any processing
    juce::dsp::AudioBlock<float> block(buffer);
    dryWetMixer.pushDrySamples(block);

    // Read mix parameter (0.0 = fully dry, 1.0 = fully wet)
    auto* mixParam = parameters.getRawParameterValue("mix");
    float mixValue = mixParam->load();
    dryWetMixer.setWetMixProportion(mixValue);

    // Phase 4.1: Core Saturation Processing
    // Processing chain:
    // 1. Read drive parameter and calculate gain
    // 2. Upsample 2x
    // 3. Apply tanh saturation manually (drive controls gain scaling)
    // 4. Downsample

    // Read drive parameter (0.0 to 1.0)
    auto* driveParam = parameters.getRawParameterValue("drive");
    float drive = driveParam->load();

    // Progressive curve mapping (architecture.md):
    // 0-30%: Very subtle (multiply by 1-2 before tanh)
    // 30-70%: Moderate warmth (multiply by 2-8)
    // 70-100%: Heavy saturation (multiply by 8-20)
    float gain;
    if (drive <= 0.3f)
    {
        // Subtle range: linear interpolation from 1 to 2
        gain = 1.0f + (drive / 0.3f) * 1.0f;
    }
    else if (drive <= 0.7f)
    {
        // Moderate range: linear interpolation from 2 to 8
        gain = 2.0f + ((drive - 0.3f) / 0.4f) * 6.0f;
    }
    else
    {
        // Heavy range: linear interpolation from 8 to 20
        gain = 8.0f + ((drive - 0.7f) / 0.3f) * 12.0f;
    }

    // Upsample
    auto oversampledBlock = oversampler.processSamplesUp(block);

    // Apply tanh saturation manually in oversampled domain
    for (size_t channel = 0; channel < oversampledBlock.getNumChannels(); ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer(channel);
        for (size_t sample = 0; sample < oversampledBlock.getNumSamples(); ++sample)
        {
            channelData[sample] = std::tanh(gain * channelData[sample]);
        }
    }

    // Downsample back to original sample rate
    oversampler.processSamplesDown(block);

    // Phase 4.2: Wow/Flutter Modulation
    // Processing chain: Apply pitch modulation via delay line after saturation
    // Read age parameter (0.0 to 1.0)
    auto* ageParam = parameters.getRawParameterValue("age");
    float age = ageParam->load();

    // Calculate LFO modulation depth based on age
    // Architecture.md: ±10 cents at max age (pitch ratio = 2^(cents/1200))
    // ±10 cents = 2^(10/1200) = 1.005777895 (~0.58% pitch variation)
    const float maxPitchVariationCents = 10.0f;
    const float pitchVariationRatio = std::pow(2.0f, maxPitchVariationCents / 1200.0f) - 1.0f;  // ~0.005777895
    float modulationDepth = age * pitchVariationRatio;

    // LFO frequency: 0.5-2Hz (architecture.md line 29)
    // Use 1.0Hz as base frequency, scaled by age for subtle variation
    const float lfoFrequency = 1.0f + age;  // 1.0-2.0Hz range
    const float lfoPhaseIncrement = (lfoFrequency * juce::MathConstants<float>::twoPi) / static_cast<float>(currentSampleRate);

    // Process each channel
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Calculate LFO modulation (sine wave)
            float lfoValue = std::sin(lfoPhase[channel]);

            // Calculate delay time in samples
            // Base delay at center of buffer (100ms) + modulation
            float baseDelaySamples = static_cast<float>(currentSampleRate) * 0.1f;  // 100ms center
            float modulationSamples = lfoValue * modulationDepth * baseDelaySamples;
            float totalDelay = baseDelaySamples + modulationSamples;

            // Push input sample to delay line
            delayLine.pushSample(channel, channelData[sample]);

            // Read modulated sample from delay line
            channelData[sample] = delayLine.popSample(channel, totalDelay);

            // Advance LFO phase
            lfoPhase[channel] += lfoPhaseIncrement;
            if (lfoPhase[channel] >= juce::MathConstants<float>::twoPi)
                lfoPhase[channel] -= juce::MathConstants<float>::twoPi;
        }
    }

    // Phase 4.3: Degradation Features (Dropout + Noise)
    // Processing chain: Apply dropout and tape noise after wow/flutter modulation

    // === Dropout Generator ===
    // Check every 100ms if dropout should occur (architecture.md line 116)
    dropoutCountdown -= numSamples;
    if (dropoutCountdown <= 0)
    {
        dropoutCountdown = static_cast<int>(currentSampleRate * 0.1);  // Reset to 100ms

        // Probability scaled by age (architecture.md line 38: rare events every 5-10 seconds at max age)
        // At age=1.0, probability = 0.02 (2% per 100ms check = ~20% per second = 5-10 second intervals)
        float dropoutProbability = age * 0.02f;

        if (random.nextFloat() < dropoutProbability && !inDropout)
        {
            // Start dropout event
            inDropout = true;
            // Random duration: 50-150ms (architecture.md line 39)
            float dropoutDurationMs = 50.0f + random.nextFloat() * 100.0f;
            dropoutSamplesRemaining = static_cast<int>(currentSampleRate * dropoutDurationMs / 1000.0f);
        }
    }

    // Process dropout envelope (smooth attack/release to avoid clicks)
    if (inDropout && dropoutSamplesRemaining > 0)
    {
        // Random attenuation factor 0.1-0.3 (70-90% reduction) (architecture.md line 40)
        const float dropoutTargetGain = 0.1f + random.nextFloat() * 0.2f;

        // Envelope attack/release time: 5-10ms (architecture.md line 118)
        const float envelopeTimeMs = 7.5f;  // Mid-range
        const float envelopeTimeSamples = currentSampleRate * envelopeTimeMs / 1000.0f;
        const float envelopeIncrement = 1.0f / envelopeTimeSamples;

        for (int sample = 0; sample < numSamples && dropoutSamplesRemaining > 0; ++sample)
        {
            // Attack: Fade down to dropout gain
            if (dropoutEnvelope > dropoutTargetGain)
            {
                dropoutEnvelope -= envelopeIncrement;
                if (dropoutEnvelope < dropoutTargetGain)
                    dropoutEnvelope = dropoutTargetGain;
            }

            // Apply dropout attenuation to all channels (stereo coherence)
            for (int channel = 0; channel < numChannels; ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                channelData[sample] *= dropoutEnvelope;
            }

            dropoutSamplesRemaining--;
        }

        if (dropoutSamplesRemaining <= 0)
        {
            // End dropout, start release
            inDropout = false;
        }
    }
    else if (dropoutEnvelope < 1.0f)
    {
        // Release: Fade back to full gain
        const float envelopeTimeMs = 7.5f;
        const float envelopeTimeSamples = currentSampleRate * envelopeTimeMs / 1000.0f;
        const float envelopeIncrement = 1.0f / envelopeTimeSamples;

        for (int sample = 0; sample < numSamples; ++sample)
        {
            dropoutEnvelope += envelopeIncrement;
            if (dropoutEnvelope >= 1.0f)
            {
                dropoutEnvelope = 1.0f;
                break;
            }

            // Apply release envelope to all channels
            for (int channel = 0; channel < numChannels; ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                channelData[sample] *= dropoutEnvelope;
            }
        }
    }

    // === Tape Noise Generator ===
    // Filtered white noise at VERY subtle amplitude (architecture.md line 124-129)
    // Noise amplitude scaled by age: 0% = silent, 100% = -80dB to -60dB

    // Calculate noise gain based on age (VERY subtle)
    // age = 0.0 → noiseGain = 0.0 (silent)
    // age = 1.0 → noiseGain = 0.0001 to 0.001 (-80dB to -60dB)
    float noiseGain = age * 0.0001f;  // Maximum -80dB at full age (VERY subtle)

    if (noiseGain > 0.0f)
    {
        // One-pole lowpass filter coefficient for ~8kHz cutoff (architecture.md line 125)
        // Formula: coeff = 1 - exp(-2π * cutoffFreq / sampleRate)
        const float cutoffFreq = 8000.0f;
        const float filterCoeff = 1.0f - std::exp(-juce::MathConstants<float>::twoPi * cutoffFreq / static_cast<float>(currentSampleRate));

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);

            for (int sample = 0; sample < numSamples; ++sample)
            {
                // Generate white noise: range [-1.0, 1.0]
                float whiteNoise = random.nextFloat() * 2.0f - 1.0f;

                // Apply one-pole lowpass filter (simulates tape frequency response)
                noiseFilterState[channel] += filterCoeff * (whiteNoise - noiseFilterState[channel]);

                // Add filtered noise at very low amplitude
                channelData[sample] += noiseFilterState[channel] * noiseGain;
            }
        }
    }

    // Phase 4.4: Mix dry/wet signals AFTER all processing
    // Equal-power crossfade with latency compensation
    dryWetMixer.mixWetSamples(block);

    // Phase 5.2: Calculate peak level for VU meter (AFTER all processing)
    float peakLevel = 0.0f;
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float channelPeak = buffer.getMagnitude(channel, 0, numSamples);
        peakLevel = std::max(peakLevel, channelPeak);
    }

    // Convert to dB and store atomically (clamp to -100dB minimum to avoid log(0))
    float peakDb = peakLevel > 0.00001f
        ? juce::Decibels::gainToDecibels(peakLevel)
        : -100.0f;
    outputLevel.store(peakDb, std::memory_order_relaxed);
}

juce::AudioProcessorEditor* TapeAgeAudioProcessor::createEditor()
{
    return new TapeAgeAudioProcessorEditor(*this);
}

void TapeAgeAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Debug logging
    juce::File("/tmp/tapeage_debug.log").appendText(
        juce::Time::getCurrentTime().toString(true, true) + " - getStateInformation called\n");

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TapeAgeAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Debug logging
    juce::File debugLog("/tmp/tapeage_debug.log");
    debugLog.appendText(
        juce::Time::getCurrentTime().toString(true, true) + " - setStateInformation called\n");

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
    {
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

        // Log parameter values after restoration
        auto* driveParam = parameters.getRawParameterValue("drive");
        auto* ageParam = parameters.getRawParameterValue("age");
        auto* mixParam = parameters.getRawParameterValue("mix");

        debugLog.appendText(
            "  Parameters after restore - Drive: " + juce::String(driveParam->load()) +
            ", Age: " + juce::String(ageParam->load()) +
            ", Mix: " + juce::String(mixParam->load()) + "\n");
    }
}

// Factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapeAgeAudioProcessor();
}
