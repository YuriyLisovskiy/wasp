/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WASP_UNIT_TESTS_CORE_TESTS_PARSERS_TESTS_COOKIE_PARSER_TESTS_H
#define WASP_UNIT_TESTS_CORE_TESTS_PARSERS_TESTS_COOKIE_PARSER_TESTS_H

#include <gtest/gtest.h>

#include <map>
#include <string>

#include "../../_def_.h"
#include "../../../../src/core/parsers/cookie_parser.h"


__UNIT_TESTS_BEGIN__

class RequestCookieParserTestCase : public ::testing::Test
{
protected:
	const char* COOKIES = "enwiki_session=17ab96bd8ffbe8ca58a78657a918558; hello=world";
	const char* FIRST_COOKIE_EXPECTED_KEY = "enwiki_session";
	const char* FIRST_COOKIE_EXPECTED_VALUE = "17ab96bd8ffbe8ca58a78657a918558";
	const char* SECOND_COOKIE_EXPECTED_KEY = "hello";
	const char* SECOND_COOKIE_EXPECTED_VALUE = "world";

	std::map<std::string, std::string>* parsedMap{};

	void SetUp() override
	{
		this->parsedMap = core::internal::cookie_parser::parse_req_cookies(COOKIES);
	}

	void TearDown() override
	{
		delete this->parsedMap;
	}
};

TEST_F(RequestCookieParserTestCase, FirstCookieTest)
{
	ASSERT_TRUE(this->parsedMap->find(FIRST_COOKIE_EXPECTED_KEY) != this->parsedMap->end());
	ASSERT_EQ(this->parsedMap->at(FIRST_COOKIE_EXPECTED_KEY), FIRST_COOKIE_EXPECTED_VALUE);
}

TEST_F(RequestCookieParserTestCase, SecondCookieTest)
{
	ASSERT_TRUE(this->parsedMap->find(SECOND_COOKIE_EXPECTED_KEY) != this->parsedMap->end());
	ASSERT_EQ(this->parsedMap->at(SECOND_COOKIE_EXPECTED_KEY), SECOND_COOKIE_EXPECTED_VALUE);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_CORE_TESTS_PARSERS_TESTS_COOKIE_PARSER_TESTS_H
