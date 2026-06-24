# evsn-runtime-port

Branch: current branch
Owner: main-agent
Status: complete

## Scope

- Copy SR-Mjolnir's `core/` runtime into a more specific top-level folder.
- Rename the runtime from SR naming to EVSN naming.
- Copy the minimal CMake and wrapper scripts needed to build/test the runtime.
- Keep the copied runtime free of SR/Sunswift/Mjolnir project names.

## Non-Goals

- Do not port application nodes outside the runtime.
- Do not regenerate RTI DDS type-support code.
- Do not require QNX or Docker builds on the host.

## Context Loaded

- `AGENTS.md`
- `.agents/skills/qnx-cpp-rtos/SKILL.md`
- C++ convention skill `following-conventions`
- Source runtime under `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/core`
- Source build helpers under `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/cmake`
- Source build wrappers under `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/scripts`

## Steps

- [x] Create `evsn-runtime/`.
- [x] Copy DDS types and node runtime.
- [x] Rename `sr_node` to `evsn_node` and `SRNode` to `EVSNNode`.
- [x] Rename DDS file/type prefixes from `sr_`/`SR` to `evsn_`/`EVSN`.
- [x] Copy CMake helper/toolchain files and minimal build scripts.
- [x] Add a root `CMakeLists.txt` for the runtime.
- [x] Update harness checks for the new runtime/build files.

## Verification

- [x] Old-name scan over `evsn-runtime`, `cmake`, `scripts`, and `CMakeLists.txt`
      Result: no matches.
- [x] `bash -n scripts/* init.sh`
      Result: passed.
- [x] `./init.sh check`
      Result: passed.
- [x] `cmake -S . -B /tmp/s7-evsn-configure -DBUILD_TESTING=OFF`
      Result: reached expected `RTIConnextDDS` package lookup failure because
      host `NDDSHOME`/`RTI_HOME` are unset.
- [x] `./scripts/test-no-evsnnode-mqtt`
      Result: passed.
- [x] `node --check scripts/rewrite-compile-commands.mjs`
      Result: passed.
- [x] `git diff --check`
      Result: passed.

## Evidence

- Runtime folder: `evsn-runtime/`
- Build helpers: `cmake/`
- Build wrappers: `scripts/`

## Handoff

Full compilation requires the Docker builder or a host environment with RTI
Connext available through `NDDSHOME`/`RTI_HOME`.
