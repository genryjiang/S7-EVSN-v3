# 2026-06-22 - Deferred HDMI Qt Driver Display Frontend

Work item: `VSN-0037`

## Request

Shelve the driver display work until last. Replace the old USB/serial display
direction with an HDMI-connected Qt frontend.

## Decision

- The old EMBD-v2-HighLevel `display_driver` USB/serial output path remains out
  of the MVP runtime.
- Driver display itself is not permanently retired; it is deferred as
  `VSN-0037`.
- When resumed, display must be rewritten as a QNX HDMI Qt Quick/QML frontend
  that consumes copied vehicle display snapshots only.
- The display process must have no vehicle-control authority, no final CAN0
  motor-command path, and no real-time-loop participation.
- Display work is last-priority and must not block CAN, motor, PedalBox, BMS,
  GNSS, lighting, telemetry, cloud, ITTIA, or network regression work.

## External Documentation Evidence

QNX online documentation consulted:

- QNX Product Documentation index:
  `https://www.qnx.com/developers/docs/index.html`
- QNX SDP 8.0 documentation:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
- QNX SDP 8.0 Graphics documentation:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/ui.html`
- QNX Screen Graphics Subsystem Developer's Guide:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen/topic/manual/cscreen_about.html`
- QNX OpenWF Display Server User's Guide:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen.wfd-server/topic/manual/cwfd-server_about.html`
- QNX OpenWF Display Configuration Developer's Guide:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen.wfdcfg/topic/manual/cwfdcfg_about.html`

Qt documentation consulted:

- Qt for QNX:
  `https://doc.qt.io/qt-6/qnx.html`
- Qt for QNX runtime target requirements:
  `https://doc.qt.io/qt-6/qnx-target-requirements.html`

## Changes

- Added `.agents/exec_plans/evsn-driver-display-qt-frontend.md`.
- Added `VSN-0037` to `.agents/feature_list.json`.
- Updated roadmap traceability and matrix rows from retired display to deferred
  display: old USB/serial path is absent from MVP, future HDMI Qt work belongs
  to `VSN-0037`.
- Kept `VSN-0026` lighting-only and explicitly excluded Qt/HDMI display work.
- Updated IDL, GNSS, noncritical-controller, and regression plans to prevent
  active MVP display scope creep.

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 37 work items and links in 128 Markdown
  files.
