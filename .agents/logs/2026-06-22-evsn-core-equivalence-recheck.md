# 2026-06-22 - EVSN Core Equivalence Recheck

Work items: `VSN-0020`, `VSN-0024`, `VSN-0032`, `VSN-0033`, `VSN-0037`

## Request

After shelving the driver display work, re-run a subagent/full-suite check to
determine whether core functionality equivalence has been reached against
EMBD-v2-HighLevel.

## Scope

- Planning and harness-documentation update only.
- No production QNX, RTI, CAN, DDS, cloud, DB, or Qt code was implemented.
- Equivalence means planning coverage of active EMBD-v2-HighLevel core
  network/control surfaces, not implemented, HIL, or vehicle-verified runtime
  equivalence.

## Subagent Evidence

Read-only reviewer subagent:

- ID: `019eed8a-95e4-7243-9e47-afd6d9313bc6`
- Verdict: `core-planning-equivalent`
- Finding: core EVSN plans cover the active old network/control surfaces at
  planning level after excluding explicitly retired systems and shelving the
  display work to `VSN-0037`.
- Remaining risks: implementation not complete, RTI MCP unavailable, QNX target
  validation still required, CAN0 timing/approval still required, device bench
  vectors still required, ITTIA/cloud credentials still required, and
  `VSN-0033` is not implemented.

## Changes

- Kept old USB/serial `display_driver` and `centre_display` out of the MVP.
- Kept future HDMI Qt/QML driver display as shelved last-priority `VSN-0037`.
- Corrected `VSN-0024` BMS desired-outcome wording to `0x620..0x629`,
  matching the hardware-equivalence matrix.
- Hardened `VSN-0032` so the cloud telemetry bridge explicitly rejects old
  cloud-originated motor reset, motor power, motor limit,
  torque/current/velocity, drive-enable, and final CAN0 command requests unless
  a future approved command-authority plan exists.
- Hardened `VSN-0033` with matching negative regression expectations for those
  cloud-to-motor command paths.

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null`: passed.
- Stale wording/regression scan: no matches.
- `git diff --check`: passed.
- `./init.sh check`: passed; harness validated 37 work items and links in 129
  Markdown files.

## Conclusion

Core planning equivalence has been reached for the MVP scope: active
EMBD-v2-HighLevel network/control surfaces have an EVSN owner, retirement, or
deferred work item. Runtime equivalence is not yet reached; it depends on RTI
MCP guidance, QNX target evidence, implementation of the package plans, and
execution of `VSN-0033`.
