/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Ringbuffer/test_available.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/RingBuffer.h>

TEST_GROUP(Ringbuffer_available) {
	arduino::RingBufferN<2> ringbuffer;
};

TEST(Ringbuffer_available, available_01) {
	LONGS_EQUAL(0, ringbuffer.available());
}

TEST(Ringbuffer_available, available_02) {
	ringbuffer.store_char('A');
	LONGS_EQUAL(1, ringbuffer.available());
	ringbuffer.store_char('B');
	LONGS_EQUAL(2, ringbuffer.available());
}
