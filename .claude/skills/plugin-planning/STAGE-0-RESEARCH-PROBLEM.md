# Stage 0 Research Problem Statement

**Date:** 2025-11-12
**Context:** Deep audit of Stage 0 research methodology
**Status:** Problem identified, solution needed

---

## The Core Problem

Stage 0 research completes too quickly and superficially for complex plugins. It produces architecture.md files that look complete but lack the depth needed for successful implementation.

**Root cause:** Stage 0 doesn't have a **meta-research phase** - it doesn't ask "What questions do I need to ask?" before jumping to "What are the answers?"

---

## Current vs Required Behavior

### Current (Shallow) Process

```
1. Read creative brief
2. "Search for JUCE classes" → List common classes
3. "Research professional plugins" → Quick web search
4. "Research parameter ranges" → Generic ranges
5. Fill template → Done
```

**Time:** 5-10 minutes
**Success rate:** 100% for simple plugins (Tier 1-2), fails for complex plugins (Tier 5+)

### Required (Deep) Process

```
1. Read creative brief
2. META-RESEARCH: What features exist? What questions need answering?
   └─ For each feature:
      2.1. What is this algorithmically?
      2.2. How do professionals implement this?
      2.3. What are the building blocks?
      2.4. What JUCE APIs exist for this?
      2.5. What are the risks/alternatives?
3. ITERATE over each feature with deep research
4. VALIDATE feasibility and integration
5. Document comprehensive architecture
```

**Time:** 10-30 minutes (graduated by complexity)
**Success rate:** Target 90%+ for all plugin tiers

---

## The "What Questions Do I Need to Ask?" Problem

### Example: LushVerb (Shimmer Reverb)

**Shallow approach (current):**
- Creative brief says "shimmer reverb"
- Search: "juce reverb" → Find `juce::dsp::Reverb`
- Done ✓

**Problem:** Shimmer isn't just reverb. The research skipped:
- What IS shimmer reverb algorithmically?
- How do you pitch-shift audio? (FFT? Granular? Delay-based?)
- Does JUCE have pitch shifting?
- How do professionals implement shimmer?

**Deep approach (needed):**

**Meta-research questions:**
1. What is shimmer reverb? (Algorithmic understanding)
2. How is it different from standard reverb?
3. What components are needed? (Decomposition)

**Research answers:**
1. Shimmer = pitch shifting (+1 octave) + reverb + feedback
2. Pitch shifting options: Phase vocoder (FFT), granular, delay-based
3. Professional implementations (Strymon BigSky, Valhalla Shimmer) use phase vocoders
4. Components needed:
   - FFT engine → JUCE has `juce::dsp::FFT`
   - Reverb engine → JUCE has `juce::dsp::Reverb`
   - Feedback/mixing → JUCE has `juce::dsp::DryWetMixer`
5. Risks: Phase vocoder is complex (HIGH risk), fallback = granular

**Outcome:** Comprehensive architecture.md that guides implementation

---

## Example: DrumRoulette (Complex Multi-Feature Plugin)

**Shallow approach (fails):**
- Creative brief says "drum sampler with folder randomization"
- Search: "juce sampler" → Find `juce::Synthesiser`
- Done ✗

**Problem:** Research missed critical components:
- How do you load audio files?
- How do you scan folders recursively?
- How do you implement randomization?
- How do you create 18 audio outputs?
- How do you handle "lock" buttons (non-parameter UI)?

**Deep approach (needed):**

**Meta-research - identify features:**
1. Sample playback
2. File loading and management
3. Folder scanning and randomization
4. Multi-output routing (18 outputs)
5. MIDI routing (8 slots)
6. UI-triggered actions (randomize, lock buttons)
7. State persistence (folder paths, lock states)

**For EACH feature, deep research:**

**Feature: Folder Scanning & Randomization**

**Meta-questions:**
- What is folder randomization algorithmically?
- How do file systems work in JUCE?
- How do I traverse directories?
- How do I select random files?

**Research:**
- Algorithm: Recursive directory traversal + array of files + random selection
- JUCE API: `juce::File::findChildFiles()` (recursive scan)
- JUCE API: `juce::Random::nextInt()` for random selection
- Implementation pattern: Cache file list, randomize index
- Thread safety: File I/O on background thread, not audio thread
- Edge cases: Empty folders, missing folders on reload, large folders (>1000 files)

**Feature: Multi-Output Routing**

**Meta-questions:**
- How do multi-output plugins work in JUCE?
- What is bus configuration?
- How many outputs can a plugin have?
- Do all DAWs support 18 outputs?

**Research:**
- JUCE API: `BusesProperties` in AudioProcessor constructor
- Configuration: Main stereo (2) + 8 individual stereos (16) = 18 outputs
- Pattern: Check juce8-critical-patterns.md for bus configuration
- Risk: DAW compatibility (HIGH risk - needs testing)
- Fallback: Reduce to main output only if 18 outputs unsupported

**Feature: UI-Triggered Actions (Randomize Button)**

**Meta-questions:**
- How do non-parameter buttons work in JUCE?
- How does UI communicate with processor?
- How do you trigger actions from WebView?

**Research:**
- Pattern: Editor calls processor method directly (not via APVTS)
- JUCE API: `processor.randomizeSlot(index)` called from editor
- WebView integration: JavaScript → C++ relay → processor method
- Thread safety: Actions on message thread, not audio thread
- State: No APVTS parameter needed (action, not state)

**Outcome:** Comprehensive architecture.md covering ALL systems, not just DSP

---

## The Graduated Complexity Problem

**Current:** Stage 0 takes same time for all plugins (5-10 minutes)

**Problem:** GainKnob (1 parameter, trivial) gets same research depth as DrumRoulette (81 parameters, file I/O, multi-output)

**Required:** Research depth scales with complexity

| Tier | Example | Features | Research Time | Depth |
|------|---------|----------|---------------|-------|
| 1 | GainKnob | 1-3 params, simple DSP | 5 min | Quick verification |
| 2 | TapeAge | 4-7 params, standard DSP | 10 min | Moderate research |
| 3 | LushVerb | Complex DSP (shimmer) | 15 min | Deep algorithm research |
| 4 | OrganicHats | Synthesizer | 20 min | MIDI + DSP research |
| 5 | DrumRoulette | File I/O + multi-output | 30 min | Multi-system research |
| 6 | Spectrum Analyzer | Analysis/visualization | 30 min | Analysis pattern research |

**Implementation:** Complexity detected in Step 1 → Adjusts research depth for Step 3

---

## What's Missing from Current Step 3

### Current Step 3 (stage-0-research.md line 65):

```markdown
### 3. Research JUCE DSP Modules

**Search for relevant juce::dsp classes:**

Use WebSearch to find JUCE documentation for:
- Audio processing (juce::dsp namespace)
- DSP utilities (filters, delays, reverb, etc.)

**Document findings:**
- Which JUCE classes match the plugin needs?
- Are any custom algorithms required?
```

### Problems:

1. **No feature identification** - Doesn't extract features from creative brief first
2. **No algorithmic research** - Jumps straight to JUCE classes
3. **No decomposition** - Doesn't break complex features into primitives
4. **Uses WebSearch** - Unreliable, outdated (JUCE 6 vs JUCE 8)
5. **No iteration** - Single pass, not per-feature research
6. **No validation** - Doesn't check critical patterns or feasibility
7. **No depth graduation** - Same process for simple and complex plugins

---

## Required Research Methodology

### New Step 3: Deep Architecture Research (Graduated Complexity)

**3.0. Detect Complexity Tier**
- Analyze creative brief for complexity indicators
- Set research depth: QUICK (Tier 1-2), MODERATE (Tier 3-4), DEEP (Tier 5-6)

**3.1. Feature Identification (META-RESEARCH)**
```xml
<meta_research>
  <question>What features/components exist in this plugin?</question>
  <process>
    - Extract from creative brief
    - Identify DSP features (reverb, saturation, filtering)
    - Identify non-DSP features (file I/O, multi-output, MIDI routing)
    - Identify UI features (parameter controls, action buttons, visualization)
  </process>
  <output>List of features requiring research</output>
</meta_research>
```

**3.2. Per-Feature Deep Research (ITERATE)**

For each feature identified in 3.1:

```xml
<feature name="[FeatureName]">

  <algorithmic_understanding use_tool="extended-thinking">
    <questions>
      - What is this feature conceptually?
      - How is it implemented algorithmically?
      - What are the mathematical/programming primitives?
    </questions>
    <sources>
      - DSP textbooks, academic papers
      - Algorithm explanations
    </sources>
  </algorithmic_understanding>

  <professional_research use_tool="WebSearch">
    <questions>
      - How do industry plugins implement this?
      - What approaches do they use?
      - What are quality vs performance tradeoffs?
    </questions>
    <sources>
      - Strymon, Valhalla, FabFilter, Waves, UAD
      - Product manuals, demos, reviews
    </sources>
  </professional_research>

  <primitive_decomposition use_tool="extended-thinking">
    <questions>
      - What are the building blocks?
      - What primitives are needed?
    </questions>
    <output>
      - List of primitives (FFT, filters, file I/O, etc.)
    </output>
  </primitive_decomposition>

  <juce_api_mapping use_tool="Context7-MCP">
    <for_each primitive>
      <query>Search JUCE documentation for [primitive]</query>
      <verify>
        - Does class exist in JUCE 8?
        - What is the API signature?
        - What module is required?
      </verify>
      <document>
        - JUCE class name
        - Module dependency
        - API usage pattern
        - OR: "No JUCE class, need custom implementation"
      </document>
    </for_each>
  </juce_api_mapping>

  <validation>
    <check_critical_patterns use_tool="Read">
      - Read troubleshooting/patterns/juce8-critical-patterns.md
      - Search for each JUCE class mentioned
      - Document gotchas, module dependencies, CMake requirements
    </check_critical_patterns>

    <feasibility_assessment use_tool="extended-thinking">
      - Is this approach implementable?
      - What is the complexity (LOW/MEDIUM/HIGH)?
      - What are the risks?
      - What are alternative approaches?
      - What is the fallback if this fails?
    </feasibility_assessment>
  </validation>

  <documentation>
    - Write findings to architecture.md
    - Include algorithmic explanation
    - Include JUCE class mappings
    - Include risks and alternatives
    - Include implementation notes
  </documentation>

</feature>
```

**3.3. Integration Analysis**
```xml
<integration_analysis use_tool="extended-thinking">
  <questions>
    - How do these features integrate?
    - Are there dependencies between features?
    - Are there parameter interactions?
    - Are there processing order requirements?
  </questions>
  <output>Processing chain diagram with integration notes</output>
</integration_analysis>
```

**3.4. Comprehensive Documentation**
- Compile all research into architecture.md
- Use enhanced template with sections for:
  - System Architecture (non-DSP components)
  - Integration Points
  - Implementation Risks (HIGH/MEDIUM/LOW with fallbacks)
  - Architecture Decisions (why this approach vs alternatives)

---

## Why Current Process Fails for Complex Plugins

### Case Study: DrumRoulette (if Stage 0 ran today)

**Current shallow process would produce:**

```markdown
# DSP Architecture: DrumRoulette

## Core Components

### Sample Playback
- **JUCE Class:** `juce::Synthesiser`
- **Purpose:** Play samples on MIDI input

### Envelope
- **JUCE Class:** `juce::ADSR`
- **Purpose:** Shape sample playback
```

**What's missing:**
- How does file loading work? (No research)
- How does folder scanning work? (No research)
- How does randomization work? (No research)
- How do 18 outputs work? (No research)
- How do lock buttons work? (No research)
- How is state persisted? (No research)

**Result:** Stage 2 (Foundation) would fail - no bus configuration documented
**Result:** Stage 3 (Shell) would fail - no guidance on file path storage
**Result:** Stage 4 (DSP) would fail - no sample loading architecture documented

---

## Success Criteria for Redesigned Stage 0

1. **Graduated complexity** - Simple plugins quick (5 min), complex plugins thorough (30 min)
2. **Meta-research phase** - Identifies all features requiring research before researching
3. **Deep per-feature research** - Algorithmic understanding → JUCE mapping → Validation
4. **Authoritative sources** - Context7 MCP for JUCE (not WebSearch), critical patterns for gotchas
5. **Comprehensive documentation** - architecture.md guides implementation for ALL plugin systems
6. **Risk assessment** - HIGH/MEDIUM/LOW ratings with fallback architectures
7. **Reasoning documented** - WHY this approach was chosen, alternatives considered

**Target:** 90%+ success rate for Tier 5-6 plugins (vs current ~0%)

---

## Next Steps

1. **Redesign step 3 in stage-0-research.md** with graduated complexity and deep research methodology
2. **Enhance architecture-template.md** with new sections (System Architecture, Integration Points, Implementation Risks, Architecture Decisions)
3. **Create structured prompting** using XML tags for deep research per feature
4. **Integrate tools properly:**
   - Extended thinking for algorithmic research
   - Context7 MCP for JUCE API verification
   - Read for critical patterns validation
   - WebSearch for professional plugin research
5. **Test with DrumRoulette** - Run new Stage 0 and verify comprehensive architecture.md

---

## Key Insight

> **"Before you can answer questions, you must ask: What questions should I be asking?"**

Stage 0 isn't just about finding JUCE classes. It's about:
1. Understanding what needs to be built (meta-research)
2. Understanding how to build it (algorithmic research)
3. Understanding what tools exist (JUCE API mapping)
4. Understanding what could go wrong (validation)
5. Documenting all of the above (comprehensive architecture)

The current process skips steps 1, 2, and 4 entirely.
