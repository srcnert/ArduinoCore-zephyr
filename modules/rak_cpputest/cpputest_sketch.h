/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// Include this once, in the .ino: it defines setup() and loop(), which run every TEST()

#pragma once

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/PlatformSpecificFunctions.h>
#include <CppUTest/TestHarness.h>

#include <Arduino.h>

extern "C" {
#include <zephyr/debug/gcov.h>
}

static void cpputest_serial_puts(const char *str, PlatformSpecificFile) {
	Serial.print(str);
}

void setup() {
	Serial.begin(115200);
	// USB serial drops output until the host opens the port
	while (!Serial) {
	}
	// Twister clears its input buffer up to 100 ms after opening the port
	delay(1000);
	PlatformSpecificFPuts = cpputest_serial_puts;

	// -b runs the tests in the order they are written
	const char *args[] = {"cpputest", "-v", "-b"};
	int failures = CommandLineTestRunner::RunAllTests(3, args);

	Serial.println(failures == 0 ? "PROJECT EXECUTION SUCCESSFUL" : "PROJECT EXECUTION FAILED");

	// Twister reads the coverage data from the console after the result line;
	// empty without CONFIG_COVERAGE. Serial.flush() never returns on mps2/an386.
	delay(100);
	gcov_coverage_dump();
}

void loop() {
	delay(1000);
}
