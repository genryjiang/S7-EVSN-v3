# 2026-06-20 VSN Work Item IDs

Work item: `VSN-0018`

## Actions

- Migrated existing durable work item IDs from the inherited project namespace
  to uppercase `VSN-XXXX` numbering.
- Updated dependency references, execution-plan references, current handoff
  references, and durable progress references.
- Added explicit `VSN-XXXX` instructions to `AGENTS.md`, the execution-plan
  template, and subagent prompt templates.
- Updated `.agents/schemas/work-items.schema.json` and
  `.agents/validate-feature-list.mjs` so non-VSN IDs are rejected.

## Evidence

- `! rg -n '[m]jolnir-[[:digit:]#x]{3}' AGENTS.md .agents
  init.sh README.md scripts tests tools cmake CMakeLists.txt` passed with no
  matches.
- `./init.sh check` passed, validating 18 work items and links in 76 Markdown
  files.
- `git diff --check` passed.
