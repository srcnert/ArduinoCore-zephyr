/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <string.h>

// Collects what Print writes, to compare it with the expected text
class BufferPrint : public Print {
public:
	using Print::write;

	size_t write(uint8_t ch) override {
		if (len < sizeof(buf) - 1) {
			buf[len++] = (char)ch;
			buf[len] = '\0';
		}
		return 1;
	}

	const char *str() {
		return buf;
	}

private:
	char buf[64] = {};
	size_t len = 0;
};

// Reads a fixed text, to test the Stream parsing functions
class TextStream : public Stream {
public:
	explicit TextStream(const char *text) : text(text) {
	}

	int available() override {
		return (int)strlen(text + pos);
	}

	int read() override {
		return text[pos] ? (unsigned char)text[pos++] : -1;
	}

	int peek() override {
		return text[pos] ? (unsigned char)text[pos] : -1;
	}

	size_t write(uint8_t) override {
		return 0;
	}

private:
	const char *text;
	size_t pos = 0;
};

TEST_GROUP(Print) {
	BufferPrint out;
};

TEST(Print, integer) {
	out.print(42);
	out.print(' ');
	out.print(-12);
	STRCMP_EQUAL("42 -12", out.str());
}

TEST(Print, integer_base) {
	out.print(255, HEX);
	out.print(' ');
	out.print(5, BIN);
	out.print(' ');
	out.print(8, OCT);
	STRCMP_EQUAL("FF 101 10", out.str());
}

TEST(Print, floating_point) {
	out.print(3.14159, 3);
	out.print(' ');
	out.print(-0.5);
	STRCMP_EQUAL("3.142 -0.50", out.str());
}

TEST(Print, string_and_println) {
	out.print(String("S"));
	out.print('c');
	out.println(1);
	STRCMP_EQUAL("Sc1\r\n", out.str());
}

TEST(Print, integer_64bit) {
	out.print(-9000000000LL);
	out.print(' ');
	out.print(18446744073709551615ULL);
	out.print(' ');
	out.print(0x123456789ABCDEF0ULL, HEX);
	out.print(' ');
	out.print(0ULL);
	STRCMP_EQUAL("-9000000000 18446744073709551615 123456789ABCDEF0 0", out.str());
}

TEST(Print, println_overloads) {
	out.println('a');
	out.println((unsigned char)200, HEX);
	out.println(7u);
	out.println(-8L);
	out.println(9UL);
	out.println(String("s"));
	out.println(-1LL);
	out.println(2ULL);
	out.println(0.5, 1);
	STRCMP_EQUAL("a\r\nC8\r\n7\r\n-8\r\n9\r\ns\r\n-1\r\n2\r\n0.5\r\n", out.str());
}

TEST(Print, flash_string) {
	out.print(F("abc"));
	out.println(F("def"));
	STRCMP_EQUAL("abcdef\r\n", out.str());
}

TEST(Print, printable) {
	out.print(IPAddress(10, 0, 0, 1));
	out.print(' ');
	out.print(IPAddress("2001:db8::1"));
	out.print(' ');
	out.print(IPAddress("1:0:0:2:0:0:0:3"));
	out.print(' ');
	out.println(IPAddress("::1"));
	STRCMP_EQUAL("10.0.0.1 2001:db8::1 1:0:0:2::3 ::1\r\n", out.str());
}

TEST_GROUP(Stream){};

TEST(Stream, parse) {
	TextStream in("x=42, y=-3.75 tail\nnext");

	in.setTimeout(10);
	LONGS_EQUAL(42, in.parseInt());
	DOUBLES_EQUAL(-3.75, in.parseFloat(), 1e-6);
	CHECK(in.find("tail"));
	in.read();
	STRCMP_EQUAL("next", in.readString().c_str());
}

TEST(Stream, find_and_read_bytes) {
	TextStream in("abcdef;12,34\nrest");
	char buf[8];

	in.setTimeout(10);
	CHECK(in.find("abX", 2));
	LONGS_EQUAL(2, in.readBytes(buf, 2));
	MEMCMP_EQUAL("cd", buf, 2);
	LONGS_EQUAL(2, in.readBytesUntil(';', buf, sizeof(buf)));
	MEMCMP_EQUAL("ef", buf, 2);
	STRCMP_EQUAL("12", in.readStringUntil(',').c_str());
	CHECK_FALSE(in.findUntil("rest", "\n"));
	CHECK(in.find('r'));
	STRCMP_EQUAL("est", in.readString().c_str());
}
