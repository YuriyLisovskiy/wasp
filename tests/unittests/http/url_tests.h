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

#ifndef WASP_UNIT_TESTS_HTTP_URL_TESTS_H
#define WASP_UNIT_TESTS_HTTP_URL_TESTS_H

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/http/url.h"


__UNIT_TESTS_BEGIN__

class SimpleUrlTestCase : public ::testing::Test
{
protected:
	wasp::Url* url{};

	const char* SCHEME_EXPECTED = "http";
	const char* HOST_EXPECTED = "127.0.0.1";
	const uint16_t PORT_EXPECTED = 5000;
	const char* PATH_EXPECTED = "/hello";
	const char* FRAGMENT_EXPECTED = "world";
	const char* STR_URL_EXPECTED = "http://127.0.0.1:5000/hello#world";

	void SetUp() override
	{
		this->url = new Url(STR_URL_EXPECTED);
	}

	void TearDown() override
	{
		delete this->url;
	}
};

TEST_F(SimpleUrlTestCase, StrUrlTest)
{
	ASSERT_EQ(this->url->str(), STR_URL_EXPECTED);
}

TEST_F(SimpleUrlTestCase, SchemeTest)
{
	ASSERT_EQ(this->url->scheme(), SCHEME_EXPECTED);
}

TEST_F(SimpleUrlTestCase, HostTest)
{
	ASSERT_EQ(this->url->hostname(), HOST_EXPECTED);
}

TEST_F(SimpleUrlTestCase, PortTest)
{
	ASSERT_EQ(this->url->port(), PORT_EXPECTED);
}

TEST_F(SimpleUrlTestCase, PathTest)
{
	ASSERT_EQ(this->url->path(), PATH_EXPECTED);
}

TEST_F(SimpleUrlTestCase, FragmentTest)
{
	ASSERT_EQ(this->url->fragment(), FRAGMENT_EXPECTED);
}


class ComplexUrlTestCase : public ::testing::Test
{
protected:
	wasp::Url* url{};

	const char* STR_URL_EXPECTED = "http://username:password@example.com/hello/world?hello=world&user=pass#someFragment";
	const char* HOST_EXPECTED = "example.com";
	const uint16_t PORT_EXPECTED = 80;
	const char* SCHEME_EXPECTED = "http";
	const char* PATH_EXPECTED = "/hello/world";
	const char* USERNAME_EXPECTED = "username";
	const char* PASSWORD_EXPECTED = "password";
	const char* QUERY_EXPECTED = "hello=world&user=pass";
	const char* FRAGMENT_EXPECTED = "someFragment";

	void SetUp() override
	{
		this->url = new Url(STR_URL_EXPECTED);
	}

	void TearDown() override
	{
		delete this->url;
	}
};

TEST_F(ComplexUrlTestCase, StrUrlTest)
{
	ASSERT_EQ(this->url->str(), STR_URL_EXPECTED);
}

TEST_F(ComplexUrlTestCase, SchemeTest)
{
	ASSERT_EQ(this->url->scheme(), SCHEME_EXPECTED);
}

TEST_F(ComplexUrlTestCase, HostTest)
{
	ASSERT_EQ(this->url->hostname(), HOST_EXPECTED);
}

TEST_F(ComplexUrlTestCase, PortTest)
{
	ASSERT_EQ(this->url->port(), PORT_EXPECTED);
}

TEST_F(ComplexUrlTestCase, PathTest)
{
	ASSERT_EQ(this->url->path(), PATH_EXPECTED);
}

TEST_F(ComplexUrlTestCase, UsernameTest)
{
	ASSERT_EQ(this->url->username(), USERNAME_EXPECTED);
}

TEST_F(ComplexUrlTestCase, PasswordTest)
{
	ASSERT_EQ(this->url->password(), PASSWORD_EXPECTED);
}

TEST_F(ComplexUrlTestCase, FramentTest)
{
	ASSERT_EQ(this->url->fragment(), FRAGMENT_EXPECTED);
}

TEST_F(ComplexUrlTestCase, QueryTest)
{
	ASSERT_EQ(this->url->query(), QUERY_EXPECTED);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_HTTP_URL_TESTS_H
