# Execution Plans

Use one execution plan per active work item. Plans are for work that spans more
than a trivial file edit, changes shared QNX/RTI/DDS/CAN contracts, affects
safety-relevant behavior, touches build or deployment flow, or needs handoff
across context windows.

## Required Fields

- Work item ID in `VSN-XXXX` format
- Branch
- Owner or subagent
- Scope and non-goals
- Repo context loaded
- Step checklist
- Verification commands
- External guidance evidence when `RTI_MCP_GUIDANCE_REQUIRED` or
  `QNX_ONLINE_DOCS_REQUIRED` is triggered
- Evidence links or log files
- Handoff notes

## Template

```md
# VSN-XXXX - <title>

Branch: feature/<slug>
Owner: main-agent
Status: planned | in_progress | blocked | complete

## Scope

## Non-Goals

## Context Loaded

## External Guidance

- RTI MCP:
- QNX online docs:

## Steps

- [ ] Step 1
- [ ] Step 2

## Verification

- [ ] Command:
      Result:

## Evidence

## Handoff
```
