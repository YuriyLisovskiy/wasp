/**
 * core/parsers/tests_url_parser.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include <xalwart.core/exceptions.h>

#include "../../../src/core/parsers/url_parser.h"

using namespace xw;


class UrlParserIPv4TestCase : public ::testing::Test
{
protected:
	parsers::url_parser parser;

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

		ASSERT_TRUE(this->parser.is_parsed);
	}
};

TEST_F(UrlParserIPv4TestCase, SchemeTest)
{
	ASSERT_EQ(this->parser.scheme, SCHEME_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, HostTest)
{
	ASSERT_EQ(this->parser.hostname, HOST_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, PortTest)
{
	ASSERT_EQ(this->parser.port, PORT_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, PathTest)
{
	ASSERT_EQ(this->parser.path, PATH_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, UsernameTest)
{
	ASSERT_EQ(this->parser.username, USERNAME_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, PasswordTest)
{
	ASSERT_EQ(this->parser.password, PASSWORD_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, FramentTest)
{
	ASSERT_EQ(this->parser.fragment, FRAGMENT_EXPECTED);
}

TEST_F(UrlParserIPv4TestCase, QueryTest)
{
	ASSERT_EQ(this->parser.query, QUERY_EXPECTED);
}


class UrlParserIPv6TestCase : public ::testing::Test
{
protected:
	parsers::url_parser parser;

	const char* STR_URL = "https://[2605:2700:0:3::4713:93e3]:8000/hello/world?hello=world&user=pass#someFragment";
	const char* HOST_EXPECTED = "2605:2700:0:3::4713:93e3";
	const char* PORT_EXPECTED = "8000";
	const char* SCHEME_EXPECTED = "https";
	const char* PATH_EXPECTED = "/hello/world";
	const char* USERNAME_EXPECTED = "";
	const char* PASSWORD_EXPECTED = "";
	const char* QUERY_EXPECTED = "hello=world&user=pass";
	const char* FRAGMENT_EXPECTED = "someFragment";

	void SetUp() override
	{
		this->parser.parse(STR_URL);

		ASSERT_TRUE(this->parser.is_parsed);
	}
};

TEST_F(UrlParserIPv6TestCase, SchemeTest)
{
	ASSERT_EQ(this->parser.scheme, SCHEME_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, HostTest)
{
	ASSERT_EQ(this->parser.hostname, HOST_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, PortTest)
{
	ASSERT_EQ(this->parser.port, PORT_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, PathTest)
{
	ASSERT_EQ(this->parser.path, PATH_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, UsernameTest)
{
	ASSERT_EQ(this->parser.username, USERNAME_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, PasswordTest)
{
	ASSERT_EQ(this->parser.password, PASSWORD_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, FramentTest)
{
	ASSERT_EQ(this->parser.fragment, FRAGMENT_EXPECTED);
}

TEST_F(UrlParserIPv6TestCase, QueryTest)
{
	ASSERT_EQ(this->parser.query, QUERY_EXPECTED);
}


class UrlParserErrorsTestCase : public ::testing::Test
{
protected:
	parsers::url_parser parser;
};

TEST_F(UrlParserErrorsTestCase, InvalidSchemeTest)
{
	this->parser.parse("ht~tp://example.com");
	ASSERT_FALSE(this->parser.is_parsed);
}

TEST_F(UrlParserErrorsTestCase, InvalidSchemeEndingTest)
{
	this->parser.parse("http//example.com");
	ASSERT_FALSE(this->parser.is_parsed);
}

TEST_F(UrlParserErrorsTestCase, AbsentFirstSlashTest)
{
	this->parser.parse("http:#example.com");
	ASSERT_FALSE(this->parser.is_parsed);
}

TEST_F(UrlParserErrorsTestCase, AbsentSecondSlashTest)
{
	this->parser.parse("http:/example.com");
	ASSERT_FALSE(this->parser.is_parsed);
}

TEST_F(UrlParserErrorsTestCase, UsernameOrHostnameErrorTest)
{
	this->parser.parse("http://$example.com");
	ASSERT_FALSE(this->parser.is_parsed);
}
