/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// Every bus at once, as a sketch uses them: Serial1 and Serial2 stream to each
// other in both directions, loop() checks SPI loopback transfers and SHTC3 ID
// reads in between, and a PWM on D2 interrupts on D3. The wiring of the
// serial, gpio, spi and i2c tests: TXD0 (pin 11) to RXD1 (pin 34), TXD1
// (pin 33) to RXD0 (pin 12), IO3 (pin 31) to IO4 (pin 32), SPI_MISO (pin 27)
// to SPI_MOSI (pin 28), and a RAK1901 in a sensor slot.

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <CppUTest/TestHarness.h>
#include <stdio.h>

using arduino::ZephyrSerial;

constexpr unsigned long DURATION_MS = 2000;
constexpr unsigned long BAUDRATE = 115200;
constexpr pin_size_t PWM_OUT = WB_IO3;
constexpr pin_size_t PWM_IN = WB_IO4;
// The variant gives all its PWM channels the same period
constexpr unsigned long PWM_PERIOD_NS = DT_PHA_BY_IDX(DT_PATH(zephyr_user), pwms, 0, period);
constexpr uint8_t SHTC3 = 0x70;
constexpr size_t SPI_SIZE = 256;
constexpr uint32_t SPI_HZ = 8000000;

// One direction of the UART stream, a byte counter as the data
struct Link {
	ZephyrSerial &tx;
	ZephyrSerial &rx;
	uint32_t sent;
	uint32_t received;
	uint32_t errors;
};

static uint8_t spi_sent[SPI_SIZE];
static uint8_t spi_buf[SPI_SIZE];
static volatile uint32_t edges;

static void count_edge() {
	edges++;
}

// Only what fits, so loop() never waits for the UART
static void send(Link &link) {
	uint8_t buf[64];
	size_t size = MIN(sizeof(buf), (size_t)MAX(link.tx.availableForWrite(), 0));

	for (size_t i = 0; i < size; i++) {
		buf[i] = link.sent + i;
	}
	link.sent += link.tx.write(buf, size);
}

static void receive(Link &link) {
	int c;

	while ((c = link.rx.read()) >= 0) {
		if (c != (uint8_t)link.received) {
			link.errors++;
		}
		link.received++;
	}
}

// Returns false if the bytes came back wrong, adds the transfer time to us
static bool spi_round(uint32_t round, unsigned long &us) {
	for (size_t i = 0; i < SPI_SIZE; i++) {
		spi_sent[i] = round + i * 7;
	}
	memcpy(spi_buf, spi_sent, SPI_SIZE);

	SPI.beginTransaction(SPISettings(SPI_HZ, MSBFIRST, SPI_MODE0));
	unsigned long start = micros();
	SPI.transfer(spi_buf, SPI_SIZE);
	us += micros() - start;
	SPI.endTransaction();
	return memcmp(spi_sent, spi_buf, SPI_SIZE) == 0;
}

static uint8_t shtc3_command(uint16_t code) {
	Wire.beginTransmission(SHTC3);
	Wire.write(code >> 8);
	Wire.write(code & 0xFF);
	return Wire.endTransmission();
}

// Polynomial 0x31, initial value 0xFF (SHTC3 datasheet)
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

static bool shtc3_id_ok() {
	uint8_t word[3];

	if (shtc3_command(0xEFC8) != 0 || Wire.requestFrom(SHTC3, 3) != 3) {
		return false;
	}
	for (uint8_t &value : word) {
		value = Wire.read();
	}
	return crc8(word, 2) == word[2] && ((word[0] << 8 | word[1]) & 0x083F) == 0x0807;
}

TEST_BASE(Buses) {
	void setup() override;
	void teardown() override;
};

void Buses::setup() {
	Serial1.begin(BAUDRATE);
	Serial2.begin(BAUDRATE);
	while (Serial1.read() >= 0 || Serial2.read() >= 0) {
	}
	SPI.begin();
	Wire.begin();
	LONGS_EQUAL_TEXT(0, shtc3_command(0x3517), "RAK1901 in a sensor slot");
	delayMicroseconds(240);
	pinMode(PWM_IN, INPUT);
}

void Buses::teardown() {
	analogWrite(PWM_OUT, 0);
	detachInterrupt(digitalPinToInterrupt(PWM_IN));
	pinMode(PWM_IN, INPUT);
	pinMode(PWM_OUT, INPUT);
}

TEST_GROUP_BASE(Load, Buses){};

TEST(Load, allBuses) {
	Link links[] = {{Serial1, Serial2, 0, 0, 0}, {Serial2, Serial1, 0, 0, 0}};
	uint32_t rounds = 0;
	uint32_t spi_errors = 0;
	uint32_t i2c_errors = 0;
	unsigned long spi_us = 0;

	edges = 0;
	attachInterrupt(digitalPinToInterrupt(PWM_IN), count_edge, RISING);
	analogWrite(PWM_OUT, 128);

	unsigned long start = millis();
	uint32_t first_edges = edges;
	while (millis() - start < DURATION_MS) {
		for (Link &link : links) {
			send(link);
			receive(link);
		}
		if (!spi_round(rounds, spi_us)) {
			spi_errors++;
		}
		if (!shtc3_id_ok()) {
			i2c_errors++;
		}
		rounds++;
	}
	unsigned long elapsed = millis() - start;
	uint32_t pwm_edges = edges - first_edges;
	analogWrite(PWM_OUT, 0);

	// The last bytes still on their way
	unsigned long wait = millis();
	while ((links[0].received < links[0].sent || links[1].received < links[1].sent) &&
		   millis() - wait < 500) {
		for (Link &link : links) {
			receive(link);
		}
	}

	char text[96];
	snprintf(text, sizeof(text), "%lu rounds, UART %lu and %lu bytes, %lu PWM edges in %lu ms",
			 (unsigned long)rounds, (unsigned long)links[0].sent, (unsigned long)links[1].sent,
			 (unsigned long)pwm_edges, elapsed);
	UT_PRINT(text);

	// Each direction at 80 % of the line rate or more: 10 bits per byte
	const uint32_t min_bytes = BAUDRATE / 10 * DURATION_MS / 1000 * 8 / 10;
	const char *names[] = {"Serial1 to Serial2", "Serial2 to Serial1"};
	for (size_t i = 0; i < ARRAY_SIZE(links); i++) {
		LONGS_EQUAL_TEXT(0, links[i].errors, names[i]);
		LONGS_EQUAL_TEXT(links[i].sent, links[i].received, names[i]);
		CHECK_COMPARE_TEXT(links[i].sent, >=, min_bytes, names[i]);
	}

	LONGS_EQUAL_TEXT(0, spi_errors, "SPI");
	CHECK_COMPARE_TEXT(spi_us, >=, rounds * (SPI_SIZE * 8 * 1000000 / SPI_HZ), "SPI wire time");
	LONGS_EQUAL_TEXT(0, i2c_errors, "I2C");

	// The PWM runs on its own clock, so 1 % and an edge at each end
	const uint32_t expected = (uint64_t)elapsed * 1000000 / PWM_PERIOD_NS;
	CHECK_COMPARE_TEXT(pwm_edges, >=, expected - expected / 100 - 2, "PWM interrupts");
	CHECK_COMPARE_TEXT(pwm_edges, <=, expected + expected / 100 + 2, "PWM interrupts");
}
