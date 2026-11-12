/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String){};

TEST(String, append_and_search) {
	String s = "Hello";

	s += ' ';
	s += "World";
	s += 42;
	STRCMP_EQUAL("Hello World42", s.c_str());
	LONGS_EQUAL(13, s.length());
	LONGS_EQUAL(6, s.indexOf("World"));
	LONGS_EQUAL(7, s.lastIndexOf('o'));
	STRCMP_EQUAL("World", s.substring(6, 11).c_str());
	LONGS_EQUAL('e', s.charAt(1));
}

TEST(String, modify) {
	String s = "  Trim Me  ";

	s.trim();
	STRCMP_EQUAL("Trim Me", s.c_str());
	s.toUpperCase();
	STRCMP_EQUAL("TRIM ME", s.c_str());
	s.replace(" ", "_");
	STRCMP_EQUAL("TRIM_ME", s.c_str());
	s.remove(4);
	s.setCharAt(0, 'G');
	STRCMP_EQUAL("GRIM", s.c_str());
}

TEST(String, compare) {
	CHECK(String("arduino").startsWith("ard"));
	CHECK(String("arduino").endsWith("ino"));
	CHECK(String("RAK").equalsIgnoreCase("rak"));
	CHECK(String("a").compareTo("b") < 0);
}

TEST(String, to_number) {
	LONGS_EQUAL(-1234, String("-1234").toInt());
	DOUBLES_EQUAL(3.5, String("3.5").toFloat(), 1e-6);
	DOUBLES_EQUAL(-2.25, String("-2.25").toDouble(), 1e-12);
}

TEST(String, from_number) {
	STRCMP_EQUAL("ff", String(255, HEX).c_str());
	STRCMP_EQUAL("101", String(5, BIN).c_str());
	STRCMP_EQUAL("4000000000", String(4000000000UL).c_str());
	STRCMP_EQUAL("1.25", String(1.25f).c_str());
	STRCMP_EQUAL("-3.142", String(-3.14159, 3).c_str());
	STRCMP_EQUAL("x=2.50", (String("x=") + 2.5f).c_str());
}
