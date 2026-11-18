/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// Serial1 and Serial2 against each other, with two jumpers on the WisBlock
// connector: TXD0 (pin 11) to RXD1 (pin 34) and TXD1 (pin 33) to RXD0 (pin 12).
// TEST(SerialN, ...) sends on SerialN (tx) and receives on the other port (rx).

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <zephyr/drivers/uart.h>

using arduino::ZephyrSerial;

constexpr unsigned long BAUDRATE = 115200;

// The devices behind Serial1 and Serial2, for uart_err_check()
static const struct device *const serial1_uart =
	DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), serials, 0));
static const struct device *const serial2_uart =
	DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), serials, 1));

// Drop what a failed test left on the lines: read until both are quiet for 10 ms
static void drain() {
	unsigned long quiet = millis();

	while (millis() - quiet < 10) {
		if (Serial1.read() >= 0 || Serial2.read() >= 0) {
			quiet = millis();
		}
	}
}

static void begin_ports(unsigned long baudrate) {
	Serial1.begin(baudrate);
	Serial2.begin(baudrate);
	drain();
}

static void write_pattern(ZephyrSerial &tx, int size) {
	uint8_t buf[64];

	for (int i = 0; i < size; i += sizeof(buf)) {
		for (size_t j = 0; j < sizeof(buf); j++) {
			buf[j] = i + j;
		}
		LONGS_EQUAL(sizeof(buf), tx.write(buf, sizeof(buf)));
	}
}

static void check_pattern(ZephyrSerial &rx, int size) {
	uint8_t buf[64];

	for (int i = 0; i < size; i += sizeof(buf)) {
		LONGS_EQUAL(sizeof(buf), rx.readBytes(buf, sizeof(buf)));
		for (size_t j = 0; j < sizeof(buf); j++) {
			BYTES_EQUAL(i + j, buf[j]);
		}
	}
}

static void check_readStringUntil(ZephyrSerial &tx, ZephyrSerial &rx) {
	tx.print("hello\n");
	STRCMP_EQUAL("hello", rx.readStringUntil('\n').c_str());
}

static void check_parseInt(ZephyrSerial &tx, ZephyrSerial &rx) {
	tx.print("1234 -56\n");
	LONGS_EQUAL(1234, rx.parseInt());
	LONGS_EQUAL(-56, rx.parseInt());
}

static void check_parseFloat(ZephyrSerial &tx, ZephyrSerial &rx) {
	tx.print("3.25\n");
	DOUBLES_EQUAL(3.25, rx.parseFloat(), 1e-6);
}

static void check_peek(ZephyrSerial &tx, ZephyrSerial &rx) {
	LONGS_EQUAL(-1, rx.peek());
	tx.print("ab");

	unsigned long start = millis();
	while (rx.available() < 2 && millis() - start < 100) {
	}
	LONGS_EQUAL(2, rx.available());
	LONGS_EQUAL('a', rx.peek());
	LONGS_EQUAL('a', rx.read());
	LONGS_EQUAL('b', rx.peek());
	LONGS_EQUAL('b', rx.read());
	LONGS_EQUAL(-1, rx.read());
}

// Nothing is read until all of it has arrived, so it must fit in the RX
// buffer (CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE)
static void check_bulk(ZephyrSerial &tx, ZephyrSerial &rx, int size) {
	write_pattern(tx, size);
	// 10 bits a byte, plus some slack
	delay(size * 10 * 1000 / BAUDRATE + 10);
	LONGS_EQUAL(size, rx.available());
	check_pattern(rx, size);
}

// Above the TX buffer, write() has to wait for the IRQ handler; what goes out
// meanwhile (size minus the TX buffer) must fit in the RX buffer
static void check_write(ZephyrSerial &tx, ZephyrSerial &rx, int size) {
	write_pattern(tx, size);
	check_pattern(rx, size);
}

static void check_print(ZephyrSerial &tx, ZephyrSerial &rx) {
	tx.print(1234);
	tx.print(' ');
	tx.print(-56);
	tx.print(' ');
	tx.print(255, HEX);
	tx.print(' ');
	tx.println(3.25);
	STRCMP_EQUAL("1234 -56 FF 3.25\r", rx.readStringUntil('\n').c_str());
}

static void check_printf(ZephyrSerial &tx, ZephyrSerial &rx) {
	tx.printf("%d %d %X %.2f\n", 1234, -56, 255, 3.25);
	STRCMP_EQUAL("1234 -56 FF 3.25", rx.readStringUntil('\n').c_str());
}

// Never returns on the nRF UARTE: the IRQ handler disables the TX IRQ once the
// TX buffer is empty, and uart_irq_tx_complete() stays 0 from then on
static void check_flush(ZephyrSerial &tx, ZephyrSerial &rx) {
	write_pattern(tx, 512);
	tx.flush();
	LONGS_EQUAL(CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE, tx.availableForWrite());
	// Only the last byte can still be on its way
	delay(1);
	LONGS_EQUAL(512, rx.available());
	check_pattern(rx, 512);
}

// The other port sends nothing, so nothing arrives
static void check_setTimeout(ZephyrSerial &port) {
	char buf[4];

	port.setTimeout(100);
	unsigned long start = millis();
	size_t length = port.readBytes(buf, sizeof(buf));
	unsigned long elapsed = millis() - start;
	port.setTimeout(1000);

	LONGS_EQUAL(0, length);
	CHECK_COMPARE(elapsed, >=, 100);
	CHECK_COMPARE(elapsed, <, 150);
}

// Only queues: 64 bytes take 67 ms on the wire at 9600 baud
static void check_writeAsync(ZephyrSerial &tx, ZephyrSerial &rx) {
	begin_ports(9600);

	unsigned long start = millis();
	write_pattern(tx, 64);
	unsigned long elapsed = millis() - start;

	CHECK_COMPARE(elapsed, <, 10);
	check_pattern(rx, 64);
}

// The TX buffer empties by itself, without flush()
static void check_availableForWrite(ZephyrSerial &tx, ZephyrSerial &rx) {
	LONGS_EQUAL(CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE, tx.availableForWrite());
	write_pattern(tx, 512);
	// At most 64 bytes can have gone out yet
	CHECK_COMPARE(tx.availableForWrite(), <=, CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE - 512 + 64);
	check_pattern(rx, 512);
	LONGS_EQUAL(CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE, tx.availableForWrite());
}

// Both ports send at the same time, count times; a receiver that falls behind
// sets UART_ERROR_OVERRUN (1)
static void check_fullDuplex(ZephyrSerial &tx, ZephyrSerial &rx, unsigned long baudrate,
							 int count) {
	begin_ports(baudrate);
	(void)uart_err_check(serial1_uart);
	(void)uart_err_check(serial2_uart);

	for (int n = 0; n < count; n++) {
		write_pattern(tx, 512);
		write_pattern(rx, 512);
		delay(512 * 10 * 1000 / baudrate + 10);
		LONGS_EQUAL_TEXT(0, uart_err_check(serial1_uart), "Serial1 UART errors");
		LONGS_EQUAL_TEXT(0, uart_err_check(serial2_uart), "Serial2 UART errors");
		check_pattern(rx, 512);
		check_pattern(tx, 512);
	}
}

// A full RX buffer keeps what it has and drops the rest: zephyrSerial.cpp
// ignores what ring_buf_put() could not store
static void check_overflow(ZephyrSerial &tx, ZephyrSerial &rx) {
	const int size = CONFIG_ARDUINO_API_SERIAL_BUFFER_SIZE;

	write_pattern(tx, size + 64);
	delay((size + 64) * 10 * 1000 / BAUDRATE + 10);
	LONGS_EQUAL(size, rx.available());
	check_pattern(rx, size);
	LONGS_EQUAL(0, rx.available());
}

// Times the transfer: with the baudrate ignored, the data still gets through
static void check_baudrate(ZephyrSerial &tx, ZephyrSerial &rx) {
	static const unsigned long baudrates[] = {9600, 115200, 1000000};

	for (unsigned long baudrate : baudrates) {
		// 10 bits a byte
		unsigned long expected = 256 * 10 * 1000 / baudrate;

		begin_ports(baudrate);
		LONGS_EQUAL(baudrate, tx.getBaudrate());

		unsigned long start = millis();
		check_write(tx, rx, 256);
		unsigned long elapsed = millis() - start;

		CHECK_COMPARE(elapsed, >=, expected);
		CHECK_COMPARE(elapsed, <=, 2 * expected + 5);
	}
}

// end() deinitializes the UART (CONFIG_DEVICE_DEINIT_SUPPORT), begin() has to
// bring it back
static void check_end(ZephyrSerial &tx, ZephyrSerial &rx) {
	tx.end();
	tx.begin(BAUDRATE);
	drain();
	check_readStringUntil(tx, rx);
	check_readStringUntil(rx, tx);
}

TEST_GROUP(Serial1) {
	ZephyrSerial &tx = Serial1;
	ZephyrSerial &rx = Serial2;

	void setup() override {
		begin_ports(BAUDRATE);
	}
};

TEST(Serial1, readStringUntil) {
	check_readStringUntil(tx, rx);
}

TEST(Serial1, parseInt) {
	check_parseInt(tx, rx);
}

TEST(Serial1, parseFloat) {
	check_parseFloat(tx, rx);
}

TEST(Serial1, peek) {
	check_peek(tx, rx);
}

TEST(Serial1, bulk1k) {
	check_bulk(tx, rx, 1024);
}

TEST(Serial1, write1536) {
	check_write(tx, rx, 1536);
}

TEST(Serial1, print) {
	check_print(tx, rx);
}

TEST(Serial1, printf) {
	check_printf(tx, rx);
}

IGNORE_TEST(Serial1, flush) {
	check_flush(tx, rx);
}

TEST(Serial1, setTimeout) {
	check_setTimeout(tx);
}

TEST(Serial1, writeAsync) {
	check_writeAsync(tx, rx);
}

TEST(Serial1, availableForWrite) {
	check_availableForWrite(tx, rx);
}

TEST(Serial1, fullDuplex) {
	check_fullDuplex(tx, rx, BAUDRATE, 1);
}

// Can overrun Serial2's receiver: the nRF UARTE takes one byte per interrupt
IGNORE_TEST(Serial1, fullDuplex1M) {
	check_fullDuplex(tx, rx, 1000000, 10);
}

TEST(Serial1, overflow) {
	check_overflow(tx, rx);
}

TEST(Serial1, baudrate) {
	check_baudrate(tx, rx);
}

TEST_GROUP(Serial2) {
	ZephyrSerial &tx = Serial2;
	ZephyrSerial &rx = Serial1;

	void setup() override {
		begin_ports(BAUDRATE);
	}
};

TEST(Serial2, readStringUntil) {
	check_readStringUntil(tx, rx);
}

TEST(Serial2, parseInt) {
	check_parseInt(tx, rx);
}

TEST(Serial2, parseFloat) {
	check_parseFloat(tx, rx);
}

TEST(Serial2, peek) {
	check_peek(tx, rx);
}

TEST(Serial2, bulk1k) {
	check_bulk(tx, rx, 1024);
}

TEST(Serial2, write1536) {
	check_write(tx, rx, 1536);
}

TEST(Serial2, print) {
	check_print(tx, rx);
}

TEST(Serial2, printf) {
	check_printf(tx, rx);
}

IGNORE_TEST(Serial2, flush) {
	check_flush(tx, rx);
}

TEST(Serial2, setTimeout) {
	check_setTimeout(tx);
}

TEST(Serial2, writeAsync) {
	check_writeAsync(tx, rx);
}

TEST(Serial2, availableForWrite) {
	check_availableForWrite(tx, rx);
}

TEST(Serial2, fullDuplex) {
	check_fullDuplex(tx, rx, BAUDRATE, 1);
}

// Overruns Serial2's receiver: the nRF UARTE takes one byte per interrupt
IGNORE_TEST(Serial2, fullDuplex1M) {
	check_fullDuplex(tx, rx, 1000000, 10);
}

TEST(Serial2, overflow) {
	check_overflow(tx, rx);
}

TEST(Serial2, baudrate) {
	check_baudrate(tx, rx);
}

// Last: a port that does not come back would fail every test after it
TEST(Serial1, end) {
	check_end(tx, rx);
}

TEST(Serial2, end) {
	check_end(tx, rx);
}
