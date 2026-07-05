/*
 * EVSN VSN-0038 MCAN startup fragment for the TI J784S4 EVM QNX BSP.
 *
 * Integration target:
 *   psdkqa/qnx/bsp/src/hardware/startup/boards/ti-j7/j784s4/evm/hw_init.c
 *
 * This fragment expects the TI QNX BSP startup headers to provide out32(),
 * in32(), kprintf(), TI SCI helpers, J784S4 padconfig bases, GPIO helpers,
 * and TISCI device/clock IDs. It is intentionally small and EVSN-owned; do
 * not paste vendor driver source into this repository.
 *
 * Call evsn_init_mcan() while the control MMR locks are open. In the inspected
 * BSP, call it after init_i2c() and before ctrl_mmr_lock().
 */

#include "tisci_clocks.h"
#include "tisci_devices.h"

#ifndef EVSN_ENABLE_MAIN_MCAN4_STARTUP
#define EVSN_ENABLE_MAIN_MCAN4_STARTUP 0
#endif

#define EVSN_MCAN_TX_PAD_VALUE (PIN_PULL_DISABLE | PIN_MODE(0))
#define EVSN_MCAN_RX_PAD_VALUE                                                \
  (PIN_INPUT_ENABLE | PIN_PULL_DISABLE | PIN_PULL_DIRECTION | PIN_MODE(0))
#define EVSN_WKUP_GPIO_OUTPUT_PAD_VALUE (PIN_PULL_DISABLE | PIN_MODE(7))

/*
 * The inspected QNX BSP header does not expose descriptive MCU_MCAN1 pin names
 * or WKUP_GPIO0_4/5 pad names. These values are the QNX WKUP padconfig offsets
 * corresponding to the Linux DTB mcu-mcan1 TX/RX pinctrl entries.
 */
#ifndef EVSN_PIN_MCU_MCAN1_TX
#define EVSN_PIN_MCU_MCAN1_TX 0x0D0U
#endif

#ifndef EVSN_PIN_MCU_MCAN1_RX
#define EVSN_PIN_MCU_MCAN1_RX 0x0D4U
#endif

/*
 * The Linux DTB exposes main-mcan4-default-pins as offset 0x84 TX and 0x88 RX
 * under MAIN PADCONFIG. Keep this path compile-time guarded until the EthFw
 * mux/transceiver conflict is resolved for the QNX image.
 */
#ifndef EVSN_PIN_MAIN_MCAN4_TX
#define EVSN_PIN_MAIN_MCAN4_TX 0x084U
#endif

#ifndef EVSN_PIN_MAIN_MCAN4_RX
#define EVSN_PIN_MAIN_MCAN4_RX 0x088U
#endif

#ifndef EVSN_PIN_MCU_MCAN0_STANDBY
#define EVSN_PIN_MCU_MCAN0_STANDBY PIN_MCU_SPI0_D1
#endif

#ifndef EVSN_PIN_MCU_MCAN1_STANDBY
#define EVSN_PIN_MCU_MCAN1_STANDBY PIN_WKUP_GPIO0_2
#endif

static int evsn_tisci_enable_mcan_clock(uint32_t dev_id, uint32_t cclk_id,
                                        uint32_t hclk_id) {
  if (ti_sci_set_device_state(dev_id, 0, MSG_DEVICE_SW_STATE_ON) == -1) {
    kprintf("%s:%d dev_id %d device-on failed\n", __func__, __LINE__, dev_id);
    return -1;
  }
  if (ti_sci_set_device_resets(dev_id, 0) == -1) {
    kprintf("%s:%d dev_id %d reset release failed\n", __func__, __LINE__,
            dev_id);
    return -1;
  }
  if (ti_sci_set_clock_state(dev_id, hclk_id, 0, MSG_CLOCK_SW_STATE_AUTO) ==
      -1) {
    kprintf("%s:%d dev_id %d hclk failed\n", __func__, __LINE__, dev_id);
    return -1;
  }
  if (ti_sci_set_clock_state(dev_id, cclk_id, 0, MSG_CLOCK_SW_STATE_AUTO) ==
      -1) {
    kprintf("%s:%d dev_id %d cclk failed\n", __func__, __LINE__, dev_id);
    return -1;
  }
  return 0;
}

static int evsn_enable_wkup_gpio0(void) {
  if (ti_sci_set_device_state(TISCI_DEV_WKUP_GPIO0, 0,
                              MSG_DEVICE_SW_STATE_ON) == -1) {
    kprintf("%s:%d WKUP_GPIO0 device-on failed\n", __func__, __LINE__);
    return -1;
  }
  if (ti_sci_set_clock_state(TISCI_DEV_WKUP_GPIO0,
                             TISCI_DEV_WKUP_GPIO0_MMR_CLK, 0,
                             MSG_CLOCK_SW_STATE_AUTO) == -1) {
    kprintf("%s:%d WKUP_GPIO0 clock failed\n", __func__, __LINE__);
    return -1;
  }
  return 0;
}

static void evsn_wkup_gpio_output_low(uint32_t pad_offset, uint32_t gpio_line) {
  const uint32_t bit = J784S4_GPIO_BIT(gpio_line);
  const uint32_t dir = J784S4_WKUP_GPIO0 + J784S4_GPIO_DIR(gpio_line);

  out32(J784S4_WKUP_CTRLMMR_PADCONFIG + pad_offset,
        EVSN_WKUP_GPIO_OUTPUT_PAD_VALUE);
  out32(J784S4_WKUP_GPIO0 + J784S4_GPIO_CLR_DATA(gpio_line), bit);
  out32(dir, in32(dir) & ~bit);
}

static void evsn_mcan_pinmux(void) {
  out32(J784S4_WKUP_CTRLMMR_PADCONFIG + PIN_MCU_MCAN0_TX,
        EVSN_MCAN_TX_PAD_VALUE);
  out32(J784S4_WKUP_CTRLMMR_PADCONFIG + PIN_MCU_MCAN0_RX,
        EVSN_MCAN_RX_PAD_VALUE);
  out32(J784S4_WKUP_CTRLMMR_PADCONFIG + EVSN_PIN_MCU_MCAN1_TX,
        EVSN_MCAN_TX_PAD_VALUE);
  out32(J784S4_WKUP_CTRLMMR_PADCONFIG + EVSN_PIN_MCU_MCAN1_RX,
        EVSN_MCAN_RX_PAD_VALUE);
  out32(J784S4_CTRLMMR_PADCONFIG + PIN_MCAN16_TX, EVSN_MCAN_TX_PAD_VALUE);
  out32(J784S4_CTRLMMR_PADCONFIG + PIN_MCAN16_RX, EVSN_MCAN_RX_PAD_VALUE);

#if EVSN_ENABLE_MAIN_MCAN4_STARTUP
  out32(J784S4_CTRLMMR_PADCONFIG + EVSN_PIN_MAIN_MCAN4_TX,
        EVSN_MCAN_TX_PAD_VALUE);
  out32(J784S4_CTRLMMR_PADCONFIG + EVSN_PIN_MAIN_MCAN4_RX,
        EVSN_MCAN_RX_PAD_VALUE);
#else
  kprintf("EVSN MCAN: MAIN_MCAN4 padmux skipped; EthFw conflict unresolved\n");
#endif
}

static int evsn_enable_mcan_devices(void) {
  if (evsn_tisci_enable_mcan_clock(TISCI_DEV_MCU_MCAN0,
                                   TISCI_DEV_MCU_MCAN0_MCANSS_CCLK_CLK,
                                   TISCI_DEV_MCU_MCAN0_MCANSS_HCLK_CLK) == -1) {
    return -1;
  }
  if (evsn_tisci_enable_mcan_clock(TISCI_DEV_MCU_MCAN1,
                                   TISCI_DEV_MCU_MCAN1_MCANSS_CCLK_CLK,
                                   TISCI_DEV_MCU_MCAN1_MCANSS_HCLK_CLK) == -1) {
    return -1;
  }
#if EVSN_ENABLE_MAIN_MCAN4_STARTUP
  if (evsn_tisci_enable_mcan_clock(TISCI_DEV_MCAN4,
                                   TISCI_DEV_MCAN4_MCANSS_CCLK_CLK,
                                   TISCI_DEV_MCAN4_MCANSS_HCLK_CLK) == -1) {
    return -1;
  }
#else
  kprintf("EVSN MCAN: MAIN_MCAN4 clock/reset skipped; EthFw conflict unresolved\n");
#endif
  if (evsn_tisci_enable_mcan_clock(TISCI_DEV_MCAN16,
                                   TISCI_DEV_MCAN16_MCANSS_CCLK_CLK,
                                   TISCI_DEV_MCAN16_MCANSS_HCLK_CLK) == -1) {
    return -1;
  }
  return 0;
}

static int evsn_init_mcan(void) {
  if (evsn_enable_wkup_gpio0() == -1) {
    return -1;
  }
  if (evsn_enable_mcan_devices() == -1) {
    return -1;
  }
  evsn_mcan_pinmux();
  evsn_wkup_gpio_output_low(EVSN_PIN_MCU_MCAN0_STANDBY, 69U);
  evsn_wkup_gpio_output_low(EVSN_PIN_MCU_MCAN1_STANDBY, 2U);
  kprintf("EVSN MCAN: startup power/clock/pinmux/transceiver setup complete\n");
  return 0;
}
