/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// Wire against a RAK1901 in a sensor slot of the base board: a Sensirion
// SHTC3 temperature and humidity sensor at 0x70. Its commands, CRC and
// timings are from the SHTC3 datasheet.
//
// The SHTC3 takes a STOP and a repeated START alike, so the stopBit
// arguments can't be told apart here.

#include <Arduino.h>
#include <Wire.h>
#include <CppUTest/TestHarness.h>

constexpr uint8_t SHTC3 = 0x70;
constexpr uint8_t ABSENT = 0x71;

constexpr uint16_t WAKEUP = 0x3517;
constexpr uint16_t READ_ID = 0xEFC8;
constexpr uint16_t MEASURE_STRETCHING = 0x7CA2; // temperature first
constexpr uint16_t MEASURE_POLLING = 0x7866;    // temperature first
constexpr unsigned long MEASURE_MAX_MS = 13;

constexpr unsigned long CYCLE_US = DIV_ROUND_UP(1000000, CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC);

static uint8_t command(uint16_t code) {
	Wire.beginTransmission(SHTC3);
	Wire.write(code >> 8);
	Wire.write(code & 0xFF);
	return Wire.endTransmission();
}

// Polynomial 0x31, initial value 0xFF
static uint8_t crc8(const uint8_t *data, size_t size) {
	uint8_t crc = 0xFF;

	for (size_t i = 0; i < size; i++) {
		crc ^= data[i];
		for (int bit = 0; bit < 8; bit++) {
			crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
		}
	}
	return crc;
}

// Words of 2 bytes and a CRC each, after requestFrom()
static void read_words(uint16_t *words, size_t count) {
	for (size_t i = 0; i < count; i++) {
		uint8_t word[3];

		for (uint8_t &value : word) {
			value = Wire.read();
		}
		BYTES_EQUAL_TEXT(crc8(word, 2), word[2], "CRC");
		words[i] = word[0] << 8 | word[1];
	}
}

// Room temperature and humidity, not the all-0 or all-1 words of a bad read
static void check_measurement(const uint16_t *words) {
	double celsius = -45 + 175.0 * words[0] / 65536;
	double humidity = 100.0 * words[1] / 65536;

	CHECK_COMPARE_TEXT(celsius, >=, 5.0, "temperature");
	CHECK_COMPARE_TEXT(celsius, <=, 45.0, "temperature");
	CHECK_COMPARE_TEXT(humidity, >=, 5.0, "humidity");
	CHECK_COMPARE_TEXT(humidity, <=, 95.0, "humidity");
}

// Returns how long requestFrom() took
static unsigned long read_id() {
	uint16_t id;

	LONGS_EQUAL(0, command(READ_ID));
	unsigned long start = micros();
	LONGS_EQUAL(3, Wire.requestFrom(SHTC3, 3));
	unsigned long elapsed = micros() - start;
	read_words(&id, 1);
	LONGS_EQUAL(0x0807, id & 0x083F);
	return elapsed;
}

TEST_BASE(Sensor) {
	void setup() override;
	void teardown() override;
};

void Sensor::setup() {
	Wire.begin();
	LONGS_EQUAL_TEXT(0, command(WAKEUP), "RAK1901 in a sensor slot");
	delayMicroseconds(240);
}

// Arduino's default
void Sensor::teardown() {
	Wire.setClock(100000);
}

TEST_GROUP_BASE(Wire, Sensor){};

// The I2C scanner way: an address without data
TEST(Wire, probe) {
	Wire.beginTransmission(SHTC3);
	LONGS_EQUAL(0, Wire.endTransmission());
}

// 2 is Arduino's code for a NACK on the address
TEST(Wire, absent) {
	Wire.beginTransmission(ABSENT);
	Wire.write(0);
	LONGS_EQUAL(2, Wire.endTransmission());
	LONGS_EQUAL(0, Wire.requestFrom(ABSENT, 3));
	LONGS_EQUAL(0, Wire.available());
}

TEST(Wire, readId) {
	read_id();
}

TEST(Wire, buffer) {
	LONGS_EQUAL(0, command(READ_ID));
	LONGS_EQUAL(3, Wire.requestFrom(SHTC3, 3));
	LONGS_EQUAL(3, Wire.available());

	int first = Wire.peek();
	LONGS_EQUAL(first, Wire.read());
	LONGS_EQUAL(2, Wire.available());
	Wire.read();
	Wire.read();
	LONGS_EQUAL(0, Wire.available());
	LONGS_EQUAL(-1, Wire.peek());
	LONGS_EQUAL(-1, Wire.read());
}

// The SHTC3 holds SCL low until the measurement is done
TEST(Wire, clockStretching) {
	uint16_t words[2];

	LONGS_EQUAL(0, command(MEASURE_STRETCHING));
	unsigned long start = millis();
	LONGS_EQUAL(6, Wire.requestFrom(SHTC3, 6));
	CHECK_COMPARE(millis() - start, >=, 5UL);
	read_words(words, 2);
	check_measurement(words);
}

// The SHTC3 doesn't take its address until the measurement is done
TEST(Wire, polling) {
	uint16_t words[2];

	LONGS_EQUAL(0, command(MEASURE_POLLING));
	LONGS_EQUAL(0, Wire.requestFrom(SHTC3, 6));
	delay(MEASURE_MAX_MS);
	LONGS_EQUAL(6, Wire.requestFrom(SHTC3, 6));
	read_words(words, 2);
	check_measurement(words);
}

// The address and 3 bytes are 36 clocks: 360 us at 100 kHz, 90 us at 400 kHz
TEST(Wire, clock) {
	Wire.setClock(100000);
	unsigned long slow = read_id();
	CHECK_COMPARE(slow + CYCLE_US, >=, 360UL);

	Wire.setClock(400000);
	unsigned long fast = read_id();
	CHECK_COMPARE(fast + CYCLE_US, >=, 90UL);
	CHECK_COMPARE(fast, <, 360UL);
}

// Runs last
TEST(Wire, end) {
	Wire.end();
	Wire.begin();
	LONGS_EQUAL(0, command(WAKEUP));
	delayMicroseconds(240);
	read_id();
}
