/**
 * http/cookie/tests_parser.cpp
 *
 * Copyright (c) 2019, 2021 Yuriy Lisovskiy
 */

#include <map>
#include <string>

#include <gtest/gtest.h>

#include "../../../src/http/cookie/parser.h"

using namespace xw;


class RequestCookieParserTestCase : public ::testing::Test
{
protected:
	const char* COOKIES = "enwiki_session=17ab96bd8ffbe8ca58a78657a918558; hello=world";
	const char* FIRST_COOKIE_EXPECTED_KEY = "enwiki_session";
	const char* FIRST_COOKIE_EXPECTED_VALUE = "17ab96bd8ffbe8ca58a78657a918558";
	const char* SECOND_COOKIE_EXPECTED_KEY = "hello";
	const char* SECOND_COOKIE_EXPECTED_VALUE = "world";

	std::vector<http::Cookie> parsed_vector;

	void SetUp() override
	{
		this->parsed_vector = http::parse_cookies(COOKIES, "");
	}
};

TEST_F(RequestCookieParserTestCase, FirstCookieTest)
{
	for (const http::Cookie& v : this->parsed_vector)
	{
		if (v.name() == std::string(FIRST_COOKIE_EXPECTED_KEY))
		{
			ASSERT_EQ(v.value(), std::string(FIRST_COOKIE_EXPECTED_VALUE));
			return;
		}
	}

	ASSERT_TRUE(false);
}

TEST_F(RequestCookieParserTestCase, SecondCookieTest)
{
	for (const http::Cookie& v : this->parsed_vector)
	{
		if (v.name() == std::string(SECOND_COOKIE_EXPECTED_KEY))
		{
			ASSERT_EQ(v.value(), std::string(SECOND_COOKIE_EXPECTED_VALUE));
			return;
		}
	}

	ASSERT_TRUE(false);
}
