/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// tone() over the jumper (jumper.h)

#include "jumper.h"

// The period within 1 %, plus up to one tick per half period (Tone.cpp rounds
// them up to whole ticks), half of it high
static void check_tone(unsigned int frequency) {
	const unsigned long period = 1000000 / frequency;

	tone(PIN_A, frequency);
	Signal signal = measure(50);
	CHECK_COMPARE(signal.period, >=, period * 99 / 100);
	CHECK_COMPARE(signal.period, <=, period * 101 / 100 + 2 * TICK_US);
	CHECK_COMPARE(signal.high, >=, signal.period / 2 - TICK_US);
	CHECK_COMPARE(signal.high, <=, signal.period / 2 + TICK_US);
}

TEST_GROUP_BASE(Tone, Jumper){};

TEST(Tone, frequency440) {
	check_tone(440);
}

TEST(Tone, frequency1k) {
	check_tone(1000);
}

// 50 ms at 1 kHz: about 50 periods, then LOW
TEST(Tone, duration) {
	attachInterrupt(digitalPinToInterrupt(PIN_B), count_call, RISING);
	tone(PIN_A, 1000, 50);
	delay(100);
	CHECK_COMPARE(calls, >=, 49UL);
	CHECK_COMPARE(calls, <=, 51UL);
	LONGS_EQUAL(LOW, digitalRead(PIN_B));
}

TEST(Tone, noTone) {
	tone(PIN_A, 1000);
	delay(10);
	noTone(PIN_A);
	attachInterrupt(digitalPinToInterrupt(PIN_B), count_call, CHANGE);
	delay(10);
	LONGS_EQUAL(0, calls);
	LONGS_EQUAL(LOW, digitalRead(PIN_B));
}
