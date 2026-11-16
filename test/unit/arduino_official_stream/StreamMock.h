/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/include/StreamMock.h and
 * test/src/StreamMock.cpp, with a fixed buffer instead of std::deque
 */

#pragma once

#include <Arduino.h>
#include <string.h>

// Tests that read past the end of the data set a short timeout: upstream fakes millis() so that
// the 1 s default timeout passes at once.
class StreamMock : public arduino::Stream {
public:
	void operator<<(char const *str) {
		for (size_t c = 0; c < strlen(str); c++) {
			push(str[c]);
		}
	}

	size_t write(uint8_t ch) override {
		push(static_cast<char>(ch));
		return 1;
	}

	int available() override {
		return static_cast<int>(_len - _pos);
	}

	int read() override {
		if (available() == 0) {
			return -1;
		}
		return _stream[_pos++];
	}

	int peek() override {
		if (available() == 0) {
			return -1;
		}
		return _stream[_pos];
	}

private:
	void push(char c) {
		if (_len < sizeof(_stream)) {
			_stream[_len++] = c;
		}
	}

	char _stream[512];
	size_t _len = 0;
	size_t _pos = 0;
};
