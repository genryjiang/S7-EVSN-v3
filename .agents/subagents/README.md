# Subagent Coordination

Use subagents to split large QNX, DDS, CAN, or safety-sensitive work into
bounded roles. The coordinator remains responsible for understanding the work
item, synthesizing findings, assigning precise tasks, and deciding when work is
complete.

## Approved Roles

| Role | Allowed work | Must return |
| --- | --- | --- |
| `researcher` | Read/search only. Inspect code, docs, contracts, generated types, and build scripts. | Findings with file references, risks, and unresolved questions. |
| `implementer` | Edit only assigned files for one work item or task slice. | Diff summary, verification commands, results, and blockers. |
| `reviewer` | Audit correctness, QNX/RTI/CAN behavior, safety assumptions, and tests. | Findings ordered by severity with file references and residual risk. |
| `target-tester` | Run QEMU or approved remote QNX checks only with explicit target config. | Target identity, commands, logs, exit status, artifacts, and cleanup notes. |

Subagents are coordination roles; they do not need to be separate processes.
Use the role boundaries even when a single agent performs multiple phases.

## Workflow

1. The coordinator selects one `.agents/feature_list.json` item.
2. For feature implementation or other plan-required work, the coordinator
   creates or updates an active execution plan before assigning implementation.
3. A `researcher` inspects the smallest useful code and documentation surface.
4. The coordinator turns findings into a concrete implementation spec.
5. An `implementer` changes only the assigned files and runs focused checks.
6. A `reviewer` checks behavior, verification, and safety evidence.
7. The coordinator records durable evidence in `.agents/feature_list.json`.

Use existing `notes`, `evidence`, `verification_commands`, and `blockers`
fields. Prefix evidence with the role when it helps future sessions, for
example:

```text
researcher: inspected pedal_driver DDS/CAN path and found no QNX-only API use.
reviewer: checked pedal_processor timing assumptions; no dynamic allocation in loop.
target-tester: ran out/qnx/bin/pedal_driver on qemu-qnx800-aarch64 via SSH port 2222.
```

## Coordination Rules

- The coordinator must synthesize findings before assigning implementation.
- Do not assign implementation for large features or plan-required work unless
  an active plan exists under `.agents/exec_plans/`.
- Prompts must be self-contained; do not rely on chat history.
- Do not recursively spawn subagents.
- Keep one active owner per work item or file area.
- Research roles must not mutate files.
- Implementation roles must run or report assigned verification.
- Target testing is opt-in and must not be part of the default local gate.
- For RTI-Connext based node, RTI Connext node, or DDS contract work, prompts must require
  `RTI_MCP_GUIDANCE_REQUIRED` evidence from the RTI Connext MCP.
- For QNX-facing work, prompts must require `QNX_ONLINE_DOCS_REQUIRED` evidence
  from current online QNX documentation, and the work must stop if those online
  docs cannot be consulted.
- Do not modify `S130_dev-tools`, QNX target scripts, or safety-relevant code
  unless the work item explicitly allows it.

## Prompt Templates

### Researcher

```markdown
# Research Task

## Context

Work item: `<VSN-XXXX and title>`
Question: `<specific question to answer>`
Relevant starting paths: `<files/directories>`

## Role

You are a researcher. Read and search only. Do not edit files.

## Constraints

- Stay within the listed paths unless a direct reference requires one hop.
- Prefer file and line references over broad summaries.
- Do not propose implementation until facts are clear.

## Deliverable

Return:
1. Findings with file references.
2. Risks or contradictions.
3. Unresolved questions that block implementation.
```

### Implementer

```markdown
# Implementation Task

## Context

Work item: `<VSN-XXXX and title>`
Coordinator decision: `<specific implementation choice>`
Execution plan: `<.agents/exec_plans/... or "not required: narrow low-risk change">`
Allowed files: `<exact files or directories>`
Verification: `<commands to run>`

## Role

You are an implementer. Make only the assigned change.

## Constraints

- Follow `AGENTS.md` and the relevant skills.
- Provide RTI MCP evidence when `RTI_MCP_GUIDANCE_REQUIRED` applies.
- Provide online QNX documentation evidence when `QNX_ONLINE_DOCS_REQUIRED`
  applies; do not implement QNX-facing behavior from memory or offline
  references alone.
- Refuse large feature implementation if no active execution plan is provided.
- Do not edit outside the allowed files.
- Do not change `S130_dev-tools` or target scripts unless explicitly allowed.
- Preserve deterministic timing and bounded behavior.

## Deliverable

Return:
1. Files changed and behavior changed.
2. Verification commands and results.
3. Blockers or evidence gaps.
```

### Reviewer

```markdown
# Review Task

## Context

Work item: `<VSN-XXXX and title>`
Diff or files to review: `<paths or patch summary>`
Expected behavior: `<acceptance criteria>`

## Role

You are a reviewer. Prioritize bugs, behavioral regressions, missing tests, and
safety-relevant assumptions.

## Constraints

- Review QNX/RTI/CAN differences explicitly where relevant.
- Verify RTI MCP evidence exists when `RTI_MCP_GUIDANCE_REQUIRED` applies.
- Verify online QNX documentation evidence exists when
  `QNX_ONLINE_DOCS_REQUIRED` applies.
- Check timing, blocking, allocation, and failure behavior.
- Do not rewrite the implementation unless asked.

## Deliverable

Return:
1. Findings ordered by severity with file references.
2. Missing verification or residual risk.
3. A short approval/blocking recommendation.
```

### Target Tester

```markdown
# Target Test Task

## Context

Work item: `<VSN-XXXX and title>`
Target: `<QEMU or remote target identity>`
Artifact: `<artifact path>`
Commands: `<bounded commands to run>`

## Role

You are a target tester. Run only the approved QNX target checks.

## Constraints

- Use credentials and target config supplied outside version control.
- Keep commands bounded and non-interactive.
- Capture logs, exit status, and artifact identity.
- Clean up temporary files unless evidence retention requires them.

## Deliverable

Return:
1. Target identity and connection method.
2. Commands run and exit status.
3. Logs or evidence paths.
4. Cleanup performed and remaining risk.
```
