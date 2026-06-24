# 2026-06-20 - rti-qnx-guidance-guard

## Request

Restore strict harness guards requiring RTI Connext MCP guidance before
RTI-Connext based node work and current online QNX documentation consultation
for QNX-facing work.

## Actions

- Tightened `AGENTS.md` so RTI-Connext based node and DDS contract work must run
  the RTI Connext MCP before implementation.
- Tightened QNX wording so agents must stop if current online QNX documentation
  cannot be consulted; offline memory or local references are not enough.
- Mirrored the rule in the RTI and QNX skills plus subagent prompt guidance.
- Extended `init.sh check` to fail if the stronger RTI/QNX guard wording is
  removed from `AGENTS.md`.

## Evidence

- Official QNX online documentation entry point verified:
  `https://www.qnx.com/developers/docs/8.0/`.

## Verification

- `./init.sh check` passed, including the RTI/QNX guard checks.
