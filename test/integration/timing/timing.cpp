/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// The board's clocks against the host's: pytest/test_timing.py answers each
// "@clock" line with the host's clock in microseconds. No wiring.

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

// Catches clock configuration errors (e.g. 32000 Hz taken for 32768 Hz), not
// oscillator drift: an RC oscillator can be up to 1 % off
constexpr long TOLERANCE_PERCENT = 1;
constexpr unsigned long HOST_PERIOD_MS = 5000;
constexpr unsigned long CYCLE_US = DIV_ROUND_UP(1000000, CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC);
constexpr unsigned long TICK_US = DIV_ROUND_UP(1000000, CONFIG_SYS_CLOCK_TICKS_PER_SEC);

struct Stamp {
	long host_us;
	unsigned long us;
	unsigned long ms;
	unsigned long round_trip_us;
};

struct Elapsed {
	long host_us;
	long micros_us;
	long millis_us;
	long round_trips_us;
};

// Drop what a failed test left unread
static void drain() {
	delay(100);
	while (Serial.read() >= 0) {
	}
}

// The host answers somewhere within the round trip
static Stamp stamp() {
	unsigned long sent = micros();

	// CppUTest -v has already printed "TEST(...)" without a newline
	Serial.println("\n@clock");
	long host_us = Serial.readStringUntil('\n').toInt();
	Stamp now = {host_us, micros(), millis(), 0};

	now.round_trip_us = now.us - sent;
	CHECK_COMPARE_TEXT(now.round_trip_us, <, 100000UL, "host answer");
	return now;
}

static Elapsed measure() {
	drain();
	Stamp first = stamp();
	delay(HOST_PERIOD_MS);
	Stamp last = stamp();

	return {last.host_us - first.host_us, (long)(last.us - first.us),
			(long)(last.ms - first.ms) * 1000, (long)(first.round_trip_us + last.round_trip_us)};
}

static void check_elapsed(long board_us, long resolution_us, const Elapsed &elapsed) {
	long allowed =
		elapsed.round_trips_us + resolution_us + elapsed.host_us * TOLERANCE_PERCENT / 100;

	CHECK_COMPARE(labs(board_us - elapsed.host_us), <=, allowed);
}

// At least ms, at most two kernel ticks more
static void check_delay(unsigned long ms) {
	unsigned long start = micros();
	delay(ms);
	unsigned long elapsed = micros() - start;

	CHECK_COMPARE(elapsed + CYCLE_US, >=, ms * 1000);
	CHECK_COMPARE(elapsed, <=, ms * 1000 + 2 * TICK_US + CYCLE_US + 200);
}

// On the nRF k_busy_wait() counts CPU cycles, so this also compares the CPU
// clock with the kernel clock
static void check_delayMicroseconds(unsigned int us) {
	unsigned long start = micros();
	delayMicroseconds(us);
	unsigned long elapsed = micros() - start;

	CHECK_COMPARE(elapsed + CYCLE_US + 1, >=, us);
	CHECK_COMPARE(elapsed, <=, us + us * TOLERANCE_PERCENT / 100 + CYCLE_US + 20);
}

TEST_GROUP(Timing){};

TEST(Timing, micros) {
	Elapsed elapsed = measure();

	check_elapsed(elapsed.micros_us, 2 * CYCLE_US, elapsed);
}

TEST(Timing, millis) {
	Elapsed elapsed = measure();

	check_elapsed(elapsed.millis_us, 2000 + TICK_US, elapsed);
}

TEST(Timing, delay) {
	check_delay(1);
	check_delay(10);
	check_delay(100);
	check_delay(1000);
}

TEST(Timing, delayMicroseconds) {
	check_delayMicroseconds(10);
	check_delayMicroseconds(100);
	check_delayMicroseconds(1000);
	check_delayMicroseconds(10000);
	check_delayMicroseconds(100000);
}

// Read back to back for 100 ms: neither clock steps back, and both advance at
// the same rate
TEST(Timing, monotonic) {
	unsigned long first_us = micros();
	unsigned long first_ms = millis();
	unsigned long last_us = first_us;
	unsigned long last_ms = first_ms;

	while (last_us - first_us < 100000) {
		unsigned long us = micros();
		unsigned long ms = millis();

		// unsigned: a step back is a huge step forward
		CHECK_COMPARE(us - last_us, <, 1000000UL);
		CHECK_COMPARE(ms - last_ms, <, 1000UL);
		last_us = us;
		last_ms = ms;
	}

	long drift = (long)(last_ms - first_ms) * 1000 - (long)(last_us - first_us);
	CHECK_COMPARE(labs(drift), <=, (long)(2000 + TICK_US + CYCLE_US));
}
