/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <string.h>

// Records what the RAK layer passes on, instead of driving a UART
class FakeRAKSerial : public rak::ZephyrRAKSerial {
public:
	using rak::ZephyrRAKSerial::begin;

	void begin(unsigned long baudrate, uint16_t config) override {
		last_baudrate = baudrate;
		last_config = config;
	}

	size_t write(const uint8_t *buffer, size_t size) override {
		if (size > sizeof(out) - 1) {
			size = sizeof(out) - 1;
		}
		memcpy(out, buffer, size);
		out[size] = '\0';
		written = size;
		return size;
	}

	unsigned long last_baudrate = 0;
	uint16_t last_config = 0;
	char out[300] = {};
	size_t written = 0;

protected:
	const struct device *getUartDevice() const override {
		return nullptr;
	}
};

TEST_GROUP(RakSerial) {
	FakeRAKSerial serial;
};

TEST(RakSerial, begin_records_mode) {
	CHECK(serial.getMode() == rak::RAK_DEFAULT_MODE);

	serial.begin(9600, rak::RAK_AT_MODE);
	UNSIGNED_LONGS_EQUAL(9600, serial.last_baudrate);
	LONGS_EQUAL(SERIAL_8N1, serial.last_config);
	CHECK(serial.getMode() == rak::RAK_AT_MODE);

	serial.begin(115200, SERIAL_8E1, rak::RAK_CUSTOM_MODE);
	UNSIGNED_LONGS_EQUAL(115200, serial.last_baudrate);
	LONGS_EQUAL(SERIAL_8E1, serial.last_config);
	CHECK(serial.getMode() == rak::RAK_CUSTOM_MODE);
}

TEST(RakSerial, printf) {
	LONGS_EQUAL(9, serial.printf("%s=%d", "value", -42));
	STRCMP_EQUAL("value=-42", serial.out);
	LONGS_EQUAL(4, serial.printf("%04X", 0xAB));
	STRCMP_EQUAL("00AB", serial.out);
}

TEST(RakSerial, printf_truncates_long_output) {
	LONGS_EQUAL(255, serial.printf("%300s", "end"));
	LONGS_EQUAL(255, serial.written);
	LONGS_EQUAL(255, strlen(serial.out));
}

TEST(RakSerial, at_console_stubs) {
	serial.lock(true);
	serial.lock(false);
	CHECK_FALSE(serial.password("1234", 4));
	CHECK_FALSE(serial.password(String("1234")));
}

TEST(RakSerial, baudrate_without_device) {
	UNSIGNED_LONGS_EQUAL(0, serial.getBaudrate());
}

TEST(RakSerial, serial_object) {
	// cpputest_sketch.h opens Serial at 115200 without a mode. A UART that
	// cannot report its rate, like the one in QEMU's mps2/an386, returns 0.
	uint32_t baudrate = Serial.getBaudrate();

	CHECK(Serial.getMode() == rak::RAK_DEFAULT_MODE);
	CHECK(baudrate == 0 || baudrate == 115200);
}
