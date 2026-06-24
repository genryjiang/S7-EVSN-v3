# Agent Harness

This directory ports the OAI harness structure into this checkout while using
SR-Mjolnir's harness content.

- `feature_list.json`: S7-EVSN work items and verification evidence
- `agent-context.md`: durable repository facts for SR-Mjolnir-style work
- `progress.md`: harness migration and future session progress
- `session-handoff.md`: restart path for the next agent session
- `exec_plans/`: active execution plans
- `logs/`: durable work logs
- `skills/`: SR-Mjolnir canonical skills
- `subagents/`: coordination roles and prompt templates
- `tooling/`: onboarding notes and QNX helper material
- `validate-feature-list.mjs`: work-item schema validation
- `validate-doc-links.mjs`: local Markdown link validation

Use `../init.sh check` from the repository root to validate harness health.
