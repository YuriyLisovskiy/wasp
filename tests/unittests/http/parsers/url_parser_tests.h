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

#ifndef WASP_UNIT_TESTS_HTTP_URL_PARSER_TESTS_H
#define WASP_UNIT_TESTS_HTTP_URL_PARSER_TESTS_H

#include <gtest/gtest.h>

#include "../../globals.h"
#include "../../../../src/http/parsers/url_parser.h"


__UNIT_TESTS_BEGIN__

class UrlParserTestCase : public ::testing::Test
{
protected:
	wasp::internal::url_parser parser;

	const char* STR_URL = "http://username:password@example.com/hello/world?hello=world&user=pass#someFragment";
	const char* HOST_EXPECTED = "example.com";
	const char* PORT_EXPECTED = "";
	const char* SCHEME_EXPECTED = "http";
	const char* PATH_EXPECTED = "/hello/world";
	const char* USERNAME_EXPECTED = "username";
	const char* PASSWORD_EXPECTED = "password";
	const char* QUERY_EXPECTED = "hello=world&user=pass";
	const char* FRAGMENT_EXPECTED = "someFragment";

	void SetUp() override
	{
		this->parser.parse(STR_URL);
	}
};

TEST_F(UrlParserTestCase, SchemeTest)
{
	ASSERT_EQ(this->parser.scheme, SCHEME_EXPECTED);
}

TEST_F(UrlParserTestCase, HostTest)
{
	ASSERT_EQ(this->parser.hostname, HOST_EXPECTED);
}

TEST_F(UrlParserTestCase, PortTest)
{
	ASSERT_EQ(this->parser.port, PORT_EXPECTED);
}

TEST_F(UrlParserTestCase, PathTest)
{
	ASSERT_EQ(this->parser.path, PATH_EXPECTED);
}

TEST_F(UrlParserTestCase, UsernameTest)
{
	ASSERT_EQ(this->parser.username, USERNAME_EXPECTED);
}

TEST_F(UrlParserTestCase, PasswordTest)
{
	ASSERT_EQ(this->parser.password, PASSWORD_EXPECTED);
}

TEST_F(UrlParserTestCase, FramentTest)
{
	ASSERT_EQ(this->parser.fragment, FRAGMENT_EXPECTED);
}

TEST_F(UrlParserTestCase, QueryTest)
{
	ASSERT_EQ(this->parser.query, QUERY_EXPECTED);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_HTTP_URL_PARSER_TESTS_H
