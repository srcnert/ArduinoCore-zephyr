/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/include/PrintableMock.h
 */

#pragma once

#include <Arduino.h>

class PrintableMock : public arduino::Printable {
public:
	int _i;

	size_t printTo(arduino::Print &p) const override {
		size_t written = 0;
		written += p.print("PrintableMock i = ");
		written += p.print(_i);
		return written;
	}
};
