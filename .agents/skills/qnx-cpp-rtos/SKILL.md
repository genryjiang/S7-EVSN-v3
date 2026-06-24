---
name: qnx-cpp-rtos
description: General QNX Neutrino RTOS C++ development. Use for QNX implementation, review, debugging, deployment, IPC, resource managers, QNX/POSIX API use, target builds, and QNX-specific runtime behavior.
---

# QNX C++ RTOS

Use this skill for QNX Neutrino RTOS C++ work in this repository.

## Core Guidance

- `QNX_ONLINE_DOCS_REQUIRED`: Before implementing or reviewing any QNX-facing
  behavior, QNX/POSIX API use, target build or deployment flow, IPC,
  scheduling, resource-manager behavior, HAM behavior, startup/process/thread
  behavior, filesystem/networking behavior, or compiler/toolchain behavior,
  consult the current online QNX documentation.
- Start from the official QNX Product Documentation page and the QNX SDP 8.0
  documentation:
  `https://www.qnx.com/developers/docs/index.html` and
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`.
- Record the URLs or section titles used in the active execution plan or log.
- If online QNX documentation is inaccessible, stop and record a blocker. Do
  not implement or review QNX-facing behavior from memory or offline references
  alone.
- Prefer the repository CMake and wrapper flow for builds. Use direct `q++` only where necessary for QNX C++ target work or focused toolchain diagnosis.
- Do not assume Linux behavior where QNX behavior may differ.
- Prefer deterministic behavior, bounded memory, bounded execution, and explicit error handling.
- Check every fallible QNX/POSIX API call and preserve enough context to diagnose failures.
- Use QNX message passing, pulses, channels, connections, and resource managers where they fit the ownership and timing model.
- Keep QNX-specific types and headers out of public cross-platform interfaces unless the target boundary requires them.
- Document target assumptions, startup behavior, IPC ownership, blocking behavior, and failure behavior when changing QNX-facing code.

## References

Read the relevant reference before giving QNX-specific API, IPC, deployment, or resource-manager guidance:

- `references/qnx-neutrino-rtos.yaml`
- `references/qnx-advanced.yaml`
