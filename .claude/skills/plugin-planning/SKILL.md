---
name: plugin-planning
description: Interactive research and planning (Stages 0-1) for JUCE plugin development
allowed-tools:
  - Read # For contracts and references
  - Write # For architecture.md, plan.md
  - Edit # For state updates (PLUGINS.md, .continue-here.md)
  - Bash # For git commits, precondition checks
  - WebSearch # For professional plugin research
  - Grep # For searching existing implementations
  - Glob # For finding reference files
preconditions:
  - creative-brief.md must exist in plugins/[Name]/.ideas/
  - Plugin must NOT already be past Stage 1
---

# plugin-planning Skill

**Purpose:** Handle Stages 0-1 (Research and Planning) through interactive contract creation without subagents. This skill creates the foundation contracts (architecture.md, plan.md) that guide implementation.

**Invoked by:** `/plan` command (to be created) or as first step of `/implement` workflow

---

## Entry Point

**Check preconditions first:**

<precondition_gate>
<validation_requirement>
MUST verify all preconditions before proceeding. If ANY check fails, BLOCK and report to user.
</validation_requirement>

1. Verify creative brief exists:
```bash
if [ ! -f "plugins/${PLUGIN_NAME}/.ideas/creative-brief.md" ]; then
    echo "✗ creative-brief.md not found - SKILL BLOCKED"
    cat assets/precondition-failed.md
    exit 1
fi
```

2. Check plugin status in PLUGINS.md:
```bash
STATUS=$(grep -A 2 "^### ${PLUGIN_NAME}$" PLUGINS.md | grep "Status:" | awk '{print $2}')
```

<blocking_condition>
IF status is 🚧 Stage N where N >= 2:
  THEN BLOCK with message "Plugin already past planning stage"
ELSE IF status is 💡 Ideated or not found:
  THEN Proceed
</blocking_condition>
</precondition_gate>

3. Check for existing contracts:
```bash
# Check what already exists
test -f "plugins/${PLUGIN_NAME}/.ideas/architecture.md" && echo "✓ architecture.md exists"
test -f "plugins/${PLUGIN_NAME}/.ideas/plan.md" && echo "✓ plan.md exists"
```

**Resume logic:**

<resume_logic>
IF architecture.md exists AND plan.md missing:
  THEN skip to Stage 1
ELSE IF architecture.md exists AND plan.md exists:
  THEN ask user: "Both contracts exist. Regenerate or proceed to implementation?"
ELSE:
  THEN start at Stage 0
</resume_logic>

---

## Stage 0: Research

<critical_sequence id="stage-0-research">
<sequence_requirement>
Execute steps 1-6 in order. Do NOT skip steps. Each step builds on previous.
</sequence_requirement>

**Goal:** Create DSP architecture specification (architecture.md)

**Duration:** 5-10 minutes

<stage_0_enforcement id="research-step-sequence" enforcement_level="STRICT">
**Purpose:** Ensure all 6 research steps execute in order before creating architecture.md.

**Why critical:** Skipping research steps creates incomplete architecture contracts that cause implementation failures in Stages 2-5.

**Step Sequence (MANDATORY):**

<step number="1" name="Read Creative Brief" blocking="true">
**Action:**
```bash
BRIEF_PATH="plugins/${PLUGIN_NAME}/.ideas/creative-brief.md"
if [ ! -f "$BRIEF_PATH" ]; then
  echo "✗ BLOCKED: creative-brief.md not found"
  exit 1
fi

BRIEF_CONTENT=$(cat "$BRIEF_PATH")
```

**Verification:**
- File exists: `[ -f "$BRIEF_PATH" ]`
- Content loaded: `[ -n "$BRIEF_CONTENT" ]`
- Contains plugin name: `grep -q "$PLUGIN_NAME" "$BRIEF_PATH"`

**If verification fails:** BLOCK with error, cannot proceed to step 2
</step>

<step number="2" name="Identify Technical Approach" blocking="true" depends_on="1">
**Action:**
- Analyze brief to determine plugin type (effect/synth/MIDI/utility)
- Identify I/O configuration (mono/stereo/sidechain)
- Determine processing domain (time/frequency/granular)

**Verification:**
- Plugin type identified and documented
- I/O configuration determined
- Processing approach selected

**Outputs required for step 3:**
- `PLUGIN_TYPE` variable set (effect|synth|midi|utility)
- `IO_CONFIG` variable set (mono|stereo|sidechain|multi)
- `PROCESSING_DOMAIN` variable set (time|frequency|granular|hybrid)

**If verification fails:** BLOCK, present error:
```
✗ Step 2 incomplete: Technical approach not identified

Required outputs:
- Plugin type: [not set]
- I/O config: [not set]
- Processing domain: [not set]

Cannot proceed to JUCE research without technical approach.
```
</step>

<step number="3" name="Research JUCE DSP Modules" blocking="true" depends_on="2">
**Action:**
- Search JUCE documentation for relevant dsp:: classes
- Use WebSearch for JUCE module examples
- Document specific classes (minimum 2 required)

**Verification:**
- At least 2 JUCE classes identified
- Classes relevant to plugin type from step 2
- Class usage documented (what each does)

**Outputs required for step 6:**
- List of JUCE classes with descriptions
- Example: `["juce::dsp::Gain - Volume control", "juce::dsp::IIR::Filter - Filtering"]`

**If verification fails:** BLOCK, present error with suggestion:
```
✗ Step 3 incomplete: Insufficient JUCE research

Found: [N] classes (minimum 2 required)
Plugin type: ${PLUGIN_TYPE}

Suggested search: "JUCE dsp ${PLUGIN_TYPE} modules"

Cannot proceed without identifying DSP components.
```
</step>

<step number="4" name="Research Professional Plugins" blocking="true" depends_on="3">
**Action:**
- WebSearch for industry plugins (FabFilter, Waves, UAD, etc.)
- Document 3-5 similar professional plugins
- Note sonic characteristics and parameter ranges

**Verification:**
- At least 3 professional plugins documented
- Each has parameter range examples
- Sonic characteristics noted

**Outputs required for step 5:**
- List of professional plugins with metadata
- Parameter range examples for each

**If verification fails:** BLOCK, present error:
```
✗ Step 4 incomplete: Insufficient professional plugin research

Found: [N] plugins (minimum 3 required)

Cannot proceed without industry standard references.
```
</step>

<step number="5" name="Research Parameter Ranges" blocking="true" depends_on="4">
**Action:**
- Extract industry-standard ranges from step 4 research
- Document typical defaults
- Identify skew factors for nonlinear ranges

**Verification:**
- Parameter ranges documented for all parameters in creative brief
- Defaults specified
- Skew factors noted where applicable

**Outputs required for step 6:**
- Parameter range table with min/max/default/skew

**If verification fails:** BLOCK, present error:
```
✗ Step 5 incomplete: Parameter ranges not documented

Creative brief defines [N] parameters
Ranges documented: [M] parameters

Cannot create architecture.md without complete parameter specifications.
```
</step>

<step number="6" name="Design Sync Check" blocking="true" depends_on="5">
**Action:**
```bash
MOCKUP_DIR="plugins/${PLUGIN_NAME}/.ideas/mockups"
if [ -d "$MOCKUP_DIR" ] && [ -n "$(ls -A $MOCKUP_DIR 2>/dev/null)" ]; then
  echo "Mockup exists - running design-sync validation"
  # Invoke design-sync skill
  # Compare mockup parameters with creative brief
  # Document sync results
else
  echo "No mockup found - skipping design sync"
fi
```

**Verification:**
- If mockup exists: design-sync completed (PASS or user-acknowledged drift)
- If no mockup: step marked as skipped (valid)

**Outputs:**
- Design sync result (passed|skipped|drift-acknowledged)

**If verification fails (mockup exists but sync failed):** BLOCK, present error:
```
✗ Step 6 incomplete: Design-brief drift detected

[Design-sync findings]

Cannot proceed to architecture.md creation with unresolved drift.
Resolve conflicts first, then re-run Stage 0.
```
</step>

**After all 6 steps verified:**

<final_verification>
Verify all required outputs collected:
- ✓ Creative brief analyzed
- ✓ Technical approach identified (type/IO/domain)
- ✓ JUCE classes researched (≥2)
- ✓ Professional plugins researched (≥3)
- ✓ Parameter ranges documented (all params)
- ✓ Design sync completed/skipped

If all verified → Proceed to architecture.md creation
If any missing → BLOCK and show which step failed verification
</final_verification>

**Anti-pattern:**

<anti_pattern severity="CRITICAL">
❌ NEVER jump directly from step 1 to architecture.md creation
❌ NEVER skip research steps 2-5
✓ ALWAYS execute all 6 steps sequentially with verification
</anti_pattern>
</stage_0_enforcement>

**Process:**

1. Read creative brief:
```bash
cat plugins/${PLUGIN_NAME}/.ideas/creative-brief.md
```

2. Identify plugin technical approach:
   - Audio effect, MIDI effect, synthesizer, or utility?
   - Input/output configuration (mono, stereo, sidechain)
   - Processing type (time-domain, frequency-domain, granular)

3. Research JUCE DSP modules:
   - Search for relevant juce::dsp classes for the identified plugin type
   - Use WebSearch for JUCE documentation and examples
   - Document specific classes (e.g., juce::dsp::Gain, juce::dsp::IIR::Filter)

4. Research professional plugin examples:
   - Search web for industry leaders (FabFilter, Waves, UAD, etc.)
   - Document 3-5 similar plugins
   - Note sonic characteristics and typical parameter ranges

5. Research parameter ranges:
   - Industry-standard ranges for plugin type
   - Typical defaults (reference professional plugins)
   - Skew factors for nonlinear ranges

6. Check design sync (if mockup exists):
   - Look for `plugins/${PLUGIN_NAME}/.ideas/mockups/v*-ui.yaml`
   - If exists: Compare mockup parameters with creative brief
   - If conflicts found: Invoke design-sync skill to resolve
   - Document sync results

**Output:**

Create `plugins/${PLUGIN_NAME}/.ideas/architecture.md` using the template from `assets/architecture-template.md`.

**Required sections:**
1. Title: `# DSP Architecture: [PluginName]`
2. Contract header (immutability statement)
3. `## Core Components` - Each DSP component with structured format
4. `## Processing Chain` - ASCII diagram showing signal flow
5. `## Parameter Mapping` - Table mapping parameter IDs to components
6. `## Algorithm Details` - Implementation approach for each algorithm
7. `## Special Considerations` - Thread safety, performance, denormals, sample rate
8. `## Research References` - Professional plugins, JUCE docs, technical resources

**State management:**

1. Create/update `.continue-here.md` at plugin root:

Create `plugins/${PLUGIN_NAME}/.continue-here.md` using template from `assets/continue-stage-0-template.md` with variables:
- `${PLUGIN_NAME}` - Plugin name
- `${TIMESTAMP}` - Current timestamp

2. Update PLUGINS.md status to `🚧 Stage 0` and add timeline entry

3. Git commit:
```bash
git add plugins/${PLUGIN_NAME}/.ideas/architecture.md plugins/${PLUGIN_NAME}/.continue-here.md PLUGINS.md
git commit -m "$(cat <<'EOF'
feat: [PluginName] Stage 0 - research complete

🤖 Generated with Claude Code

Co-Authored-By: Claude <noreply@anthropic.com>
EOF
)"
```

**Decision menu (numbered format):**

Display menu from `assets/decision-menu-stage-0.md`

<user_input_handling>
Wait for user response:
- Numeric 1-6: Execute option
- Keywords: "continue"→1, "pause"→5, "review"→2
- Other: Re-present menu
</user_input_handling>

<checkpoint_requirement>
After completing step 6, MUST commit changes and present decision menu. Do NOT auto-proceed.
</checkpoint_requirement>
</critical_sequence>

---

## Stage 1: Planning

<critical_sequence id="stage-1-planning">
<sequence_requirement>
Execute steps 1-8 in order. Do NOT skip steps. Complexity calculation must precede plan generation.
</sequence_requirement>

**Goal:** Calculate complexity and create implementation plan (plan.md)

**Duration:** 2-5 minutes

**Preconditions:**

<decision_gate id="stage-1-contract-validation">
<validation_requirement>
MUST verify parameter-spec.md AND architecture.md exist before proceeding. This is a BLOCKING gate.
</validation_requirement>

Check for required contracts:
```bash
PARAM_SPEC_EXISTS=$(test -f "plugins/${PLUGIN_NAME}/.ideas/parameter-spec.md" && echo "true" || echo "false")
ARCH_EXISTS=$(test -f "plugins/${PLUGIN_NAME}/.ideas/architecture.md" && echo "true" || echo "false")

echo "Parameter spec: ${PARAM_SPEC_EXISTS}"
echo "Architecture: ${ARCH_EXISTS}"
```

<blocking_condition>
IF PARAM_SPEC_EXISTS == "false" OR ARCH_EXISTS == "false":
  THEN display error message (see assets/stage-1-blocked.md)
  AND exit skill
ELSE:
  Proceed to complexity calculation
</blocking_condition>
</decision_gate>

Exit skill and wait for user to create contracts.

**Process (contracts confirmed present):**

1. Read all contracts:
```bash
cat plugins/${PLUGIN_NAME}/.ideas/parameter-spec.md
cat plugins/${PLUGIN_NAME}/.ideas/architecture.md
```

2. Calculate complexity score:

**Formula:**
```
score = min(param_count / 5, 2.0) + algorithm_count + feature_count
Cap at 5.0
```

**Extract metrics:**

From parameter-spec.md:
- Count parameters (each parameter definition = 1)
- param_score = min(param_count / 5, 2.0)

From architecture.md:
- Count distinct DSP algorithms/components
- algorithm_count = number of juce::dsp classes or custom algorithms

Features to identify (from architecture.md):
- Feedback loops present? (+1)
- FFT/frequency domain processing? (+1)
- Multiband processing? (+1)
- Modulation systems (LFO, envelope)? (+1)
- External MIDI control? (+1)
- feature_count = sum of above

**Display breakdown:**
```
Complexity Calculation:
- Parameters: [N] parameters ([N/5] points, capped at 2.0) = [X.X]
- Algorithms: [N] DSP components = [N]
- Features: [List features] = [N]
Total: [X.X] / 5.0
```

3. Determine implementation strategy:
   - **Simple (score ≤ 2.0):** Single-pass implementation
   - **Complex (score ≥ 3.0):** Phase-based implementation with staged commits

4. For complex plugins, create phases:

**Stage 4 (DSP) phases:**
- Phase 4.1: Core processing (essential audio path)
- Phase 4.2: Parameter modulation (APVTS integration)
- Phase 4.3: Advanced features (if applicable)

**Stage 5 (GUI) phases:**
- Phase 5.1: Layout and basic controls
- Phase 5.2: Advanced UI elements
- Phase 5.3: Polish and styling (if applicable)

Each phase needs description, test criteria, estimated duration.

**Output:**

Create `plugins/${PLUGIN_NAME}/.ideas/plan.md` using the template from `assets/plan-template.md`.

**State management:**

1. Update `.continue-here.md` at plugin root:

Update `plugins/${PLUGIN_NAME}/.continue-here.md` using template from `assets/continue-stage-1-template.md` with variables:
- `${PLUGIN_NAME}` - Plugin name
- `${TIMESTAMP}` - Current timestamp
- `${COMPLEXITY_SCORE}` - Calculated complexity score
- `${PHASED_IMPLEMENTATION}` - true/false
- `${IMPLEMENTATION_STRATEGY}` - "Single-pass" or "Phased implementation"

2. Update PLUGINS.md status to `🚧 Stage 1` and add timeline entry

3. Git commit:
```bash
git add plugins/${PLUGIN_NAME}/.ideas/plan.md plugins/${PLUGIN_NAME}/.continue-here.md PLUGINS.md
git commit -m "$(cat <<'EOF'
feat: [PluginName] Stage 1 - planning complete

Complexity: [X.X]
Strategy: [Single-pass | Phased implementation]

🤖 Generated with Claude Code

Co-Authored-By: Claude <noreply@anthropic.com>
EOF
)"
```

**Decision menu (numbered format):**

Display menu from `assets/decision-menu-stage-1.md`

<user_input_handling>
Wait for user response:
- Numeric 1-6: Execute option
- Keywords: "continue"→1, "pause"→5, "review"→2
- Other: Re-present menu
</user_input_handling>

<checkpoint_requirement>
After completing step 8, MUST commit changes and present decision menu. Do NOT auto-proceed.
</checkpoint_requirement>
</critical_sequence>

---

## Handoff to Implementation

<handoff_protocol id="planning-to-implementation">
<state_requirement>
CRITICAL: Handoff file must be updated to Stage 2. Execute steps EXACTLY in sequence.
</state_requirement>

**When user chooses to proceed to Stage 2:**

<critical_sequence>
1. Update handoff file at plugin root:
```bash
# Update existing plugins/${PLUGIN_NAME}/.continue-here.md for Stage 2
# Use template from assets/implementation-handoff-template.md
cat > plugins/${PLUGIN_NAME}/.continue-here.md <<'EOF'
[template content from assets/implementation-handoff-template.md]
EOF
```

2. Verify handoff:
```bash
test -f "plugins/${PLUGIN_NAME}/.continue-here.md" || { echo "✗ Handoff not created"; exit 1; }
echo "✓ Handoff verified"
```
</critical_sequence>

<verification_step>
After handoff, verify:
- plugins/[PluginName]/.continue-here.md exists at root
- PLUGINS.md status updated to 🚧 Stage 2
</verification_step>
</handoff_protocol>

Display to user:

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✓ Planning Complete
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Plugin: [PluginName]
Complexity: [X.X] ([Simple/Complex])
Strategy: [Single-pass | Phased implementation]

Contracts created:
✓ creative-brief.md
✓ parameter-spec.md
✓ architecture.md
✓ plan.md

Ready to build. Run: /implement [PluginName]
```

---

## Reference Files

Detailed stage implementations are in:
- `references/stage-0-research.md` - Research stage details
- `references/stage-1-planning.md` - Planning stage details

Templates are in:
- `assets/architecture-template.md` - DSP architecture contract template
- `assets/plan-template.md` - Implementation plan template
