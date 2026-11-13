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

TEST(String, construct) {
	STRCMP_EQUAL("He", String("Hello", 2).c_str());
	STRCMP_EQUAL("Q", String('Q').c_str());
	STRCMP_EQUAL("c8", String((unsigned char)200, HEX).c_str());
	STRCMP_EQUAL("ffff", String(65535u, HEX).c_str());
	STRCMP_EQUAL("-100000", String(-100000L).c_str());
	STRCMP_EQUAL("flash", String(F("flash")).c_str());
}

TEST(String, move_and_invalidate) {
	String a("moved");
	String b(static_cast<String &&>(a));

	STRCMP_EQUAL("moved", b.c_str());
	CHECK(!a);
	a = static_cast<String &&>(b);
	STRCMP_EQUAL("moved", a.c_str());
	CHECK(!b);
	a = (const char *)NULL;
	CHECK(!a);
	LONGS_EQUAL(0, a.length());
	a = F("back");
	STRCMP_EQUAL("back", a.c_str());
}

TEST(String, concat_and_sum) {
	String s;

	CHECK(s.concat(String("a")));
	CHECK(s.concat((unsigned char)1));
	CHECK(s.concat(2u));
	CHECK(s.concat(-3L));
	CHECK(s.concat(4UL));
	CHECK(s.concat(0.5));
	CHECK(s.concat(F("f")));
	STRCMP_EQUAL("a12-340.50f", s.c_str());

	String sum = String("v") + String("w") + "x" + 'y' + (unsigned char)1 + 2;
	sum = sum + 3u + 4L + 5UL + 0.25 + F("z");
	STRCMP_EQUAL("vwxy123450.25z", sum.c_str());
}

TEST(String, compare_and_access) {
	String s("cat");
	const String &cs = s;

	CHECK(s == String("cat"));
	CHECK(s == "cat");
	CHECK(s != "dog");
	CHECK(String("") == "");
	CHECK(String("b").compareTo(String("a")) > 0);
	LONGS_EQUAL(0, s.compareTo(String("cat")));
	CHECK(String("a") < String("b"));
	s[0] = 'b';
	STRCMP_EQUAL("bat", s.c_str());
	s[10] = 'x';
	STRCMP_EQUAL("bat", s.c_str());
	LONGS_EQUAL(0, cs[10]);
}

TEST(String, search_and_bytes) {
	String s("abcabc");
	unsigned char buf[4];

	LONGS_EQUAL(1, s.indexOf('b'));
	LONGS_EQUAL(4, s.indexOf('b', 2));
	LONGS_EQUAL(-1, s.indexOf('z'));
	LONGS_EQUAL(4, s.lastIndexOf("bc"));
	LONGS_EQUAL(1, s.lastIndexOf("bc", 3));
	s.getBytes(buf, sizeof(buf), 1);
	STRCMP_EQUAL("bca", (const char *)buf);
	s.getBytes(buf, sizeof(buf), 10);
	STRCMP_EQUAL("", (const char *)buf);
	STRCMP_EQUAL("bca", s.substring(4, 1).c_str());
}

TEST(String, replace_and_case) {
	String s("a-b-c");
	String m("MiXeD");

	s.replace('-', '+');
	STRCMP_EQUAL("a+b+c", s.c_str());
	s.replace("+", "--");
	STRCMP_EQUAL("a--b--c", s.c_str());
	s.replace("--", "_");
	STRCMP_EQUAL("a_b_c", s.c_str());
	s.remove(1, 3);
	STRCMP_EQUAL("ac", s.c_str());
	m.toLowerCase();
	STRCMP_EQUAL("mixed", m.c_str());
}
