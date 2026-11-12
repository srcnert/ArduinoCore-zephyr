/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Ringbuffer/test_store_char.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/RingBuffer.h>

TEST_GROUP(Ringbuffer_store_char) {
	arduino::RingBufferN<2> ringbuffer;
};

TEST(Ringbuffer_store_char, store_char_01) {
	ringbuffer.store_char('A');
	LONGS_EQUAL('A', ringbuffer._aucBuffer[0]);
	ringbuffer.store_char('B');
	LONGS_EQUAL('B', ringbuffer._aucBuffer[1]);
}
