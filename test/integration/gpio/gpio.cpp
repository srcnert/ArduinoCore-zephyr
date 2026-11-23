/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// Digital I/O and interrupts over the jumper (jumper.h)

#include "jumper.h"

// count LOW-HIGH-LOW pulses, slow enough for the interrupt to keep up
static void pulse(int count) {
	for (int i = 0; i < count; i++) {
		digitalWrite(PIN_A, HIGH);
		delayMicroseconds(100);
		digitalWrite(PIN_A, LOW);
		delayMicroseconds(100);
	}
}

static void check_writeRead(pin_size_t out, pin_size_t in) {
	pinMode(in, INPUT);
	pinMode(out, OUTPUT);
	drive(out, HIGH);
	LONGS_EQUAL(HIGH, digitalRead(in));
	drive(out, LOW);
	LONGS_EQUAL(LOW, digitalRead(in));
}

// With both pins inputs, only the pull resistor sets the level
static void check_pull(pin_size_t pulled, pin_size_t other) {
	pinMode(other, INPUT);
	pinMode(pulled, INPUT_PULLUP);
	delayMicroseconds(10);
	LONGS_EQUAL(HIGH, digitalRead(pulled));
	LONGS_EQUAL(HIGH, digitalRead(other));
	pinMode(pulled, INPUT_PULLDOWN);
	delayMicroseconds(10);
	LONGS_EQUAL(LOW, digitalRead(pulled));
	LONGS_EQUAL(LOW, digitalRead(other));
}

static void check_edges(PinStatus mode, unsigned long expected) {
	attachInterrupt(digitalPinToInterrupt(PIN_B), count_call, mode);
	pulse(10);
	LONGS_EQUAL(expected, calls);
}

// A level interrupt fires for as long as the level is there, so the handler
// takes it away
static void clear_high() {
	calls++;
	digitalWrite(PIN_A, LOW);
}

static void clear_low() {
	calls++;
	digitalWrite(PIN_A, HIGH);
}

TEST_GROUP_BASE(Digital, Jumper){};

TEST(Digital, writeRead) {
	check_writeRead(PIN_A, PIN_B);
	check_writeRead(PIN_B, PIN_A);
}

TEST(Digital, pull) {
	check_pull(PIN_A, PIN_B);
	check_pull(PIN_B, PIN_A);
}

TEST_GROUP_BASE(Interrupt, Jumper){};

TEST(Interrupt, rising) {
	check_edges(RISING, 10);
}

TEST(Interrupt, falling) {
	check_edges(FALLING, 10);
}

TEST(Interrupt, change) {
	check_edges(CHANGE, 20);
}

TEST(Interrupt, detach) {
	check_edges(CHANGE, 20);
	detachInterrupt(digitalPinToInterrupt(PIN_B));
	pulse(10);
	LONGS_EQUAL(20, calls);
}

TEST(Interrupt, reattach) {
	check_edges(RISING, 10);
	detachInterrupt(digitalPinToInterrupt(PIN_B));
	calls = 0;
	check_edges(FALLING, 10);
}

TEST(Interrupt, high) {
	attachInterrupt(digitalPinToInterrupt(PIN_B), clear_high, HIGH);
	drive(PIN_A, HIGH);
	LONGS_EQUAL(1, calls);
	LONGS_EQUAL(LOW, digitalRead(PIN_B));
}

TEST(Interrupt, low) {
	drive(PIN_A, HIGH);
	attachInterrupt(digitalPinToInterrupt(PIN_B), clear_low, LOW);
	drive(PIN_A, LOW);
	LONGS_EQUAL(1, calls);
	LONGS_EQUAL(HIGH, digitalRead(PIN_B));
}

TEST(Interrupt, detachLevel) {
	attachInterrupt(digitalPinToInterrupt(PIN_B), clear_high, HIGH);
	detachInterrupt(digitalPinToInterrupt(PIN_B));
	drive(PIN_A, HIGH);
	LONGS_EQUAL(0, calls);
}
