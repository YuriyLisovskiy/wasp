/**
 * http/tests_url.cpp
 *
 * Copyright (c) 2019, 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include <xalwart.base/exceptions.h>

#include "../../src/http/url.h"

using namespace xw;

template <typename _Container>
bool containers_are_equal(const _Container& lhs, const _Container& rhs)
{
	return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

TEST(TestCase_url, query_parser_get)
{
	auto expected = std::map<std::string, std::string> {
		std::pair<std::string, std::string>("hello", "world"),
		std::pair<std::string, std::string>("world", "hello"),
		std::pair<std::string, std::string>("hello_key", "hello_value"),
		std::pair<std::string, std::string>("world_key", "world_value"),
	};
	auto actual = http::parse_query("hello=world&world=hello&hello_key=hello_value&world_key=world_value");
	for (const auto& item : expected)
	{
		ASSERT_TRUE(actual.contains(item.first));
		ASSERT_EQ(item.second, actual.get(item.first));
	}
}

TEST(TestCase_url, query_parser_getList)
{
	std::string key = "hello";
	auto expected = std::vector<std::string> {
		"world", "hello", "hello_value"
	};
	auto parameters = http::parse_query("hello=world&hello=hello&hello=hello_value");

	ASSERT_TRUE(parameters.contains(key));

	auto actual = parameters.getlist(key);

	ASSERT_EQ(actual.size(), expected.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST(TestCase_url, query_parser_keys)
{
	auto expected = std::vector<std::string> {
		"hello",
		"hello_key",
		"world",
		"world_key",
	};
	auto parsed = http::parse_query("hello=world&world=hello&hello_key=hello_value&world_key=world_value");
	auto actual = parsed.keys();

	ASSERT_TRUE(containers_are_equal(expected, actual));
}

TEST(TestCase_url, query_parser_size)
{
	auto expected = 4;
	auto parsed = http::parse_query("hello=world&world=hello&hello_key=hello_value&world_key=world_value");
	auto actual = parsed.size();

	ASSERT_EQ(expected, actual);
}

TEST(TestCase_url, query_parser_empty)
{
	auto parsed = http::parse_query("");
	ASSERT_TRUE(parsed.empty());
}

TEST(TestCase_url, valid_optional_port_IsEmpty)
{
	ASSERT_TRUE(http::internal::valid_optional_port(""));
}

TEST(TestCase_url, FirstIsNotColon)
{
	ASSERT_FALSE(http::internal::valid_optional_port("12345"));
}

TEST(TestCase_url, valid_optional_port_IsValid)
{
	ASSERT_TRUE(http::internal::valid_optional_port(":12345"));
}

TEST(TestCase_url, valid_optional_port_IsNotValid)
{
	ASSERT_FALSE(http::internal::valid_optional_port(":123g45"));
}

TEST(TestCase_url, split_CutSepTrue)
{
	auto actual = http::internal::split("hello, world", ',', true);
	ASSERT_EQ(actual.first, "hello");
	ASSERT_EQ(actual.second, " world");
}

TEST(TestCase_url, split_CutSepFalse)
{
	auto actual = http::internal::split("hello, world", ',', false);
	ASSERT_EQ(actual.first, "hello");
	ASSERT_EQ(actual.second, ", world");
}

TEST(TestCase_url, split_NotExistingSep)
{
	auto actual = http::internal::split("hello, world", '.', false);
	ASSERT_EQ(actual.first, "hello, world");
	ASSERT_EQ(actual.second, "");
}

class TestCase_url_UrlParserIPv4 : public ::testing::Test
{
protected:
	http::URL url;

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
		this->url = http::parse_url(STR_URL);
	}
};

TEST_F(TestCase_url_UrlParserIPv4, SchemeTest)
{
	ASSERT_EQ(this->url.scheme, SCHEME_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, HostTest)
{
	ASSERT_EQ(this->url.hostname(), HOST_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, PortTest)
{
	ASSERT_EQ(this->url.port(), PORT_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, PathTest)
{
	ASSERT_EQ(this->url.path, PATH_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, UsernameTest)
{
	ASSERT_EQ(this->url.user.username, USERNAME_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, PasswordTest)
{
	ASSERT_EQ(this->url.user.password, PASSWORD_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, FramentTest)
{
	ASSERT_EQ(this->url.fragment, FRAGMENT_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv4, QueryTest)
{
	ASSERT_EQ(this->url.raw_query, QUERY_EXPECTED);
}

class TestCase_url_UrlParserIPv6 : public ::testing::Test
{
protected:
	http::URL url;

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
		this->url = http::parse_url(STR_URL);
	}
};

TEST_F(TestCase_url_UrlParserIPv6, SchemeTest)
{
	ASSERT_EQ(this->url.scheme, SCHEME_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv6, HostTest)
{
	ASSERT_EQ(this->url.hostname(), HOST_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv6, PortTest)
{
	ASSERT_EQ(this->url.port(), std::string(PORT_EXPECTED));
}

TEST_F(TestCase_url_UrlParserIPv6, PathTest)
{
	ASSERT_EQ(this->url.path, PATH_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv6, UsernameTest)
{
	ASSERT_EQ(this->url.user.username, USERNAME_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv6, PasswordTest)
{
	ASSERT_EQ(this->url.user.password, PASSWORD_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv6, FramentTest)
{
	ASSERT_EQ(this->url.fragment, FRAGMENT_EXPECTED);
}

TEST_F(TestCase_url_UrlParserIPv6, QueryTest)
{
	ASSERT_EQ(this->url.raw_query, QUERY_EXPECTED);
}

TEST(TestCase_url, parse_url_InvalidSchemeTest)
{
	ASSERT_THROW(http::parse_url("ht~tp://example.com"), ParseError);
}

TEST(TestCase_url, parse_url_InvalidSchemeEndingTest)
{
	ASSERT_TRUE(http::parse_url("http//example.com").scheme.empty());
}

TEST(TestCase_url, parse_url_AbsentFirstSlashTest)
{
	ASSERT_TRUE(http::parse_url("http:#example.com").host.empty());
}

TEST(TestCase_url, parse_url_AbsentSecondSlashTest)
{
	ASSERT_TRUE(http::parse_url("http:/example.com").host.empty());
}

class TestCase_url_SimpleUrl : public ::testing::Test
{
protected:
	http::URL* url{};

	const std::string SCHEME_EXPECTED = "http";
	const std::string HOST_EXPECTED = "127.0.0.1";
	const uint16_t PORT_EXPECTED = 5000;
	const std::string PATH_EXPECTED = "/hello";
	const std::string FRAGMENT_EXPECTED = "world";
	const std::string STR_URL_EXPECTED = "http://127.0.0.1:5000/hello#world";

	void SetUp() override
	{
		this->url = new http::URL();
		this->url->scheme = this->SCHEME_EXPECTED;
		this->url->host = this->HOST_EXPECTED + ":" + std::to_string(this->PORT_EXPECTED);
		this->url->path = this->PATH_EXPECTED;
		this->url->fragment = this->FRAGMENT_EXPECTED;
	}

	void TearDown() override
	{
		delete this->url;
	}
};

TEST_F(TestCase_url_SimpleUrl, StrUrlTest)
{
	ASSERT_EQ(this->url->str(), STR_URL_EXPECTED);
}

TEST_F(TestCase_url_SimpleUrl, SchemeTest)
{
	ASSERT_EQ(this->url->scheme, SCHEME_EXPECTED);
}

TEST_F(TestCase_url_SimpleUrl, HostTest)
{
	ASSERT_EQ(this->url->hostname(), HOST_EXPECTED);
}

TEST_F(TestCase_url_SimpleUrl, PortTest)
{
	ASSERT_EQ(this->url->port(), std::to_string(PORT_EXPECTED));
}

TEST_F(TestCase_url_SimpleUrl, PathTest)
{
	ASSERT_EQ(this->url->path, PATH_EXPECTED);
}

TEST_F(TestCase_url_SimpleUrl, FragmentTest)
{
	ASSERT_EQ(this->url->fragment, FRAGMENT_EXPECTED);
}


class TestCase_url_ComplexUrl : public ::testing::Test
{
protected:
	http::URL* url{};

	const std::string STR_URL_EXPECTED = "http://username:password@example.com:80/hello/world?hello=world&user=pass#someFragment";
	const std::string HOST_EXPECTED = "example.com";
	const uint16_t PORT_EXPECTED = 80;
	const std::string SCHEME_EXPECTED = "http";
	const std::string PATH_EXPECTED = "/hello/world";
	const std::string USERNAME_EXPECTED = "username";
	const std::string PASSWORD_EXPECTED = "password";
	const std::string QUERY_EXPECTED = "hello=world&user=pass";
	const std::string FRAGMENT_EXPECTED = "someFragment";

	void SetUp() override
	{
		this->url = new http::URL();
		this->url->scheme = this->SCHEME_EXPECTED;
		this->url->host = this->HOST_EXPECTED + ":" + std::to_string(this->PORT_EXPECTED);
		this->url->path = this->PATH_EXPECTED;
		this->url->fragment = this->FRAGMENT_EXPECTED;
		this->url->raw_query = this->QUERY_EXPECTED;
		this->url->user = http::URL::user_info_t{this->USERNAME_EXPECTED, this->PASSWORD_EXPECTED};
	}

	void TearDown() override
	{
		delete this->url;
	}
};

TEST_F(TestCase_url_ComplexUrl, StrUrlTest)
{
	ASSERT_EQ(this->url->str(), STR_URL_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, SchemeTest)
{
	ASSERT_EQ(this->url->scheme, SCHEME_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, HostTest)
{
	ASSERT_EQ(this->url->hostname(), HOST_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, PortTest)
{
	ASSERT_EQ(this->url->port(), std::to_string(PORT_EXPECTED));
}

TEST_F(TestCase_url_ComplexUrl, PathTest)
{
	ASSERT_EQ(this->url->path, PATH_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, UsernameTest)
{
	ASSERT_EQ(this->url->user.username, USERNAME_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, PasswordTest)
{
	ASSERT_EQ(this->url->user.password, PASSWORD_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, FramentTest)
{
	ASSERT_EQ(this->url->fragment, FRAGMENT_EXPECTED);
}

TEST_F(TestCase_url_ComplexUrl, QueryTest)
{
	ASSERT_EQ(this->url->raw_query, QUERY_EXPECTED);
}
