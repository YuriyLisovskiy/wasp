/**
 * http/tests_cookie.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/http/cookie.h"

using namespace xw;


TEST(CookieTestCase, toStringTestAllParameters)
{
	http::Cookie cookie(
		"hello", "World", 0, "Thu, 18 Jul 2019 16:25:19 GMT", "localhost.com", "/hello", true, true
	);
	std::string expected = "Set-Cookie: hello=World; Domain=localhost.com; Path=/hello; Max-Age=0; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Secure; HttpOnly";
	auto actual = cookie.to_string();
	ASSERT_EQ(actual, expected);
}

TEST(CookieTestCase, toStringTestHttpOnlyIsFalse)
{
	http::Cookie cookie("hello", "1", 0, "Thu, 18 Jul 2019 16:25:19 GMT", "localhost.com", "/hello", true, false);
	std::string expected = "Set-Cookie: hello=1; Domain=localhost.com; Path=/hello; Max-Age=0; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Secure";
	auto actual = cookie.to_string();
	ASSERT_EQ(actual, expected);
}

TEST(CookieTestCase, toStringTestOnlyRequiredParameters)
{
	http::Cookie cookie("hello", "2.2", 0, "Thu, 18 Jul 2019 16:25:19 GMT");
	std::string expected = "Set-Cookie: hello=2.2; Path=/; Max-Age=0; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Secure";
	auto actual = cookie.to_string();
	ASSERT_EQ(actual, expected);
}

TEST(CookieTestCase, toStringTestEmptyPath)
{
	http::Cookie cookie("hello", "2.2", 0, "Thu, 18 Jul 2019 16:25:19 GMT", "", "");
	std::string expected = "Set-Cookie: hello=2.2; Max-Age=0; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Secure";
	auto actual = cookie.to_string();
	ASSERT_EQ(actual, expected);
}
