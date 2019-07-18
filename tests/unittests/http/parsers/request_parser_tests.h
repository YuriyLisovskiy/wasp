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


__INTERNAL_BEGIN__

class HttpRequestParserTest : public ::testing::Test
{
protected:
	static void isCharTest()
	{
		for (size_t i = 0; i < 128; i++)
		{
			ASSERT_TRUE(HttpRequestParser::isChar(i));
		}
	}

	static void isNotCharTest()
	{
		ASSERT_FALSE(HttpRequestParser::isChar(-1));
		ASSERT_FALSE(HttpRequestParser::isChar(128));
	}

	static void isControlTest()
	{
		for (size_t i = 0; i < 32; i++)
		{
			ASSERT_TRUE(HttpRequestParser::isControl(i));
		}
		ASSERT_TRUE(HttpRequestParser::isControl(127));
	}

	static void isNotControlTest()
	{
		ASSERT_FALSE(HttpRequestParser::isControl(-1));
		for (size_t i = 32; i < 127; i++)
		{
			ASSERT_FALSE(HttpRequestParser::isControl(i));
		}
		ASSERT_FALSE(HttpRequestParser::isControl(128));
	}

	static void isSpecialTest()
	{
		char specials[] = {
			'(', ')', '<', '>', '@', ',', ';', ':', '"', '/', '\\', '[', ']', '?', '=', '{', '}', '\t'
		};
		for (auto special : specials)
		{
			ASSERT_TRUE(HttpRequestParser::isSpecial(special));
		}
	}

	static void isNotSpecialTest()
	{
		ASSERT_FALSE(HttpRequestParser::isSpecial('f'));
		ASSERT_FALSE(HttpRequestParser::isSpecial('0'));
		ASSERT_FALSE(HttpRequestParser::isSpecial('\n'));
	}

	static void isDigitTest()
	{
		for (char i = '0'; i <= '9'; i++)
		{
			ASSERT_TRUE(HttpRequestParser::isDigit(i));
		}
	}

	static void isNotDigitTest()
	{
		ASSERT_FALSE(HttpRequestParser::isDigit('h'));
		ASSERT_FALSE(HttpRequestParser::isDigit('e'));
		ASSERT_FALSE(HttpRequestParser::isDigit('l'));
		ASSERT_FALSE(HttpRequestParser::isDigit('l'));
		ASSERT_FALSE(HttpRequestParser::isDigit('o'));
	}

	void parseQueryTest()
	{
		auto expected = std::map<std::string, std::string> {
			std::pair<std::string, std::string>("hello", "world"),
			std::pair<std::string, std::string>("world", "hello"),
			std::pair<std::string, std::string>("hello_key", "hello_value"),
			std::pair<std::string, std::string>("world_key", "world_value"),
		};
		auto actual = HttpRequestParser::_parseQuery("hello=world&world=hello&hello_key=hello_value&world_key=world_value");
		ASSERT_TRUE(this->mapsAreEqual(expected, *actual));
		delete actual;
	}
private:
	HttpRequestParser parser;

	template <typename _Map>
	bool mapsAreEqual(const _Map& lhs, const _Map& rhs)
	{
		return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}
};

TEST_F(HttpRequestParserTest, IsCharTest)
{
	HttpRequestParserTest::isCharTest();
}

TEST_F(HttpRequestParserTest, IsNotCharTest)
{
	HttpRequestParserTest::isNotCharTest();
}

TEST_F(HttpRequestParserTest, IsControlTest)
{
	HttpRequestParserTest::isControlTest();
}

TEST_F(HttpRequestParserTest, IsNotControlTest)
{
	HttpRequestParserTest::isNotControlTest();
}

TEST_F(HttpRequestParserTest, IsSpecialTest)
{
	HttpRequestParserTest::isSpecialTest();
}

TEST_F(HttpRequestParserTest, IsNotSpecialTest)
{
	HttpRequestParserTest::isNotSpecialTest();
}

TEST_F(HttpRequestParserTest, IsDigitTest)
{
	HttpRequestParserTest::isDigitTest();
}

TEST_F(HttpRequestParserTest, IsNotDigitTest)
{
	HttpRequestParserTest::isNotDigitTest();
}

TEST_F(HttpRequestParserTest, ParseQueryTest)
{
	this->parseQueryTest();
}

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_REQUEST_PARSER_TESTS_H
