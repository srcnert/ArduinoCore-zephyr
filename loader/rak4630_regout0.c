/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * On the RAK4630 module the nRF52840 is supplied through VDDH (high-voltage
 * mode), where the GPIO/IO rail is driven by the internal regulator configured
 * via UICR REGOUT0. The factory default is 1.8 V, which leaves LEDs and 3.3 V
 * peripherals underpowered after a full chip erase removes the stock
 * bootloader's setting.
 *
 * This early-boot hook programs REGOUT0 to 3.3 V (once) and resets the chip so
 * the new setting takes effect. It only acts when the chip runs in
 * high-voltage mode and REGOUT0 is still at its default, so it is a no-op on
 * boards powered in normal-voltage mode.
 */

#include <zephyr/init.h>

#if defined(CONFIG_BOARD_NRF52840DK)
#include <cmsis_core.h>
#include <hal/nrf_power.h>

static int rak4630_vddh_regout0_3v3(void) {
	if ((nrf_power_mainregstatus_get(NRF_POWER) == NRF_POWER_MAINREGSTATUS_HIGH) &&
		((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) ==
		 (UICR_REGOUT0_VOUT_DEFAULT << UICR_REGOUT0_VOUT_Pos))) {

		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {
		}

		NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
							(UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos);

		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {
		}

		/* UICR changes only take effect after a reset */
		NVIC_SystemReset();
	}

	return 0;
}

SYS_INIT(rak4630_vddh_regout0_3v3, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
#endif /* CONFIG_BOARD_NRF52840DK */
