/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Checks the debug output of the loader. Build the loader with the debug
 * snippet listed last (e.g. ./extra/build.sh rak4631 -S rzi opt-rzi debug),
 * compile this sketch with the Debug option, and open the log port before
 * the shell port. Then send a command on this sketch's Serial port:
 *
 *   l  ask the I2C driver for 1 MHz, which the nRF52840 TWIM does not have:
 *      the log port shows "<err> i2c_nrfx_twim: unsupported speed", but only
 *      when the loader is built with logging
 *   p  printk() a line to the log port, with or without logging
 *   c  write through a null pointer: the MPU stops the sketch and the green
 *      LED blinks 4 fast + 4 slow. The fault dump stays in the USB buffer,
 *      so read it with a debugger. Press reset to recover.
 */

#include <Arduino.h>
#include <Wire.h>

static volatile uint32_t *volatile null_ptr;

static void print_menu(void) {
	Serial.println("l: log an I2C driver error");
	Serial.println("p: printk() a line");
	Serial.println("c: crash (null pointer write)");
}

void setup() {
	Serial.begin(115200);
	unsigned long start = millis();
	while (!Serial && millis() - start < 5000) {
		delay(100);
	}

	Serial.println("RAKwireless Debug Check");
	Serial.println("------------------------------------------------------");
	if (IS_ENABLED(CONFIG_LOG)) {
		Serial.println("Loader logging: on");
	} else {
		Serial.println("Loader logging: off, build the loader with the debug snippet");
	}

	Wire.begin();
	print_menu();
}

void loop() {
	if (!Serial.available()) {
		delay(10);
		return;
	}

	switch (Serial.read()) {
	case 'l':
		Serial.println("Setting the I2C clock to 1 MHz...");
		Wire.setClock(1000000);
		break;
	case 'p':
		printk("debug_check: hello from printk\n");
		Serial.println("printk() done");
		break;
	case 'c':
		Serial.println("Writing through a null pointer...");
		delay(100);
		*null_ptr = 0;
		break;
	case '\r':
	case '\n':
		break;
	default:
		print_menu();
		break;
	}
}
