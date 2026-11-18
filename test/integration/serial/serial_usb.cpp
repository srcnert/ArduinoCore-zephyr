/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// Serial (USB CDC) against the host: pytest/test_serial.py answers the
// "@<command> <argument>" lines sent by request()

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

// Drop what a failed test left unread
static void drain() {
	delay(100);
	while (Serial.read() >= 0) {
	}
}

static void request(const char *command) {
	drain();

	// CppUTest -v has already printed "TEST(...)" without a newline
	Serial.print("\n@");
	Serial.println(command);
}

// Answer to "expect" and "pattern": "ok" if the next line matched
static void check_reply() {
	STRCMP_EQUAL("ok", Serial.readStringUntil('\n').c_str());
}

static void write_pattern(int size) {
	char buf[64];

	for (int i = 0; i < size; i += sizeof(buf)) {
		for (size_t j = 0; j < sizeof(buf); j++) {
			buf[j] = 'a' + (i + j) % 26;
		}
		LONGS_EQUAL(sizeof(buf), Serial.write(buf, sizeof(buf)));
	}
	Serial.println();
}

static void check_bulk(int size) {
	String command("bulk ");
	uint8_t buf[64];

	command += size;
	request(command.c_str());
	for (int i = 0; i < size; i += sizeof(buf)) {
		LONGS_EQUAL(sizeof(buf), Serial.readBytes(buf, sizeof(buf)));
		for (size_t j = 0; j < sizeof(buf); j++) {
			BYTES_EQUAL(i + j, buf[j]);
		}
	}
}

TEST_GROUP(SerialUSB){};

TEST(SerialUSB, readStringUntil) {
	request("echo hello");
	STRCMP_EQUAL("hello", Serial.readStringUntil('\n').c_str());
}

TEST(SerialUSB, parseInt) {
	request("echo 1234 -56");
	LONGS_EQUAL(1234, Serial.parseInt());
	LONGS_EQUAL(-56, Serial.parseInt());
}

TEST(SerialUSB, parseFloat) {
	request("echo 3.25");
	DOUBLES_EQUAL(3.25, Serial.parseFloat(), 1e-6);
}

TEST(SerialUSB, peek) {
	request("echo ab");

	unsigned long start = millis();
	while (Serial.available() < 3 && millis() - start < 1000) {
	}
	LONGS_EQUAL(3, Serial.available());
	LONGS_EQUAL('a', Serial.peek());
	LONGS_EQUAL('a', Serial.read());
	LONGS_EQUAL('b', Serial.peek());
}

// Fits in the RX buffer (CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE)
TEST(SerialUSB, bulk1k) {
	check_bulk(1024);
}

// Only passes if the sketch reads faster than the host sends
IGNORE_TEST(SerialUSB, bulk8k) {
	check_bulk(8192);
}

// More than the TX buffer, so write() has to wait for the IRQ handler
TEST(SerialUSB, write4k) {
	request("pattern 4096");
	write_pattern(4096);
	check_reply();
}

TEST(SerialUSB, print) {
	request("expect 1234 -56 FF 3.25");
	Serial.print(1234);
	Serial.print(' ');
	Serial.print(-56);
	Serial.print(' ');
	Serial.print(255, HEX);
	Serial.print(' ');
	Serial.println(3.25);
	check_reply();
}

TEST(SerialUSB, printf) {
	request("expect 1234 -56 FF 3.25");
	Serial.printf("%d %d %X %.2f\n", 1234, -56, 255, 3.25);
	check_reply();
}

TEST(SerialUSB, flush) {
	request("pattern 512");
	write_pattern(512);
	Serial.flush();
	LONGS_EQUAL(CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE, Serial.availableForWrite());
	check_reply();
}

TEST(SerialUSB, setTimeout) {
	char buf[4];

	drain();
	Serial.setTimeout(100);
	unsigned long start = millis();
	size_t length = Serial.readBytes(buf, sizeof(buf));
	unsigned long elapsed = millis() - start;
	Serial.setTimeout(1000);

	LONGS_EQUAL(0, length);
	CHECK_COMPARE(elapsed, >=, 100);
	CHECK_COMPARE(elapsed, <, 150);
}

// The rate the host opened the port with: 115200 for Twister
TEST(SerialUSB, getBaudrate) {
	LONGS_EQUAL(115200, Serial.getBaudrate());
}
