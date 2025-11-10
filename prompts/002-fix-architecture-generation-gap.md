<objective>
Fix the architecture.md generation gap in the Plugin Freedom System by updating Stage 0 Research to output architecture.md instead of research.md. This resolves a critical workflow blocker where Stage 1 Planning requires architecture.md but no stage creates it.

This matters because: Without architecture.md, the entire plugin development workflow breaks at Stage 1, preventing implementation from proceeding.
</objective>

<context>
Project: Plugin Freedom System - A 7-stage workflow for JUCE audio plugin development
Issue: Stage 0 Research outputs research.md (research notes) but all later stages expect architecture.md (DSP specification)
Impact: 38 references across 8 documentation files need updating

The system uses immutable contracts during implementation:
- creative-brief.md (vision)
- architecture.md (DSP specification) ← Currently has no generation path
- parameter-spec.md (UI controls)
- plan.md (implementation strategy)

Review @ROADMAP.md, @architecture/, @procedures/, and @plan/phase-2-plan.md for current state.
</context>

<requirements>
1. Update Stage 0 Research to output architecture.md instead of research.md
2. Maintain backwards compatibility - Stage 0 is still called "Research" (process name), just outputs architecture.md (artifact)
3. Find and replace all 38 references from research.md to architecture.md across documentation
4. Add distinction between Stage 0 "preliminary complexity estimate" and Stage 1 "actual complexity calculation"
5. Update or create the architecture.md template with proper contract structure
6. Ensure Stage 1 Planning correctly reads only architecture.md and parameter-spec.md (not research.md)
7. Verify Stage 4 (dsp-agent) and other stages already reference architecture.md correctly
</requirements>

<implementation>
Thoroughly analyze the impact across all files before making changes. The update sequence should be:

1. **Update/Create Template** (.claude/skills/plugin-workflow/assets/architecture.md):
   - Add CONTRACT STATUS section marking it immutable during Stages 2-5
   - Include Core Components (JUCE modules), Processing Chain, Parameter Mapping, Technical Notes
   - Add Research References section to embed sources (Context7, implementation techniques)
   - This preserves research findings within the architecture document

2. **Update ROADMAP.md**:
   - Line 248: Change Stage 0 output from research.md to architecture.md
   - Lines 323-373: Rewrite format specification from research.md to architecture.md format
   - Add preliminary complexity estimation for graduated research depth:
     * Simple (1-2): Basic Context7 lookup
     * Moderate (3): Enhanced research with web search
     * Complex (4-5): Deep research with papers and comparisons

3. **Update Architecture Documents**:
   - architecture/02-core-abstractions.md: Remove research.md from contract list (4 contracts, not 5)
   - architecture/06-state-architecture.md: Update contract file listings
   - architecture/09-file-system-design.md: Update file structure diagrams
   - architecture/13-error-handling-recovery.md: Update references

4. **Update Procedure Documents**:
   - procedures/skills/plugin-workflow.md: Major changes to Stage 0 and Stage 1 specifications
   - procedures/skills/plugin-workflow/SKILL.md: Update actual implementation code
   - procedures/commands/implement.md: Update Stage 0 output documentation
   - procedures/core/checkpoint-system.md: Update handoff examples
   - procedures/skills/context-resume.md: Update context loading examples

5. **Update Phase 2 Plan** (plan/phase-2-plan.md):
   - Task 6 (Stage 0): Change all ~15 references from research.md to architecture.md
   - Task 7 (Stage 1): Remove research.md from preconditions, simplify to 2 contracts
   - Task 9 (Validator): Update to validate architecture.md instead of research.md
   - Task 13 (E2E tests): Update all file paths and verification steps

Critical distinction to add:
- Stage 0: Makes "preliminary complexity estimate" from creative-brief.md alone (determines research depth)
- Stage 1: Calculates "actual complexity score" from architecture.md + parameter-spec.md (determines phasing)
</implementation>

<validation>
After making changes, verify:
1. Run: `grep -r "research\.md" architecture/ procedures/ plan/ ROADMAP.md | wc -l` - Should return 0
2. Check Stage 0 creates architecture.md with DSP specification format
3. Verify Stage 1 reads only parameter-spec.md and architecture.md
4. Confirm Stage 4 (dsp-agent) already references architecture.md correctly
5. Test with existing TapeAge plugin that already has architecture.md manually created
6. Ensure all 38 references have been updated
</validation>

<output>
Update files in place (no new files except template if missing):
- `.claude/skills/plugin-workflow/assets/architecture.md` - Update template or create if missing
- `ROADMAP.md` - Update Stage 0 specification and format
- `architecture/02-core-abstractions.md` - Update contract list
- `architecture/06-state-architecture.md` - Update contract references
- `architecture/09-file-system-design.md` - Update file structure
- `architecture/13-error-handling-recovery.md` - Update references
- `procedures/skills/plugin-workflow.md` - Update Stage 0/1 specs
- `.claude/skills/plugin-workflow/SKILL.md` - Update implementation
- `procedures/commands/implement.md` - Update documentation
- `procedures/core/checkpoint-system.md` - Update examples
- `procedures/skills/context-resume.md` - Update context loading
- `plan/phase-2-plan.md` - Extensive updates throughout
</output>

<success_criteria>
The update is successful when:
1. Stage 0 Research generates architecture.md containing DSP specification (not research notes)
2. All 38 references to research.md have been replaced with architecture.md
3. Stage 1 Planning no longer looks for research.md and successfully reads architecture.md
4. The preliminary vs actual complexity distinction is clearly documented
5. Architecture.md template exists with proper contract structure
6. No grep results for "research.md" in documentation files
7. TapeAge test plugin (which already has architecture.md) works with updated workflow
</success_criteria>