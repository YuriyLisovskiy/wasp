/**
 * http/tests_utility.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/http/utility.h"

using namespace xw;


TEST(IsSameDomainTestCase, TestPatternIsEmpty)
{
	auto domain = ".localhost";
	ASSERT_FALSE(http::is_same_domain(domain, ""));
}

TEST(IsSameDomainTestCase, TestReturnsFalse)
{
	auto domain = ".local";
	auto pattern = ".localhost";
	ASSERT_FALSE(http::is_same_domain(domain, pattern));

	domain = "www.example.com";
	pattern = "example.com.ua";
	ASSERT_FALSE(http::is_same_domain(domain, pattern));
}

TEST(IsSameDomainTestCase, TestReturnsTrue)
{
	auto domain = ".localhost";
	auto pattern = ".localhost";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));

	domain = "example.com";
	pattern = ".example.com";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));

	domain = "127.0.0.1";
	pattern = "127.0.0.1";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));

	domain = "[::1]";
	pattern = "[::1]";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));
}

TEST(SplitDomainPort_TestCase, split_domain_port)
{
	std::string host = "127.0.0.1:8000";
	std::string domain, port;
	http::split_domain_port(host, domain, port);
	ASSERT_EQ(domain, "127.0.0.1");
	ASSERT_EQ(port, "8000");
}

TEST(ValidateHost_TestCase, validate_host)
{
	std::string domain = "127.0.0.1";
	std::vector<std::string> allowed_hosts = {".localhost", "127.0.0.1", "::1"};
	ASSERT_TRUE(http::validate_host(domain, allowed_hosts));
}

template <typename T>
bool assert_vector(const std::vector<T>& actual, const std::vector<T>& expected)
{
	if (actual.size() != expected.size())
	{
		return false;
	}

	for (size_t i = 0; i < actual.size(); i++)
	{
		if (actual[i] != expected[i])
		{
			return false;
		}
	}

	return true;
}

TEST(ParseHttpDateTestCase, ParseGMTDate)
{
	auto actual = http::parse_http_date("Fri, 15 Nov 2019 12:45:26 GMT");
	long expected = 1573821926;
	ASSERT_EQ(actual, expected);
}

TEST(QuoteETagTestCase, NeedQuotesTest)
{
	std::string e_tag = "33a64df551425fcc55e4d42a148795d9f25f89d4";
	std::string expected = "\"" + e_tag + "\"";
	auto actual = http::quote_etag(e_tag);
	ASSERT_EQ(expected, actual);
}

TEST(QuoteETagTestCase, AlreadyQuotedTest)
{
	std::string expected = R"("33a64df551425fcc55e4d42a148795d9f25f89d4")";
	auto actual = http::quote_etag(expected);
	ASSERT_EQ(expected, actual);

	expected = R"(W/"0815")";
	actual = http::quote_etag(expected);
	ASSERT_EQ(expected, actual);
}

TEST(ParseETagsTestCase, MatchAllTest)
{
	auto e_tags = "*";
	std::vector<std::string> expected = {"*"};
	auto actual = http::parse_etags(e_tags);
	ASSERT_TRUE(assert_vector(actual, expected));
}

TEST(ParseETagsTestCase, SingleETagTest)
{
	auto e_tags = R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")";
	std::vector<std::string> expected = {R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")"};
	auto actual = http::parse_etags(e_tags);
	ASSERT_TRUE(assert_vector(actual, expected));
}

TEST(ParseETagsTestCase, MultipleETagsTest)
{
	auto e_tags = R"(W/"67ab43", "54ed21", "7892dd")";
	std::vector<std::string> expected = {R"(W/"67ab43")", R"("54ed21")", R"("7892dd")"};
	auto actual = http::parse_etags(e_tags);
	ASSERT_TRUE(assert_vector(actual, expected));
}
