/**
 * core/parsers/tests_cookie_parser.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <map>
#include <string>

#include <gtest/gtest.h>

#include "../../../src/core/parsers/cookie_parser.h"

using namespace xw;


class RequestCookieParserTestCase : public ::testing::Test
{
protected:
	const char* COOKIES = "enwiki_session=17ab96bd8ffbe8ca58a78657a918558; hello=world";
	const char* FIRST_COOKIE_EXPECTED_KEY = "enwiki_session";
	const char* FIRST_COOKIE_EXPECTED_VALUE = "17ab96bd8ffbe8ca58a78657a918558";
	const char* SECOND_COOKIE_EXPECTED_KEY = "hello";
	const char* SECOND_COOKIE_EXPECTED_VALUE = "world";

	std::map<std::string, std::string> parsedMap;

	void SetUp() override
	{
		auto parser = parsers::cookie_parser();
		parser.parse(COOKIES);
		this->parsedMap = parser.result;
	}
};

TEST_F(RequestCookieParserTestCase, FirstCookieTest)
{
	ASSERT_TRUE(this->parsedMap.find(FIRST_COOKIE_EXPECTED_KEY) != this->parsedMap.end());
	ASSERT_EQ(this->parsedMap.at(FIRST_COOKIE_EXPECTED_KEY), FIRST_COOKIE_EXPECTED_VALUE);
}

TEST_F(RequestCookieParserTestCase, SecondCookieTest)
{
	ASSERT_TRUE(this->parsedMap.find(SECOND_COOKIE_EXPECTED_KEY) != this->parsedMap.end());
	ASSERT_EQ(this->parsedMap.at(SECOND_COOKIE_EXPECTED_KEY), SECOND_COOKIE_EXPECTED_VALUE);
}
