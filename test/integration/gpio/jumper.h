/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// D2 and D3 against each other, with one jumper on the WisBlock connector:
// IO3 (pin 31) to IO4 (pin 32). PIN_A drives and PIN_B reads, unless a test
// says otherwise. Not D0/D1: D1 is the RAK19007's 3V3_S power enable.

#pragma once

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

constexpr pin_size_t PIN_A = WB_IO3;
constexpr pin_size_t PIN_B = WB_IO4;

// tone() and pulseIn() count kernel ticks
constexpr unsigned long TICK_US = 1000000 / CONFIG_SYS_CLOCK_TICKS_PER_SEC;

inline volatile unsigned long calls;

inline void count_call() {
	calls++;
}

inline void drive(pin_size_t pin, PinStatus status) {
	digitalWrite(pin, status);
	delayMicroseconds(10);
}

struct Signal {
	unsigned long period;
	unsigned long high;
};

inline volatile unsigned long rises;
inline volatile unsigned long falls;
inline volatile unsigned long first_rise;
inline volatile unsigned long last_rise;
inline volatile unsigned long high_sum;

inline void time_edge() {
	unsigned long now = micros();

	if (digitalRead(PIN_B) == HIGH) {
		if (rises++ == 0) {
			first_rise = now;
		}
		last_rise = now;
	} else if (rises > 0) {
		high_sum += now - last_rise;
		falls++;
	}
}

// The signal on PIN_B, averaged over count periods: micros() moves in kernel
// clock steps (30.5 us), the average hides them
inline Signal measure(unsigned long count) {
	rises = 0;
	falls = 0;
	high_sum = 0;
	attachInterrupt(digitalPinToInterrupt(PIN_B), time_edge, CHANGE);

	unsigned long start = millis();
	while (rises <= count && millis() - start < 1000) {
	}
	detachInterrupt(digitalPinToInterrupt(PIN_B));

	CHECK_COMPARE(rises, >, count);
	CHECK_COMPARE(falls, >, 0UL);
	return {(last_rise - first_rise) / (rises - 1), high_sum / falls};
}

TEST_BASE(Jumper) {
	void setup() override;
	void teardown() override;
};

inline void Jumper::setup() {
	calls = 0;
	pinMode(PIN_B, INPUT);
	pinMode(PIN_A, OUTPUT);
}

// On nRF reconfiguring a pin also turns off its interrupt (gpio_nrfx.c),
// which detachInterrupt() leaves on. PIN_B first, before PIN_A lets the line
// float.
inline void Jumper::teardown() {
	noTone(PIN_A);
	detachInterrupt(digitalPinToInterrupt(PIN_B));
	pinMode(PIN_B, INPUT);
	pinMode(PIN_A, INPUT);
}
