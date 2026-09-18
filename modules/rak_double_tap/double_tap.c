/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Double-tap reset: press the reset button twice within
 * CONFIG_RAK_DOUBLE_TAP_WINDOW_MS duration to enter the bootloader's recovery mode.
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/retention/bootmode.h>

#define RAK_BOOT_MODE_DOUBLE_TAP_ARMED 0xD7

#ifdef CONFIG_MCUBOOT_INDICATION_LED
static const struct gpio_dt_spec window_led = GPIO_DT_SPEC_GET(DT_ALIAS(mcuboot_led0), gpios);
#endif

static void window_led_set(bool on) {
#ifdef CONFIG_MCUBOOT_INDICATION_LED
	(void)gpio_pin_configure_dt(&window_led, on ? GPIO_OUTPUT_ACTIVE : GPIO_OUTPUT_INACTIVE);
#else
	ARG_UNUSED(on);
#endif
}

static bool reset_by_button(void) {
	uint32_t cause = 0;
	int rc;

	rc = hwinfo_get_reset_cause(&cause);
	(void)hwinfo_clear_reset_cause();

	if (rc != 0) {
		return false;
	}

	return (cause & RESET_PIN) != 0;
}

static int double_tap_check(void) {
	bool button = reset_by_button();

	if (bootmode_check(BOOT_MODE_TYPE_BOOTLOADER) == 1) {
		return 0;
	}

	if (!button) {
		(void)bootmode_clear();
		return 0;
	}

	if (bootmode_check(RAK_BOOT_MODE_DOUBLE_TAP_ARMED) == 1) {
		/* Second tap: keep the LED on; MCUboot takes it over in recovery. */
		window_led_set(true);
		(void)bootmode_set(BOOT_MODE_TYPE_BOOTLOADER);
		return 0;
	}

	if (bootmode_set(RAK_BOOT_MODE_DOUBLE_TAP_ARMED) != 0) {
		return 0;
	}

	window_led_set(true);
	k_busy_wait(CONFIG_RAK_DOUBLE_TAP_WINDOW_MS * USEC_PER_MSEC);
	window_led_set(false);

	(void)bootmode_clear();

	return 0;
}

SYS_INIT(double_tap_check, APPLICATION, 0);
