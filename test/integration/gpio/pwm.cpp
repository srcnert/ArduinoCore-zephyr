/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// analogWrite() over the jumper (jumper.h), and pulseIn() measuring it

#include "jumper.h"

// PIN_A's PWM channel runs at 500 Hz (variant overlay, pwms)
constexpr unsigned long PWM_PERIOD_US = 2000;

// A new duty cycle starts with the next period
static void check_pwm(unsigned long high) {
	delay(5);
	Signal signal = measure(50);
	CHECK_COMPARE(signal.period, >=, PWM_PERIOD_US - 10);
	CHECK_COMPARE(signal.period, <=, PWM_PERIOD_US + 10);
	CHECK_COMPARE(signal.high, >=, high - 20);
	CHECK_COMPARE(signal.high, <=, high + 20);
}

static void check_duty(int value) {
	analogWrite(PIN_A, value);
	check_pwm(PWM_PERIOD_US * value / 255);
}

static void check_constant(int value, PinStatus level) {
	analogWrite(PIN_A, value);
	calls = 0;
	attachInterrupt(digitalPinToInterrupt(PIN_B), count_call, CHANGE);
	delay(10);
	detachInterrupt(digitalPinToInterrupt(PIN_B));
	LONGS_EQUAL(0, calls);
	LONGS_EQUAL(level, digitalRead(PIN_B));
}

static void check_pulseIn(int value, uint8_t state) {
	const unsigned long high = PWM_PERIOD_US * value / 255;
	const unsigned long expected = state == HIGH ? high : PWM_PERIOD_US - high;

	analogWrite(PIN_A, value);
	delay(5);
	unsigned long length = pulseIn(PIN_B, state, 10000);
	CHECK_COMPARE(length, >=, expected - 2 * TICK_US);
	CHECK_COMPARE(length, <=, expected + 2 * TICK_US);
}

// The other files drive PIN_A as a GPIO, which only works with the PWM
// stopped (see PWM.constant)
struct PwmJumper : Jumper {
	void teardown() override {
		analogWrite(PIN_A, 0);
		Jumper::teardown();
	}
};

TEST_GROUP_BASE(PWM, PwmJumper){};

TEST(PWM, duty) {
	check_duty(64);
	check_duty(128);
	check_duty(192);
}

TEST(PWM, resolution) {
	analogWriteResolution(12);
	analogWrite(PIN_A, 1024);
	analogWriteResolution(8);
	check_pwm(PWM_PERIOD_US * 1024 / 4095);
}

// 0 and full scale stop the PWM and hold the pin (pwm_nrfx.c)
TEST(PWM, constant) {
	check_constant(0, LOW);
	check_constant(255, HIGH);
}

// On nRF pinMode() does not take the pin back from the PWM peripheral,
// analogWrite(pin, 0) has to stop it first
TEST(PWM, digitalWrite) {
	analogWrite(PIN_A, 128);
	delay(5);
	analogWrite(PIN_A, 0);
	pinMode(PIN_A, OUTPUT);
	drive(PIN_A, LOW);
	attachInterrupt(digitalPinToInterrupt(PIN_B), count_call, CHANGE);
	delay(10);
	LONGS_EQUAL(0, calls);
	LONGS_EQUAL(LOW, digitalRead(PIN_B));
	drive(PIN_A, HIGH);
	LONGS_EQUAL(HIGH, digitalRead(PIN_B));
}

TEST_GROUP_BASE(PulseIn, PwmJumper){};

TEST(PulseIn, high) {
	check_pulseIn(64, HIGH);
	check_pulseIn(192, HIGH);
}

TEST(PulseIn, low) {
	check_pulseIn(64, LOW);
	check_pulseIn(192, LOW);
}

// The timeout is in microseconds
TEST(PulseIn, timeout) {
	analogWrite(PIN_A, 0);

	unsigned long start = millis();
	unsigned long length = pulseIn(PIN_B, HIGH, 5000);
	unsigned long elapsed = millis() - start;

	LONGS_EQUAL(0, length);
	CHECK_COMPARE(elapsed, <, 50UL);
}
