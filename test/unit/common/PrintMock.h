/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/include/PrintMock.h and
 * test/src/PrintMock.cpp, with a fixed buffer instead of std::string
 */

#pragma once

#include <Arduino.h>

class PrintMock : public Print {
public:
	char _str[128] = {};

	size_t write(uint8_t b) override {
		if (_len < sizeof(_str) - 1) {
			_str[_len++] = static_cast<char>(b);
			_str[_len] = '\0';
		}
		return 1;
	}

	void mock_setWriteError() {
		setWriteError();
	}

	void mock_setWriteError(int err) {
		setWriteError(err);
	}

private:
	size_t _len = 0;
};
