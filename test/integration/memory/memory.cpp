/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

// The heap and the stack a sketch gets. The minimums are a first guess, each
// test prints what the board really has. No wiring.

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <stdarg.h>
#include <stdio.h>

constexpr size_t MIN_BLOCK = 8 * 1024;
constexpr size_t MIN_STRING = 4 * 1024;
constexpr size_t MIN_STACK = 2 * 1024;
constexpr size_t PIECE = 1024;
constexpr size_t SRAM_SIZE = DT_REG_SIZE(DT_CHOSEN(zephyr_sram));

static void report(const char *format, ...) {
	char text[96];
	va_list args;

	va_start(args, format);
	vsnprintf(text, sizeof(text), format, args);
	va_end(args);
	UT_PRINT(text);
}

// Binary search, malloc() returns NULL for what doesn't fit
static size_t largest_block() {
	size_t low = 0;
	size_t high = SRAM_SIZE;

	while (low < high) {
		size_t mid = low + (high - low + 1) / 2;
		void *block = malloc(mid);

		if (block) {
			free(block);
			low = mid;
		} else {
			high = mid - 1;
		}
	}
	return low;
}

// PIECE-sized blocks until malloc() fails, then frees them all
static size_t total_in_pieces() {
	static void *pieces[SRAM_SIZE / PIECE];
	size_t count = 0;

	while (count < ARRAY_SIZE(pieces) && (pieces[count] = malloc(PIECE)) != nullptr) {
		count++;
	}
	for (size_t i = 0; i < count; i++) {
		free(pieces[i]);
	}
	return count * PIECE;
}

// The sketch thread grows its stack down to stack_info.start
static size_t free_stack() {
	volatile uint8_t marker = 0;

	return (uintptr_t)&marker - k_current_get()->stack_info.start;
}

static __attribute__((noinline)) uint32_t use_stack() {
	volatile uint8_t buf[MIN_STACK];
	uint32_t sum = 0;

	for (size_t i = 0; i < sizeof(buf); i++) {
		buf[i] = i;
	}
	for (size_t i = 0; i < sizeof(buf); i++) {
		sum += buf[i];
	}
	return sum;
}

TEST_GROUP(Memory){};

TEST(Memory, largestBlock) {
	size_t largest = largest_block();

	report("largest block %zu bytes, %zu bytes in 1 KB pieces", largest, total_in_pieces());
	CHECK_COMPARE(largest, >=, MIN_BLOCK);
}

// Freed memory comes back: the same largest block after many allocations
TEST(Memory, freeReuses) {
	size_t before = largest_block();

	for (size_t round = 0; round < 100; round++) {
		void *small = malloc(16 + round);
		void *big = malloc(before / 2);

		CHECK(small != nullptr);
		CHECK(big != nullptr);
		free(big);
		free(small);
	}
	LONGS_EQUAL(before, largest_block());
}

TEST(Memory, newDelete) {
	size_t before = largest_block();

	for (int round = 0; round < 10; round++) {
		uint8_t *block = new uint8_t[before / 2];

		CHECK(block != nullptr);
		block[0] = round;
		block[before / 2 - 1] = round;
		delete[] block;
	}
	LONGS_EQUAL(before, largest_block());
}

// Grown one character at a time, the way sketches build text
TEST(Memory, string) {
	String text;
	size_t wrong = 0;

	for (size_t i = 0; i < MIN_STRING; i++) {
		CHECK(text.concat((char)('a' + i % 26)));
	}
	LONGS_EQUAL(MIN_STRING, text.length());
	for (size_t i = 0; i < MIN_STRING; i++) {
		if (text.charAt(i) != (char)('a' + i % 26)) {
			wrong++;
		}
	}
	LONGS_EQUAL(0, wrong);
}

TEST(Memory, stack) {
	size_t left = free_stack();
	uint32_t expected = 0;

	report("stack %zu bytes, %zu left in a test", k_current_get()->stack_info.size, left);
	// use_stack() needs its buffer and a frame
	CHECK_COMPARE(left, >=, MIN_STACK + 256);
	for (size_t i = 0; i < MIN_STACK; i++) {
		expected += (uint8_t)i;
	}
	LONGS_EQUAL(expected, use_stack());
}
