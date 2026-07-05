# J784S4 EVM QNX BSP MCAN Overlay

This directory carries the EVSN-owned QNX BSP integration material for
`VSN-0038`. It is not a copy of the TI/QNX BSP. Apply these fragments to the
licensed BSP tree in the build container, then rebuild the IFS.

Inspected BSP paths in the current builder container:

```text
/opt/ti-processor-sdk-rtos-j784s4-evm-11_02_00_06/psdkqa/qnx/bsp/src/hardware/startup/boards/ti-j7/j784s4/evm/hw_init.c
/opt/ti-processor-sdk-rtos-j784s4-evm-11_02_00_06/psdkqa/qnx/bsp/BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342/images/j784s4-evm-ti.build
```

## Files

| File | Purpose |
| --- | --- |
| `j784s4_evm_mcan_startup_overlay.c` | EVSN startup fragment for MCAN TI SCI power/clock enable, padmux, and MCU transceiver standby GPIO setup. |
| `j784s4-evm-ti.mcan.buildfile.fragment` | mkifs buildfile and startup-script fragment for staging and starting `dev-evsn-mcan-j784`. |

## Startup Integration

Add the startup fragment to the J784S4 EVM startup source and call
`evsn_init_mcan()` while the control MMR lock is open. In the inspected BSP,
that means placing the call near the existing board initializers, after
`init_i2c()` and before the lock is restored.

Expected startup responsibilities:

- Enable WKUP GPIO0 through TI SCI.
- Enable clocks and deassert resets for:
  - `TISCI_DEV_MCU_MCAN0`
  - `TISCI_DEV_MCU_MCAN1`
  - `TISCI_DEV_MCAN4`
  - `TISCI_DEV_MCAN16`
- Program padmux for:
  - `MCU_MCAN0` J42
  - `MCU_MCAN1` J43
  - `MAIN_MCAN16` J46
  - `MAIN_MCAN4` J45 only after the EthFw/mux conflict is intentionally
    resolved.
- Drive MCU transceiver standby GPIOs low:
  - `WKUP_GPIO0_69` for `MCU_MCAN0`
  - `WKUP_GPIO0_2` for `MCU_MCAN1`

`MAIN_MCAN4` is guarded because the inspected
`k3-j784s4-evm-ethfw.dtbo` disables `main_mcan4`, `transceiver3`, and `mux1`.
Enable `EVSN_ENABLE_MAIN_MCAN4_STARTUP` only in an image where QNX is meant to
own that path and EthFw is not disabling it.

## IFS Integration

Use the buildfile fragment as a guide for:

- Staging `/sbin/dev-evsn-mcan-j784`.
- Running `--enable-transceivers` after the I2C expander setup.
- Running the `MAIN_MCAN4` I2C transceiver command only after the EthFw
  conflict has been resolved.
- Starting one external CAN resource-manager process per enabled controller.

The production-named launch command is:

```sh
dev-evsn-mcan-j784 --external-can-resource-manager-endpoint <id> <irq>
```

This is an alias for the interrupt-driven hardware resource-manager path with
Bosch internal loopback disabled. It does not by itself prove external CAN-FD
readiness; connector/analyzer evidence is still required.

## Evidence Inputs

The overlay is based on these inspected evidence sources:

```text
extra_docs/sprr458d/PROC141E5_RP/j784s4_evm_pinmux.syscfg
extra_docs/tisdk-adas-image-j784s4-evm/boot/dtb/ti/k3-j784s4-evm.dtb
extra_docs/tisdk-adas-image-j784s4-evm/boot/dtb/ti/k3-j784s4-evm-ethfw.dtbo
s7-evsn-qnx-dev-env-builder-1 TI QNX BSP startup headers and buildfile
```

The Linux DTB proves the board wiring target and expected Linux-enabled MCAN
paths. It does not prove that the QNX production boot image already owns those
paths. The QNX startup/buildfile integration is the port of that ownership.
