/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// SPI with MISO wired to MOSI, so every byte comes back as it went out, and
// SPI_CS and SPI_CLK wired to IO5 and IO6, to read them back.
// RAK4631: SPI_MISO (pin 27) to SPI_MOSI (pin 28), SPI_CS (pin 25) to IO5
// (pin 37), SPI_CLK (pin 26) to IO6 (pin 38).
//
// A loopback can't tell the bit order or the clock phase and polarity apart,
// both ends use the same settings. And an in-place transfer that fails leaves
// the buffer as it was, so the buffer tests also check that the transfer took
// the time its bytes need on the wire.

#include <Arduino.h>
#include <SPI.h>
#include <CppUTest/TestHarness.h>

constexpr size_t SIZE = 1024;
constexpr unsigned long CYCLE_US = DIV_ROUND_UP(1000000, CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC);
constexpr pin_size_t CS_SENSE = WB_IO5;
constexpr pin_size_t SCK_SENSE = WB_IO6;

static uint8_t sent[SIZE];
static uint8_t buf[SIZE];
static volatile unsigned long edges;

static void count_edge() {
	edges++;
}

static void send(size_t size, SPISettings settings) {
	SPI.beginTransaction(settings);
	SPI.transfer(buf, size);
	SPI.endTransaction();
}

// SCK rests at the clock polarity: LOW in modes 0 and 1, HIGH in 2 and 3.
// SCK_SENSE pulls the other way, so a floating SCK reads wrong too.
static PinStatus set_idle_sense(SPIMode mode) {
	bool high = mode == SPI_MODE2 || mode == SPI_MODE3;

	pinMode(SCK_SENSE, high ? INPUT_PULLDOWN : INPUT_PULLUP);
	return high ? HIGH : LOW;
}

// 8 clocks per byte
static unsigned long wire_us(size_t size, uint32_t hz) {
	return (unsigned long)(size * 8ULL * 1000000 / hz);
}

// Returns how long the transfer took
static unsigned long check_transfer(size_t size, SPISettings settings) {
	unsigned long hz = settings.getClockFreq();
	char text[48];

	for (size_t i = 0; i < size; i++) {
		sent[i] = i * 7 + 3;
	}
	memcpy(buf, sent, size);

	SPI.beginTransaction(settings);
	unsigned long start = micros();
	SPI.transfer(buf, size);
	unsigned long elapsed = micros() - start;
	SPI.endTransaction();

	// Not MEMCMP_EQUAL(): its message is too big for the heap left over
	for (size_t i = 0; i < size; i++) {
		if (buf[i] != sent[i]) {
			snprintf(text, sizeof(text), "byte %zu of %zu at %lu Hz", i, size, hz);
			BYTES_EQUAL_TEXT(sent[i], buf[i], text);
		}
	}
	snprintf(text, sizeof(text), "%zu bytes at %lu Hz", size, hz);
	CHECK_COMPARE_TEXT(elapsed + CYCLE_US, >=, wire_us(size, hz), text);
	return elapsed;
}

TEST_BASE(Loopback) {
	void setup() override;
	void teardown() override;
};

void Loopback::setup() {
	SPI.begin();
}

void Loopback::teardown() {
	detachInterrupt(digitalPinToInterrupt(CS_SENSE));
	detachInterrupt(digitalPinToInterrupt(SCK_SENSE));
	pinMode(CS_SENSE, INPUT);
	pinMode(SCK_SENSE, INPUT);
}

TEST_GROUP_BASE(SPI, Loopback){};

// Runs first (-b runs the TESTs in file order), before any
// beginTransaction(): begin() alone gives working default settings on the
// AVR and SAMD cores. This core needs a beginTransaction() first.
IGNORE_TEST(SPI, noTransaction) {
	BYTES_EQUAL(0xA5, SPI.transfer(0xA5));
}

TEST(SPI, transfer) {
	constexpr uint8_t VALUES[] = {0x00, 0xFF, 0x55, 0xAA, 0x01, 0x80};

	SPI.beginTransaction(SPISettings());
	for (uint8_t value : VALUES) {
		BYTES_EQUAL_TEXT(value, SPI.transfer(value), "MISO-MOSI jumper");
	}
	SPI.endTransaction();
}

TEST(SPI, buffer) {
	constexpr size_t SIZES[] = {1, 2, 255, SIZE};

	for (size_t size : SIZES) {
		check_transfer(size, SPISettings(1000000, MSBFIRST, SPI_MODE0));
	}
}

TEST(SPI, modes) {
	constexpr SPIMode MODES[] = {SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3};

	for (SPIMode mode : MODES) {
		check_transfer(256, SPISettings(1000000, MSBFIRST, mode));
	}
}

TEST(SPI, lsbFirst) {
	check_transfer(256, SPISettings(1000000, LSBFIRST, SPI_MODE0));
}

// Not faster than asked for, and at 1 MHz not much slower either
TEST(SPI, clock) {
	constexpr uint32_t CLOCKS[] = {2000000, 4000000, 8000000};

	unsigned long elapsed = check_transfer(SIZE, SPISettings(1000000, MSBFIRST, SPI_MODE0));
	CHECK_COMPARE(elapsed, <=, 2 * wire_us(SIZE, 1000000) + 5000);
	for (uint32_t hz : CLOCKS) {
		check_transfer(SIZE, SPISettings(hz, MSBFIRST, SPI_MODE0));
	}
}

// The SD card speed before its initialization
TEST(SPI, slowClock) {
	unsigned long elapsed = check_transfer(SIZE, SPISettings(400000, MSBFIRST, SPI_MODE0));
	CHECK_COMPARE(elapsed, <=, 2 * wire_us(SIZE, 400000) + 5000);
}

// SS belongs to the sketch: it reaches SPI_CS, and the library leaves it
// where the sketch put it, during a transfer too
TEST(SPI, chipSelect) {
	pinMode(CS_SENSE, INPUT);
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);
	delayMicroseconds(10);
	LONGS_EQUAL(HIGH, digitalRead(CS_SENSE));
	digitalWrite(SS, LOW);
	delayMicroseconds(10);
	LONGS_EQUAL(LOW, digitalRead(CS_SENSE));

	edges = 0;
	attachInterrupt(digitalPinToInterrupt(CS_SENSE), count_edge, CHANGE);
	send(64, SPISettings(1000000, MSBFIRST, SPI_MODE0));
	LONGS_EQUAL(0, edges);
	LONGS_EQUAL(LOW, digitalRead(CS_SENSE));
	digitalWrite(SS, HIGH);
}

// SCK moves during a transfer only. Its edges come faster than the interrupt
// can count them, so any number above 0 will do.
TEST(SPI, clockEdges) {
	pinMode(SCK_SENSE, INPUT);
	send(1, SPISettings(1000000, MSBFIRST, SPI_MODE0));
	edges = 0;
	attachInterrupt(digitalPinToInterrupt(SCK_SENSE), count_edge, CHANGE);
	delay(10);
	LONGS_EQUAL_TEXT(0, edges, "idle");
	send(8, SPISettings(1000000, MSBFIRST, SPI_MODE0));
	CHECK_COMPARE_TEXT(edges, >, 0UL, "transfer");
}

// After a transfer in each mode
TEST(SPI, clockIdle) {
	constexpr SPIMode MODES[] = {SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3};
	constexpr const char *NAMES[] = {"mode 0", "mode 1", "mode 2", "mode 3"};

	for (SPIMode mode : MODES) {
		PinStatus idle = set_idle_sense(mode);

		send(1, SPISettings(1000000, MSBFIRST, mode));
		delayMicroseconds(10);
		LONGS_EQUAL_TEXT(idle, digitalRead(SCK_SENSE), NAMES[mode]);
	}
}

// Already after beginTransaction(), as on the AVR and SAMD cores: the sketch
// pulls CS low next, and a device can take an SCK edge then for a clock
// edge. nrf-spim rejects a transfer without buffers before it configures, so
// SCK reaches the new idle level only in the first transfer.
IGNORE_TEST(SPI, clockIdleAtBegin) {
	send(1, SPISettings(1000000, MSBFIRST, SPI_MODE0));
	PinStatus idle = set_idle_sense(SPI_MODE2);

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE2));
	delayMicroseconds(10);
	PinStatus level = digitalRead(SCK_SENSE);
	SPI.endTransaction();
	LONGS_EQUAL(idle, level);
}

TEST(SPI, transfer16) {
	SPI.beginTransaction(SPISettings());
	LONGS_EQUAL(0x1234, SPI.transfer16(0x1234));
	LONGS_EQUAL(0xA55A, SPI.transfer16(0xA55A));
	SPI.endTransaction();
}

// Runs last
TEST(SPI, end) {
	SPI.end();
	SPI.begin();
	check_transfer(256, SPISettings(1000000, MSBFIRST, SPI_MODE0));
}
