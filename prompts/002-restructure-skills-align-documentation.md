<objective>
Restructure all Claude Code skills to follow Anthropic's official skill structure pattern with subdirectories for references and assets, and update all architecture and procedure documentation to align with this new structure.

This restructuring is critical because:
1. Current skill files are massive (1000+ lines), causing performance issues
2. Templates exist in /templates/ but aren't being used by skills
3. Documentation references outdated patterns that don't match implementation
4. We need consistency between what's documented and what's built
</objective>

<context>
Project: PLUGIN FREEDOM SYSTEM - A JUCE plugin development automation system
Current State:
- Phase 0 (Foundation) and Phase 1 (Discovery System) are complete
- 7 skills implemented but using monolithic SKILL.md files
- /templates/ directory exists with 4 contract templates but skills don't reference them
- Phase 2-7 are planned but not yet implemented

Key constraint: We must preserve functionality while restructuring - the system needs to continue working throughout the refactor.

@.claude/CLAUDE.md - Review current system status
@architecture/09-file-system-design.md - Check documented file structure
@architecture/04-component-architecture.md - Review skill structure specifications
</context>

<analysis>
First, thoroughly analyze the current state:

1. Measure current skill file sizes:
   !wc -l .claude/skills/*/SKILL.md | sort -rn

2. Identify which skills need the most restructuring (likely the largest ones)

3. Scan for all references to skill structure in documentation:
   !grep -r "skills/" architecture/ procedures/ --include="*.md" | head -20
   !grep -r "templates/" architecture/ procedures/ .claude/ --include="*.md"

4. Determine which templates should move to which skill's assets/ directory
</analysis>

<restructuring_plan>
For each skill, deeply consider the optimal structure following Anthropic's pattern:

```
skill-name/
├── SKILL.md (orchestration logic only - target ~200-400 lines)
├── references/ (documentation loaded as needed)
│   └── *.md files with detailed workflows, specifications
└── assets/ (templates and resources)
    └── *.md, *.yaml, *.txt templates
```

Priority order (largest skills first):
1. plugin-testing (1161 lines)
2. ui-mockup (1120 lines)
3. plugin-workflow (1037 lines)
4. plugin-lifecycle (990 lines)
5. context-resume (866 lines)
6. plugin-ideation (604 lines)
7. plugin-improve (577 lines)
</restructuring_plan>

<implementation>
Execute the restructuring systematically:

## Phase 1: Restructure Skills (for each skill)

1. Create subdirectory structure:
   ```bash
   mkdir -p .claude/skills/[skill-name]/references
   mkdir -p .claude/skills/[skill-name]/assets
   ```

2. Extract references from SKILL.md:
   - Workflow phase documentation → references/workflow-phases.md
   - Technical specifications → references/technical-specs.md
   - Integration guidelines → references/integration.md
   - Leave only orchestration logic in SKILL.md

3. Move relevant templates from /templates/ to assets/:
   - plugin-ideation gets: creative-brief.md template
   - plugin-workflow gets: plan.md template
   - ui-mockup gets: parameter-spec.md, architecture.md templates

4. Update SKILL.md to reference subdirectory files:
   - Change embedded templates to: "Copy from assets/template.md"
   - Change embedded docs to: "See references/workflow-phases.md for details"

## Phase 2: Update Architecture Documentation

Scan and update all architecture files that reference:
- Old skill structure (monolithic SKILL.md)
- /templates/ directory usage
- Skill file organization patterns

Files likely needing updates:
- architecture/04-component-architecture.md
- architecture/09-file-system-design.md
- architecture/02-core-abstractions.md

## Phase 3: Update Procedure Documentation

Update procedure files to match new structure:
- procedures/skills/*.md - Update any that describe skill implementation
- procedures/core/*.md - Update references to skill organization

## Phase 4: Update Phase Plans

Review and update phase plans for consistency:
- plan/phase-1-plan.md - Mark as using new structure
- plan/phase-2-plan.md - Update to expect new structure

## Phase 5: Clean Up

After verification:
- Remove /templates/ directory (contents moved to skill assets/)
- Update .claude/CLAUDE.md to reflect new structure
</implementation>

<validation>
After each skill restructure, verify:

1. Skill still loads correctly:
   - Test with a simple invocation
   - Verify SKILL.md frontmatter is intact

2. References are accessible:
   - Ensure SKILL.md can read from references/ when needed
   - Ensure templates can be copied from assets/

3. File sizes are reasonable:
   !wc -l .claude/skills/*/SKILL.md | sort -rn
   - Target: All SKILL.md files under 500 lines

4. Documentation consistency:
   !grep -r "templates/" architecture/ procedures/ --include="*.md" | wc -l
   - Should return 0 or only historical references

5. Git tracking:
   - Ensure all moves are tracked properly
   - Commit after each major skill restructure
</validation>

<parallel_operations>
For maximum efficiency, when restructuring each skill:
1. Create all directories simultaneously
2. Move all files in parallel where dependencies allow
3. Update multiple documentation files concurrently after skill restructures are complete
</parallel_operations>

<output>
Structure after completion:

```
.claude/skills/
├── plugin-ideation/
│   ├── SKILL.md (~250 lines)
│   ├── references/
│   │   ├── workflow-phases.md
│   │   └── decision-trees.md
│   └── assets/
│       └── creative-brief-template.md
├── ui-mockup/
│   ├── SKILL.md (~300 lines)
│   ├── references/
│   │   ├── webview-integration.md
│   │   ├── parameter-mapping.md
│   │   └── member-ordering.md
│   └── assets/
│       ├── parameter-spec-template.md
│       ├── architecture-template.md
│       ├── ui-yaml-template.yaml
│       └── editor-templates/
│           ├── editor-h.txt
│           └── editor-cpp.txt
[... continue for all skills ...]

/templates/ - REMOVED (contents moved to appropriate skill assets/)
```

Save progress summary to: `./restructure-summary.md`
</output>

<success_criteria>
The restructuring is complete when:
1. All 7 skills follow the Anthropic skill structure pattern
2. No SKILL.md file exceeds 500 lines
3. All templates are in appropriate skill assets/ directories
4. All architecture docs reflect the new structure
5. All procedure docs reflect the new structure
6. /templates/ directory is removed
7. System still functions correctly with restructured skills
8. Git history preserves the refactoring clearly
</success_criteria>

<considerations>
Throughout this refactor, carefully consider:
- What functionality has been implemented (Phase 0-1) vs planned (Phase 2-7)
- How Phase 2+ implementations will interact with the new structure
- Whether subagents (Phase 3) will need their own subdirectory patterns
- How this structure scales to future phases

Be thorough in analyzing interdependencies before moving files. This is a foundational change that affects the entire system architecture.
</considerations>