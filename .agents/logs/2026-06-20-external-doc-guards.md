# 2026-06-20 External Documentation Guards

Added strict harness guards for RTI Connext and QNX work.

Changes:

- `AGENTS.md` now requires RTI Connext MCP guidance before designing, creating,
  or materially changing RTI Connext-based nodes or DDS contracts.
- `AGENTS.md` now requires current online QNX documentation before QNX-facing
  implementation or review.
- `rti-connext-dds` and `qnx-cpp-rtos` skills repeat the guard at the skill
  level.
- `subagents/README.md` requires subagent prompts and reviews to carry the same
  evidence requirements.
- `exec_plans/README.md` includes an External Guidance evidence section.
- `init.sh check` now fails if the guard anchors are removed from the harness.

Official QNX documentation entry points used for the guard:

- `https://www.qnx.com/developers/docs/index.html`
- `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`

Verification:

- `./init.sh check` passed on 2026-06-20 and reported all seven external
  guidance guard checks as present.
- `bash -n init.sh` passed.
- `git diff --check` passed.
