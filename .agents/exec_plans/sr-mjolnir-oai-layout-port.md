# sr-mjolnir-layout-port

Branch: current branch
Owner: main-agent
Status: complete

## Scope

- Recreate the OAI harness file structure in this checkout.
- Replace OAI-specific content with SR-Mjolnir harness content.
- Adapt path references from SR-Mjolnir's `harness/` layout to this OAI-style
  `.agents/` layout where needed.
- Add a root `init.sh` harness-health gate.

## Non-Goals

- Do not import unrelated SR-Mjolnir application source.
- Do not modify OAI source repositories.
- Do not run Docker, QNX builds, or target-mutating commands as part of the
  default harness check.

## Context Loaded

- `/Users/henryjiang/.agents/skills/harness-creator/SKILL.md`
- OAI harness: `AGENTS.md`, `init.sh`, `.agents/**`, and `scripts/**`
- SR-Mjolnir harness: `AGENTS.md`, `CLAUDE.md`, `harness/**`,
  `docs/operations/harness-onboarding.md`, and `skills/**`

## Steps

- [x] Inspect existing destination state.
- [x] Locate OAI and SR-Mjolnir source harnesses.
- [x] Copy SR-Mjolnir work items, context, subagent rules, onboarding notes,
      validation scripts, QNX helper, and skills into OAI-style paths.
- [x] Add root instructions, progress, handoff, and execution-plan files.
- [x] Add root `init.sh` with check and optional setup/verify modes.

## Verification

- [x] `./init.sh check`

## Evidence

- `./init.sh check` passed on 2026-06-20.
- Validation covered 13 SR-Mjolnir work items and links in 56 Markdown files.

## Handoff

Use `AGENTS.md` for startup. There is no active implementation work item after
this harness migration; choose or add one in `.agents/feature_list.json` for the
next task.
