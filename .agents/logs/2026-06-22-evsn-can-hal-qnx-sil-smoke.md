# VSN-0019 - QNX/SIL CAN HAL Smoke

Date: 2026-06-22
Branch: `feature/evsn-can-abstraction-layer`
Role: target-tester
Status: passed for QNX/SIL vcan loopback

## Scope

Run the bounded QNX/SIL runtime smoke requested for `VSN-0019` using the user's
QEMU target at:

`/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`

This smoke verifies `dev-can-linux -L` vcan resource-manager endpoints and raw
CAN traffic through `cansend`/`candump`. It is not physical TDA4 FD-CAN
evidence.

## QNX Documentation Gate

Current official QNX documentation was consulted before target work:

- QNX Product Documentation:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- CAN `devctl()` command index:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- C library `devctl()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/d/devctl.html

Design consequence: the smoke remains raw CAN/vcan evidence only. Official QNX
raw `struct can_msg` documentation still shows an 8-byte classic CAN payload,
so this does not relax the HAL's FD-CAN portability constraints.

## Target Identity

SSH command:

```sh
ssh -o BatchMode=yes -o StrictHostKeyChecking=no \
  -o UserKnownHostsFile=/dev/null -o ConnectTimeout=3 \
  -p 2222 root@localhost 'uname -a'
```

Output:

```text
QNX qnx-a64 8.0.0 2025/07/30-19:17:34EDT QEMU_virt aarch64le
```

## Tool Build

The target image did not include `dev-can-linux`, `cansend`, or `candump`, so
the tools were built in the existing project QNX builder.

Command:

```sh
docker compose --project-name s7-evsn-qnx --env-file docker/.env \
  -f docker/compose.yml exec -T --user developer dev-env-builder bash -lc '
set -euo pipefail
rm -rf /tmp/evsn-vcan-build /tmp/evsn-vcan-deploy
cmake -S /workspace/project/vcan-drivers -B /tmp/evsn-vcan-build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=/workspace/project/vcan-drivers/cmake/qnx-aarch64le.toolchain.cmake \
  -DCMAKE_INSTALL_PREFIX=/tmp/evsn-vcan-deploy \
  -DBUILD_TESTING=OFF
cmake --build /tmp/evsn-vcan-build --parallel 8 --target dev-can-linux cansend candump
'
```

Result: target build succeeded. The upstream install step was not used for
evidence because it expects an unbuilt `canread` artifact. The three smoke
binaries were copied directly from the build tree.

Artifact identity:

```text
out/qnx-vcan-smoke/candump:       ELF 64-bit LSB pie executable, ARM aarch64, QNX
out/qnx-vcan-smoke/cansend:       ELF 64-bit LSB pie executable, ARM aarch64, QNX
out/qnx-vcan-smoke/dev-can-linux: ELF 64-bit LSB pie executable, ARM aarch64, QNX
```

## Deployment

Command:

```sh
ssh -p 2222 root@localhost 'rm -rf /tmp/evsn-vcan-smoke && mkdir -p /tmp/evsn-vcan-smoke'
scp -O -P 2222 out/qnx-vcan-smoke/dev-can-linux \
  out/qnx-vcan-smoke/cansend out/qnx-vcan-smoke/candump \
  root@localhost:/tmp/evsn-vcan-smoke/
ssh -p 2222 root@localhost 'chmod 755 /tmp/evsn-vcan-smoke/* && ls -l /tmp/evsn-vcan-smoke'
```

Target listing:

```text
-rwxr-xr-x  1 root root 144192 2026-06-22 05:18 candump
-rwxr-xr-x  1 root root 144048 2026-06-22 05:18 cansend
-rwxr-xr-x  1 root root 276088 2026-06-22 05:18 dev-can-linux
```

## Smoke Command

The bounded target script:

```sh
cd /tmp/evsn-vcan-smoke
./dev-can-linux -L 1 -u id=0,rx=1,tx=1 -b id=0,freq=1000k > driver.log 2>&1 &
driver_pid=$!
# wait up to 10 seconds for /dev/can0/rx0 and /dev/can0/tx0
./candump -u0,rx0 > candump.log 2>&1 &
dump_pid=$!
./cansend -u0,tx0 -m0x124,0,0x1122334455667788
./cansend -u0,tx0 -m0x1234,1,0xAABBCCDD
kill "$dump_pid"
kill "$driver_pid"
grep -q '\[SFF\] 124 \[8\] 124#1122334455667788' candump.log
grep -q '\[EFF\] 1234 \[4\] 1234#AABBCCDD' candump.log
```

Exit status: `0`

Observed target endpoints:

```text
crw-rw-rw-  1 root root 0,   1 2026-06-22 05:19 rx0
crw-rw-rw-  1 root root 0,   1 2026-06-22 05:19 tx0
```

Observed CAN frames:

```text
/dev/can0/rx0 TS: 0ms [SFF] 124 [8] 124#1122334455667788
/dev/can0/rx0 TS: 0ms [EFF] 1234 [4] 1234#AABBCCDD
```

Driver log: empty during this quiet successful run.

## Cleanup

The smoke script killed `candump` and `dev-can-linux` before exit. Follow-up
inspection showed no remaining `dev-can-linux`, `candump`, or `cansend`
processes and `/dev/can0` was removed after the resource manager stopped.

## Result

`VSN-0019` now has QNX/SIL vcan runtime evidence for standard and extended raw
CAN loopback through `dev-can-linux -L`, `cansend`, and `candump`. The work item
remains `in_progress` because TDA4 FD-CAN vendor artifacts and physical or
bench evidence are still unavailable.
