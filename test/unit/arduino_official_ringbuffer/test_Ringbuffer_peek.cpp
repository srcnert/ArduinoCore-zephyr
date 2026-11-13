/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Ringbuffer/test_peek.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/RingBuffer.h>

TEST_GROUP(Ringbuffer_peek) {
	arduino::RingBufferN<2> ringbuffer;
};

TEST(Ringbuffer_peek, peek_01_empty) {
	LONGS_EQUAL(-1, ringbuffer.peek());
}

TEST(Ringbuffer_peek, peek_01_fifo_not_removed) {
	ringbuffer.store_char('A');
	ringbuffer.store_char('B');
	LONGS_EQUAL('A', ringbuffer.peek());
	LONGS_EQUAL('A', ringbuffer.peek());
}
