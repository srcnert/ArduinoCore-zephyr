/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// analogRead() on every analog pin of the variant. An analog pin on the gpio
// test's jumper (IO3 to IO4) is driven from the other end, every other one is
// wired: even index (A0, A2, ...) to GND, odd index (A1, A3, ...) to VDD, so a
// swapped channel reads the wrong level. The sketch never drives an analog
// pin itself: a wired one would short, and on STM32 analogRead() switches the
// pin to analog mode anyway.
//
// RAK4631: A0 (AIN0, pin 21) to GND, A1 (AIN1, pin 22) and A3 (IO7, pin 24)
// to VDD, A2 is IO4 on the jumper.

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

#define ANALOG_PIN(node, prop, i)  A##i,
#define ANALOG_NAME(node, prop, i) "A" #i,

constexpr pin_size_t PINS[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), adc_pin_gpios, ANALOG_PIN)};
constexpr const char *NAMES[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), adc_pin_gpios, ANALOG_NAME)};

// At 12 bits. VDD is 92 % of the nRF's 3.6 V full scale, and an ADC with a
// reference below VDD reads 100 %
constexpr int HIGH_MIN = 4096 * 75 / 100;
constexpr int LOW_MAX = 4096 * 2 / 100;

// The other end of the gpio test's jumper, or the pin itself
static pin_size_t jumper_end(pin_size_t pin) {
	if (pin == WB_IO3) {
		return WB_IO4;
	}
	if (pin == WB_IO4) {
		return WB_IO3;
	}
	return pin;
}

// Drives a pin on the jumper to level, returns the level the pin reads
static PinStatus set_level(size_t index, PinStatus level) {
	pin_size_t end = jumper_end(PINS[index]);

	if (end == PINS[index]) {
		return index % 2 == 0 ? LOW : HIGH;
	}
	pinMode(end, OUTPUT);
	digitalWrite(end, level);
	return level;
}

static void check_level(size_t index, PinStatus level) {
	int value = analogRead(PINS[index]);

	CHECK_COMPARE_TEXT(value, >=, 0, NAMES[index]);
	if (level == HIGH) {
		CHECK_COMPARE_TEXT(value, >=, HIGH_MIN, NAMES[index]);
	} else {
		CHECK_COMPARE_TEXT(value, <=, LOW_MAX, NAMES[index]);
	}
}

TEST_BASE(Wired) {
	void setup() override;
	void teardown() override;
};

void Wired::setup() {
	pinMode(WB_IO3, INPUT);
	pinMode(WB_IO4, INPUT);
	analogReadResolution(12);
}

void Wired::teardown() {
	pinMode(WB_IO3, INPUT);
	pinMode(WB_IO4, INPUT);
	analogReadResolution(10);
}

TEST_GROUP_BASE(Analog, Wired){};

TEST(Analog, levels) {
	for (size_t i = 0; i < ARRAY_SIZE(PINS); i++) {
		check_level(i, set_level(i, HIGH));
		check_level(i, set_level(i, LOW));
	}
}

// analogRead() scales the sample to the resolution asked for
TEST(Analog, resolution) {
	constexpr int RESOLUTIONS[] = {8, 10, 12, 14, 16};
	// An odd index reads HIGH
	constexpr size_t INDEX = ARRAY_SIZE(PINS) > 1 ? 1 : 0;

	(void)set_level(INDEX, HIGH);
	int sample = analogRead(PINS[INDEX]);

	for (int bits : RESOLUTIONS) {
		char text[16];

		snprintf(text, sizeof(text), "%d bits", bits);
		analogReadResolution(bits);
		LONGS_EQUAL_TEXT(bits, analogReadResolution(), text);
		int value = analogRead(PINS[INDEX]);
		int value12 = bits < 12 ? value << (12 - bits) : value >> (bits - 12);
		CHECK_COMPARE_TEXT(abs(value12 - sample), <=, 32, text);
	}
}

// pinMode() makes the pin digital again after analogRead()
TEST(Analog, digitalRead) {
	constexpr PinStatus LEVELS[] = {HIGH, LOW};

	for (size_t i = 0; i < ARRAY_SIZE(PINS); i++) {
		for (PinStatus level : LEVELS) {
			PinStatus expected = set_level(i, level);

			(void)analogRead(PINS[i]);
			pinMode(PINS[i], INPUT);
			LONGS_EQUAL_TEXT(expected, digitalRead(PINS[i]), NAMES[i]);
		}
	}
}
