/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file zephyrRAKSerial.cpp
 * @brief Implementation of the RAK serial extensions.
 */

#include <stdarg.h>
#include <stdio.h>

#include <zephyr/drivers/uart.h>

#include <api/HardwareSerial.h>
#include <rak/zephyrRAKSerial.h>

namespace {

/* printf() formats into a buffer of this size on the stack; longer output is
 * truncated.
 */
constexpr size_t PRINTF_STACK_BUF_SIZE = 256;

} // anonymous namespace

void rak::ZephyrRAKSerial::begin(unsigned long baudrate, uint16_t config, RAK_SERIAL_MODE mode) {
	serial_mode_ = mode;
	begin(baudrate, config);
}

void rak::ZephyrRAKSerial::begin(unsigned long baudrate, RAK_SERIAL_MODE mode) {
	begin(baudrate, SERIAL_8N1, mode);
}

rak::RAK_SERIAL_MODE rak::ZephyrRAKSerial::getMode() const {
	return serial_mode_;
}

size_t rak::ZephyrRAKSerial::printf(const char *fmt, ...) {
	char buf[PRINTF_STACK_BUF_SIZE];
	va_list ap;

	va_start(ap, fmt);
	int len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (len < 0) {
		return 0;
	}

	size_t written = static_cast<size_t>(len);

	if (written > sizeof(buf) - 1) {
		written = sizeof(buf) - 1;
	}

	return write(reinterpret_cast<const uint8_t *>(buf), written);
}

void rak::ZephyrRAKSerial::lock(bool locked) {
	ARG_UNUSED(locked);
}

bool rak::ZephyrRAKSerial::password(const char *new_passwd, size_t len) {
	ARG_UNUSED(new_passwd);
	ARG_UNUSED(len);

	return false;
}

bool rak::ZephyrRAKSerial::password(const ::arduino::String &new_passwd) {
	return password(new_passwd.c_str(), new_passwd.length());
}

uint32_t rak::ZephyrRAKSerial::getBaudrate() {
	const struct device *uart = getUartDevice();
	struct uart_config config;
	uint32_t baudrate;

	if (uart == nullptr) {
		return 0;
	}

	if (uart_config_get(uart, &config) == 0) {
		return config.baudrate;
	}

	if (uart_line_ctrl_get(uart, UART_LINE_CTRL_BAUD_RATE, &baudrate) == 0) {
		return baudrate;
	}

	return 0;
}
