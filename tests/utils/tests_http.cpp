/**
 * utils/tests_http.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <string>

#include <gtest/gtest.h>

#include "../../src/utils/http.h"

using namespace xw;


template <typename _T>
bool assert_vector(const std::vector<_T>& actual, const std::vector<_T>& expected)
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
	auto actual = utils_http::parse_http_date("Fri, 15 Nov 2019 12:45:26 GMT");
	long expected = 1573814726;
	ASSERT_EQ(actual, expected);
}

TEST(QuoteETagTestCase, NeedQuotesTest)
{
	std::string e_tag = "33a64df551425fcc55e4d42a148795d9f25f89d4";
	std::string expected = "\"" + e_tag + "\"";
	auto actual = utils_http::quote_etag(e_tag);
	ASSERT_EQ(expected, actual);
}

TEST(QuoteETagTestCase, AlreadyQuotedTest)
{
	std::string expected = R"("33a64df551425fcc55e4d42a148795d9f25f89d4")";
	auto actual = utils_http::quote_etag(expected);
	ASSERT_EQ(expected, actual);

	expected = R"(W/"0815")";
	actual = utils_http::quote_etag(expected);
	ASSERT_EQ(expected, actual);
}

TEST(ParseETagsTestCase, MatchAllTest)
{
	auto e_tags = "*";
	std::vector<std::string> expected = {"*"};
	auto actual = utils_http::parse_etags(e_tags);
	ASSERT_TRUE(assert_vector(actual, expected));
}

TEST(ParseETagsTestCase, SingleETagTest)
{
	auto e_tags = R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")";
	std::vector<std::string> expected = {R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")"};
	auto actual = utils_http::parse_etags(e_tags);
	ASSERT_TRUE(assert_vector(actual, expected));
}

TEST(ParseETagsTestCase, MultipleETagsTest)
{
	auto e_tags = R"(W/"67ab43", "54ed21", "7892dd")";
	std::vector<std::string> expected = {R"(W/"67ab43")", R"("54ed21")", R"("7892dd")"};
	auto actual = utils_http::parse_etags(e_tags);
	ASSERT_TRUE(assert_vector(actual, expected));
}
