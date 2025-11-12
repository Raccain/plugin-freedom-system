---
name: deep-research
description: Multi-agent parallel investigation for complex JUCE problems
model: claude-opus-4-1-20250805
allowed-tools:
  - Read # Local troubleshooting docs
  - Grep # Search docs
  - Task # Spawn parallel research subagents (Level 3)
  - WebSearch # Web research (Level 2-3)
preconditions:
  - Problem is non-trivial (quick Context7 lookup insufficient)
extended-thinking: false # Level 1-2, true for Level 3 (15k budget)
timeout: 3600 # 60 min max
---

# deep-research Skill

**Purpose:** Multi-level autonomous investigation for complex JUCE plugin development problems using graduated research depth protocol.

## Overview

This skill performs thorough research on technical problems by consulting multiple knowledge sources with automatic escalation based on confidence. Starts fast (5 min local lookup) and escalates as needed (up to 60 min parallel investigation).

**Why graduated protocol matters:**

Most problems have known solutions (Level 1: 5 min). Complex problems need deep research (Level 3: 60 min). Graduated protocol prevents over-researching simple problems while ensuring complex problems get thorough investigation.

**Key innovation:** Stops as soon as confident answer found. User controls depth via decision menus.

---

## Read-Only Protocol

**CRITICAL:** This skill is **read-only** and **advisory only**. It NEVER:

- Edits code files
- Runs builds
- Modifies contracts or configurations
- Implements solutions

**Workflow handoff:**

1. Research completes and presents findings with decision menu
2. User selects "Apply solution"
3. Output routing instruction: "User selected: Apply solution. Invoking plugin-improve skill..."
4. This signals main conversation to invoke plugin-improve
5. plugin-improve reads research findings from conversation history
6. plugin-improve handles all implementation (with versioning, backups, testing)

**Why separation matters:**

- Research uses Opus + extended thinking (expensive)
- Implementation needs codebase context (different focus)
- Clear decision gate between "here are options" and "making changes"
- Research can't break anything (safe exploration)

---

## Entry Points

**Invoked by:**

- troubleshooter agent (Level 4 investigation)
- User manual: `/research [topic]`
- build-automation "Investigate" option
- Natural language: "research [topic]", "investigate [problem]"

**Entry parameters:**

- **Topic/question**: What to research
- **Context** (optional): Plugin name, stage, error message
- **Starting level** (optional): Skip to Level 2/3 if explicitly requested

---

## Level 1: Quick Check (5-10 min, Sonnet, no extended thinking)

**Goal:** Find solution in local knowledge base or basic JUCE docs

### Process

1. **Parse research topic/question**

   - Extract keywords
   - Identify JUCE components involved
   - Classify problem type (build, runtime, API usage, etc.)

2. **Search local troubleshooting docs:**

   ```bash
   # Search by keywords
   grep -r "[topic keywords]" troubleshooting/

   # Check relevant categories
   ls troubleshooting/by-symptom/[category]/

   # Search both paths (by-plugin and by-symptom)
   ```

3. **Check Context7 JUCE docs for direct answers:**

   - Query for relevant class/API
   - Check method signatures
   - Review basic usage examples
   - Look for known limitations

4. **Assess confidence:**

   - **HIGH:** Exact match in local docs OR clear API documentation
   - **MEDIUM:** Similar issue found but needs adaptation
   - **LOW:** No relevant matches or unclear documentation

5. **Decision point:**

   **If HIGH confidence:**

   ```
   ✓ Level 1 complete (found solution)

   Solution: [Brief description]
   Source: [Local doc or JUCE API]

   What's next?
   1. Apply solution (recommended)
   2. Review details - See full explanation
   3. Continue deeper - Escalate to Level 2 for more options
   4. Other
   ```

   **If MEDIUM/LOW confidence:**
   Auto-escalate to Level 2 with notification:

   ```
   Level 1: No confident solution found
   Escalating to Level 2 (moderate investigation)...
   ```

---

## Level 2: Moderate Investigation (15-30 min, Sonnet, no extended thinking)

**Goal:** Find authoritative answer through comprehensive documentation and community knowledge

### Process

1. **Context7 deep-dive:**

   - Full module documentation
   - Code examples and patterns
   - Related classes and methods
   - Best practices sections
   - Migration guides (if version-related)

2. **JUCE forum search via WebSearch:**

   ```
   Query: "site:forum.juce.com [topic]"

   Look for:
   - Recent threads (last 2 years)
   - Accepted solutions
   - JUCE team responses
   - Multiple user confirmations
   ```

3. **GitHub issue search:**

   ```
   Query: "site:github.com/juce-framework/JUCE [topic]"

   Look for:
   - Closed issues with solutions
   - Pull requests addressing problem
   - Code examples in discussions
   - Version-specific fixes
   ```

4. **Synthesize findings:**

   - Compare multiple sources
   - Verify version compatibility (JUCE 8.x)
   - Assess solution quality (proper fix vs workaround)
   - Identify common recommendations

5. **Decision point:**

   **If HIGH/MEDIUM confidence:**

   ```
   ✓ Level 2 complete (found N solutions)

   Investigated sources:
   - Context7 JUCE docs: [Finding]
   - JUCE forum: [N threads]
   - GitHub: [N issues]

   Solutions found:
   1. [Solution 1] (recommended)
   2. [Solution 2] (alternative)

   What's next?
   1. Apply recommended solution
   2. Review all options - Compare approaches
   3. Continue deeper - Escalate to Level 3 (parallel investigation)
   4. Other
   ```

   **If LOW confidence OR novel problem:**
   Auto-escalate to Level 3 with notification:

   ```
   Level 2: Complex problem detected (requires original implementation)
   Escalating to Level 3 (deep research with parallel investigation)...

   Switching to Opus model + extended thinking (may take up to 60 min)
   ```

---

## Level 3: Deep Research (30-60 min, Opus, extended thinking 15k budget)

**Goal:** Comprehensive investigation with parallel subagent research for novel/complex problems

### Process

1. **Switch to Opus model with extended thinking:**

   ```yaml
   extended_thinking: true
   thinking_budget: 15000
   timeout: 3600 # 60 min
   ```

2. **Identify research approaches:**

   Analyze problem and determine 2-3 investigation paths:

   **For DSP algorithm questions:**

   - Algorithm A investigation (e.g., BLEP anti-aliasing)
   - Algorithm B investigation (e.g., oversampling approach)
   - Commercial implementation research (industry standards)

   **For novel feature implementation:**

   - JUCE native approach (using built-in APIs)
   - External library approach (e.g., RubberBand, SoundTouch)
   - Custom implementation approach (from scratch)

   **For performance optimization:**

   - Algorithm optimization (better complexity)
   - Caching/pre-computation (trade memory for CPU)
   - SIMD/parallel processing (JUCE SIMD wrappers)

3. **Spawn parallel research subagents:**

   Use Task tool to spawn 2-3 specialized research agents in fresh contexts:

   ```typescript
   // Example: Wavetable anti-aliasing research

   Task(
     (subagent_type = "general-purpose"),
     (description = "Research BLEP anti-aliasing"),
     (prompt = `
       Investigate Band-Limited Step (BLEP) anti-aliasing for wavetable synthesis.
   
       Research:
       1. Algorithm description and theory
       2. JUCE API support (dsp::Oscillator helpers?)
       3. Implementation complexity (1-5 scale)
       4. CPU performance characteristics
       5. Audio quality assessment
       6. Code examples or references
   
       Return structured findings:
       {
         "approach": "BLEP",
         "juce_support": "description",
         "complexity": 3,
         "cpu_cost": "low",
         "quality": "high",
         "pros": ["...", "..."],
         "cons": ["...", "..."],
         "references": ["...", "..."]
       }
     `)
   );

   Task(
     (subagent_type = "general-purpose"),
     (description = "Research oversampling anti-aliasing"),
     (prompt = `
       Investigate oversampling + filtering anti-aliasing for wavetable synthesis.
   
       Research:
       1. Algorithm description (render high SR, filter, downsample)
       2. JUCE API support (juce::dsp::Oversampling class?)
       3. Implementation complexity (1-5 scale)
       4. CPU performance characteristics
       5. Audio quality assessment
       6. Code examples or references
   
       Return structured findings:
       {
         "approach": "Oversampling",
         "juce_support": "description",
         "complexity": 2,
         "cpu_cost": "medium",
         "quality": "high",
         "pros": ["...", "..."],
         "cons": ["...", "..."],
         "references": ["...", "..."]
       }
     `)
   );

   Task(
     (subagent_type = "general-purpose"),
     (description = "Research commercial implementations"),
     (prompt = `
       Research how commercial wavetable synthesizers handle anti-aliasing.
   
       Investigate:
       1. Industry standard approaches
       2. Common trade-offs (CPU vs quality)
       3. Best practices from established plugins
       4. Any public information on implementations
   
       Return structured findings:
       {
         "industry_standard": "description",
         "common_approach": "...",
         "trade_offs": "...",
         "references": ["...", "..."]
       }
     `)
   );
   ```

   **Each subagent:**

   - Runs in fresh context (no accumulated conversation)
   - Has focused research goal
   - Returns structured findings
   - Takes 10-20 minutes
   - Runs in parallel (3 agents = 20 min total, not 60 min)

4. **Main context synthesis (extended thinking):**

   After all subagents return, use extended thinking to synthesize:

   - **Aggregate findings** from all subagents
   - **Compare approaches:**
     - Pros/cons for each
     - Implementation complexity (1-5 scale)
     - CPU cost vs quality trade-offs
     - JUCE integration ease
   - **Recommend best fit:**
     - For this specific use case
     - Considering complexity budget
     - Balancing CPU and quality
   - **Implementation roadmap:**
     - Step-by-step approach
     - JUCE APIs to use
     - Potential pitfalls
     - Testing strategy

5. **Academic paper search (if applicable):**

   For DSP algorithms, search for authoritative papers:

   - "Reducing Aliasing from Synthetic Audio Signals"
   - "Digital Audio Signal Processing"
   - "Real-Time Audio Programming"

6. **Generate comprehensive report:**

   Use template from `assets/report-template.json` with populated values:

   ```json
   {
     "agent": "deep-research",
     "status": "success",
     "level": 3,
     "topic": "[research topic]",
     "findings": [
       {
         "solution": "[Approach 1]",
         "confidence": "high|medium|low",
         "description": "...",
         "pros": ["...", "..."],
         "cons": ["...", "..."],
         "implementation_complexity": 3,
         "references": ["...", "..."]
       },
       {
         "solution": "[Approach 2]",
         ...
       }
     ],
     "recommendation": 0,  // Index into findings
     "reasoning": "Why this approach is recommended...",
     "sources": [
       "Context7: ...",
       "JUCE Forum: ...",
       "Paper: ...",
       "Subagent findings: ..."
     ],
     "time_spent_minutes": 45,
     "escalation_path": [1, 2, 3]
   }
   ```

7. **Present findings:**

   ```
   ✓ Level 3 complete (comprehensive investigation)

   Investigated N approaches:
   1. [Approach 1] - Complexity: 3/5, CPU: Low, Quality: High
   2. [Approach 2] - Complexity: 2/5, CPU: Medium, Quality: High
   3. [Approach 3] - Complexity: 4/5, CPU: Low, Quality: Very High

   Recommendation: [Approach 2]
   Reasoning: [Why this is the best fit]

   What's next?
   1. Apply recommended solution (recommended)
   2. Review all findings - See detailed comparison
   3. Try alternative approach - [Approach N name]
   4. Document findings - Save to troubleshooting/
   5. Other
   ```

---

## Report Generation

All levels return structured report. Format depends on level:

### Level 1 Report (Quick)

```markdown
## Research Report: [Topic]

**Level:** 1 (Quick Check)
**Time:** 5 minutes
**Confidence:** HIGH

**Source:** troubleshooting/by-plugin/[Plugin]/[category]/[file].md

**Solution:**
[Direct answer from local docs or JUCE API]

**How to apply:**
[Step-by-step]

**Reference:** [Source file path or JUCE API link]
```

### Level 2 Report (Moderate)

```markdown
## Research Report: [Topic]

**Level:** 2 (Moderate Investigation)
**Time:** 18 minutes
**Confidence:** MEDIUM-HIGH

**Sources:**

- Context7 JUCE docs: [Finding]
- JUCE forum: 3 threads reviewed
- GitHub: 2 issues reviewed

**Solutions:**

### Option 1: [Recommended]

**Approach:** ...
**Pros:** ...
**Cons:** ...
**Implementation:** ...

### Option 2: [Alternative]

**Approach:** ...
**Pros:** ...
**Cons:** ...
**Implementation:** ...

**Recommendation:** Option 1
**Reasoning:** [Why recommended]

**References:**

- [Source 1]
- [Source 2]
```

### Level 3 Report (Comprehensive)

```markdown
## Research Report: [Topic]

**Level:** 3 (Deep Research)
**Time:** 45 minutes
**Confidence:** HIGH
**Model:** Opus + Extended Thinking

**Investigation approach:**

- Spawned 3 parallel research subagents
- Investigated 3 distinct approaches
- Synthesized findings with extended thinking

**Findings:**

### Approach 1: [Name]

**Description:** ...
**JUCE support:** juce::dsp::Oversampling
**Complexity:** 2/5
**CPU cost:** Medium
**Quality:** High
**Pros:**

- Works for all waveforms
- JUCE native API
  **Cons:**
- Higher CPU than alternatives
  **References:**
- JUCE dsp::Oversampling docs
- Subagent findings

### Approach 2: [Name]

**Description:** ...
[Same structure]

### Approach 3: [Name]

**Description:** ...
[Same structure]

**Recommendation:** Approach 1
**Reasoning:**
Oversampling is simpler to implement (complexity 2/5) and works for arbitrary waveforms.
BLEP requires waveform-specific code (complexity 3/5). For this use case, simplicity
outweighs the CPU cost difference.

**Implementation Roadmap:**

1. Add juce::dsp::Oversampling to PluginProcessor.h
2. Configure in prepareToPlay() with 2x oversampling
3. Wrap processBlock() wavetable rendering with oversampler
4. Profile CPU usage and adjust oversampling factor if needed

**Sources:**

- Context7: juce::dsp::Oversampling documentation
- Paper: Reducing Aliasing from Synthetic Audio Signals (2007)
- JUCE Forum: Wavetable synthesis best practices
- Subagent research: 3 parallel investigations

**Testing strategy:**

- Sweep test (play waveform through full frequency range)
- FFT analysis (check for aliasing harmonics above Nyquist)
- CPU profiling (ensure real-time safe)
```

---

## Decision Menus

After each level, present decision menu (user controls depth):

### After Level 1

```
What's next?
1. Apply solution (recommended) - [One-line description]
2. Review details - See full explanation
3. Continue deeper - Escalate to Level 2 for more options
4. Other
```

**Handle responses:**

- **Option 1 ("Apply solution"):**
  Output: "User selected: Apply solution. Invoking plugin-improve skill..."
  This routing instruction signals main conversation to invoke plugin-improve, which reads research findings from history and skips Phase 0.5 investigation.
- **Option 2:** Display full report, then re-present menu
- **Option 3:** Escalate to Level 2
- **Option 4:** Ask what they'd like to do

**IMPORTANT:** This skill is **read-only**. Never edit code, run builds, or modify files. All implementation happens through plugin-improve skill after research completes.

### After Level 2

```
What's next?
1. Apply recommended solution - [Solution name]
2. Review all options - Compare N approaches
3. Continue deeper - Escalate to Level 3 (may take up to 60 min)
4. Other
```

**Handle responses:**

- **Option 1 ("Apply recommended solution"):**
  Output: "User selected: Apply recommended solution. Invoking plugin-improve skill..."
  This routing instruction signals main conversation to invoke plugin-improve, which reads research findings from history.
- **Option 2:** Display detailed comparison, then re-present menu
- **Option 3:** Escalate to Level 3
- **Option 4:** Ask what they'd like to do

### After Level 3

```
What's next?
1. Apply recommended solution (recommended) - [Solution name]
2. Review all findings - See detailed comparison
3. Try alternative approach - [Alternative name]
4. Document findings - Save to troubleshooting/ knowledge base
5. Other
```

**Handle responses:**

- **Option 1 ("Apply recommended solution"):**
  Output: "User selected: Apply recommended solution. Invoking plugin-improve skill..."
  This routing instruction signals main conversation to invoke plugin-improve, which reads research findings from history and proceeds to implementation.
- **Option 2:** Display comprehensive report with all approaches, then re-present menu
- **Option 3:** User wants to try different approach - update recommendation, then output routing instruction to invoke plugin-improve with alternative approach
- **Option 4:** Invoke troubleshooting-docs skill to capture findings in knowledge base (for future Level 1 fast path)
- **Option 5:** Ask what they'd like to do

---

## Integration with troubleshooter

**troubleshooter Level 4:**

When troubleshooter agent exhausts Levels 0-3, it invokes deep-research:

```markdown
## troubleshooter.md - Level 4

If Levels 0-3 insufficient, escalate to deep-research skill:

"I need to investigate this more deeply. Invoking deep-research skill..."

[Invoke deep-research with problem context]

deep-research handles:

- Graduated research protocol (Levels 1-3)
- Parallel investigation (Level 3)
- Extended thinking budget
- Returns structured report with recommendations
```

**Integration flow:**

1. troubleshooter detects complex problem (Level 3 insufficient)
2. Invokes deep-research skill
3. deep-research starts at Level 1 (may escalate)
4. Returns structured report to troubleshooter
5. troubleshooter presents findings to user

---

## Integration with troubleshooting-docs

After successful application of Level 2-3 findings:

**Auto-suggest documentation:**

```
✓ Solution applied successfully

This was a complex problem (Level N research). Document for future reference?

1. Yes - Create troubleshooting doc (recommended)
2. No - Skip documentation
3. Other
```

If user chooses "Yes":

- Invoke troubleshooting-docs skill
- Pass research report + solution as context
- Creates dual-indexed documentation
- Future Level 1 searches will find it instantly

**The feedback loop:**

1. Level 3 research solves complex problem (45 min)
2. Document solution → troubleshooting/
3. Similar problem occurs → Level 1 finds it (5 min)
4. Knowledge compounds, research gets faster over time

---

## Success Criteria

Research is successful when:

- ✅ Appropriate level reached (no over-research)
- ✅ Confidence level clearly stated
- ✅ Solution(s) provided with pros/cons
- ✅ Implementation steps included
- ✅ References cited
- ✅ User controls depth via decision menus

**Level-specific:**

**Level 1:**

- ✅ Local docs or JUCE API checked
- ✅ HIGH confidence OR escalated

**Level 2:**

- ✅ Context7, forum, GitHub searched
- ✅ Multiple sources compared
- ✅ MEDIUM-HIGH confidence OR escalated

**Level 3:**

- ✅ Parallel subagents spawned (2-3)
- ✅ Extended thinking used for synthesis
- ✅ Comprehensive report generated
- ✅ Clear recommendation with rationale

---

## Error Handling

**Timeout (>60 min):**

```
⚠️ Research timeout (60 min limit)

Returning best findings based on investigation so far:
[Partial findings]

What's next?
1. Use current findings - Proceed with partial answer
2. Retry with extended timeout - Continue research (80 min)
3. Other
```

**No solution found:**

```
Research exhausted (Level 3 complete, no definitive solution)

Attempted:
✓ Local troubleshooting docs (0 matches)
✓ Context7 JUCE documentation (API exists but undocumented)
✓ JUCE forum + GitHub (no clear consensus)
✓ Parallel investigation (3 approaches, all have significant drawbacks)

Recommendations:
1. Post to JUCE forum with detailed investigation
2. Try experimental approach: [Suggestion]
3. Consider alternative feature: [Workaround]

I can help formulate forum post if needed.
```

**Subagent failure (Level 3):**

```
⚠️ Subagent [N] failed to complete research

Proceeding with findings from other subagents (N-1 completed)...
```

Continue with available findings, note partial investigation in report.

**WebSearch unavailable:**
Level 2 proceeds with Context7 only:

```
⚠️ Web search unavailable

Proceeding with Context7 JUCE documentation only.
Results may be incomplete for community knowledge.
```

---

## Notes for Claude

**When executing this skill:**

1. Always start at Level 1 (never skip directly to Level 3)
2. Stop as soon as HIGH confidence achieved (don't over-research)
3. Switch to Opus + extended thinking at Level 3 (architecture requirement)
4. Spawn parallel subagents at Level 3 (2-3 concurrent, not serial)
5. Use relative confidence (HIGH/MEDIUM/LOW based on sources)
6. Present decision menus (user controls escalation)
7. Integration with troubleshooting-docs (capture complex solutions)
8. Timeout at 60 min (return best findings)

**Common pitfalls:**

- Skipping Level 1 local docs check (always fastest path)
- Serial subagent invocation at Level 3 (use parallel Task calls)
- Using Sonnet for Level 3 (must switch to Opus)
- Forgetting extended thinking at Level 3 (critical for synthesis)
- Over-researching simple problems (stop at HIGH confidence)
- Not presenting decision menus (user should control depth)

---

## Performance Budgets

**Level 1:**

- Time: 5-10 min
- Extended thinking: No
- Success rate: 40% of problems (known solutions)

**Level 2:**

- Time: 15-30 min
- Extended thinking: No
- Success rate: 40% of problems (documented solutions)

**Level 3:**

- Time: 30-60 min
- Extended thinking: Yes (15k budget)
- Success rate: 20% of problems (novel/complex)

**Overall:**

- Average resolution time: 15 min (weighted by success rates)
- 80% of problems solved at Level 1-2 (fast)
- 20% require Level 3 (deep research justified)

---

## Example Scenarios

### Scenario 1: Known Problem (Level 1, 5 min)

**Topic:** "Parameter not saving in DAW"

**Level 1:**

- Search troubleshooting/by-symptom/parameter-issues/
- Found: parameter-not-saving-decay-ReverbPlugin-20251110.md
- HIGH confidence

**Output:**

```
✓ Level 1 complete (found solution)

Source: Local troubleshooting docs

Problem: APVTS state not reflecting UI changes
Solution: Implement parameterValueChanged() to update processor state

[Full solution from doc]

What's next?
1. Apply solution (recommended)
2. Review details
3. Continue deeper
4. Other
```

**Resolution:** 5 minutes, HIGH confidence

### Scenario 2: JUCE API Question (Level 2, 18 min)

**Topic:** "How to use juce::dsp::Compressor?"

**Level 1:**

- Search troubleshooting/ → No matches
- Quick Context7 lookup → Found class, but need usage details
- MEDIUM confidence → Escalate to Level 2

**Level 2:**

- Context7 deep-dive → Full API docs, prepare() and process() methods
- JUCE forum search → Found example thread with code
- GitHub search → Found working implementation in examples
- HIGH confidence

**Output:**

```
✓ Level 2 complete (found solution)

Sources:
- Context7: juce::dsp::Compressor API docs
- JUCE forum: Usage example with threshold/ratio settings
- GitHub: JUCE examples repository

Solution: [Step-by-step implementation]

What's next?
1. Apply solution (recommended)
2. Review all sources
3. Continue deeper (not necessary, HIGH confidence)
4. Other
```

**Resolution:** 18 minutes, HIGH confidence

### Scenario 3: Novel Implementation (Level 3, 45 min)

**Topic:** "Implement wavetable anti-aliasing"

**Level 1:**

- Search troubleshooting/ → No matches
- Context7 → Found dsp::Oscillator but no anti-aliasing docs
- LOW confidence → Escalate to Level 2

**Level 2:**

- Context7 deep-dive → No built-in anti-aliasing
- JUCE forum → Multiple approaches mentioned, no consensus
- GitHub → Related issues but no clear solution
- MEDIUM confidence, novel implementation → Escalate to Level 3

**Level 3:**

- Switch to Opus + extended thinking
- Spawn 3 parallel subagents:
  - Subagent 1: Research BLEP
  - Subagent 2: Research oversampling
  - Subagent 3: Research commercial implementations
- Synthesize findings (extended thinking)
- Generate comprehensive report

**Output:**

```
✓ Level 3 complete (comprehensive investigation)

Investigated 3 approaches:
1. BLEP - Complexity: 3/5, CPU: Low, Quality: High
2. Oversampling - Complexity: 2/5, CPU: Medium, Quality: High
3. Minblep - Complexity: 4/5, CPU: Low, Quality: Very High

Recommendation: Oversampling
Reasoning: Simplest implementation, works for all waveforms, JUCE native API

[Comprehensive report with implementation roadmap]

What's next?
1. Apply recommended solution (recommended)
2. Review all findings
3. Try alternative (BLEP)
4. Document findings
5. Other
```

**Resolution:** 45 minutes, HIGH confidence, comprehensive investigation

---

## Future Enhancements

**Not in Phase 7 scope, but potential:**

- Learning from research patterns (which sources most useful)
- Caching Context7 queries (avoid repeated lookups)
- Confidence calibration (track prediction accuracy)
- Custom subagent types (specialized researchers)
- Integration with external knowledge bases (Stack Overflow, papers)
