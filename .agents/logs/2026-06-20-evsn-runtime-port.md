# 2026-06-20 - evsn-runtime-port

## Actions

- Copied SR-Mjolnir `core/` into `evsn-runtime/`.
- Renamed node runtime files, targets, tests, environment variables, and DDS
  type-support prefixes to EVSN naming.
- Copied minimal build support into `cmake/` and `scripts/`.
- Added root `CMakeLists.txt`.
- Updated `init.sh check` to include the runtime/build surface and to tolerate
  either `Dev-Env-Builder/` or the current `cloud-infra/` infra folder.

## Verification

- Old-name scan over copied runtime/build files returned no matches.
- `bash -n scripts/* init.sh` passed.
- `./init.sh check` passed.
- CMake configure reached the expected RTI Connext package lookup failure
  because host `NDDSHOME`/`RTI_HOME` are unset.
- `./scripts/test-no-evsnnode-mqtt` passed.
- `node --check scripts/rewrite-compile-commands.mjs` passed.
- `git diff --check` passed.
