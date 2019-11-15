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

#ifndef WASP_HTTP_PARSERS_REQUEST_PARSER_TESTS_H
#define WASP_HTTP_PARSERS_REQUEST_PARSER_TESTS_H

#include <map>
#include <algorithm>

#include <gtest/gtest.h>

#include "../../../../src/globals.h"
#include "../../../../src/http/parsers/request_parser.h"


__UNIT_TESTS_BEGIN__

TEST(RequestParserTestCase, IsCharTest)
{
	for (size_t i = 0; i < 128; i++)
	{
		ASSERT_TRUE(wasp::internal::request_parser::is_char(i));
	}
}

TEST(RequestParserTestCase, IsNotCharTest)
{
	ASSERT_FALSE(wasp::internal::request_parser::is_char(-1));
	ASSERT_FALSE(wasp::internal::request_parser::is_char(128));
}

TEST(RequestParserTestCase, IsControlTest)
{
	for (size_t i = 0; i < 32; i++)
	{
		ASSERT_TRUE(wasp::internal::request_parser::is_control(i));
	}

	ASSERT_TRUE(wasp::internal::request_parser::is_control(127));
}

TEST(RequestParserTestCase, IsNotControlTest)
{
	ASSERT_FALSE(wasp::internal::request_parser::is_control(-1));

	for (size_t i = 32; i < 127; i++)
	{
		ASSERT_FALSE(wasp::internal::request_parser::is_control(i));
	}

	ASSERT_FALSE(wasp::internal::request_parser::is_control(128));
}

TEST(RequestParserTestCase, IsSpecialTest)
{
	char specials[] = {
			'(', ')', '<', '>', '@', ',', ';', ':', '"', '/', '\\', '[', ']', '?', '=', '{', '}', '\t'
	};
	for (auto special : specials)
	{
		ASSERT_TRUE(wasp::internal::request_parser::is_special(special));
	}
}

TEST(RequestParserTestCase, IsNotSpecialTest)
{
	ASSERT_FALSE(wasp::internal::request_parser::is_special('f'));
	ASSERT_FALSE(wasp::internal::request_parser::is_special('0'));
	ASSERT_FALSE(wasp::internal::request_parser::is_special('\n'));
}

TEST(RequestParserTestCase, IsDigitTest)
{
	for (char i = '0'; i <= '9'; i++)
	{
		ASSERT_TRUE(wasp::internal::request_parser::is_digit(i));
	}
}

TEST(RequestParserTestCase, IsNotDigitTest)
{
	ASSERT_FALSE(wasp::internal::request_parser::is_digit('h'));
	ASSERT_FALSE(wasp::internal::request_parser::is_digit('e'));
	ASSERT_FALSE(wasp::internal::request_parser::is_digit('l'));
	ASSERT_FALSE(wasp::internal::request_parser::is_digit('l'));
	ASSERT_FALSE(wasp::internal::request_parser::is_digit('o'));
}

__UNIT_TESTS_END__


#endif // WASP_HTTP_PARSERS_REQUEST_PARSER_TESTS_H
