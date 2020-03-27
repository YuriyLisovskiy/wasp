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

#include <string>

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../utils.h"
#include "../../../src/utils/http.h"


__UNIT_TESTS_BEGIN__

TEST(ParseHttpDateTestCase, ParseGMTDate)
{
	auto actual = utils_http::parse_http_date("Fri, 15 Nov 2019 12:45:26 GMT");
	long expected = 1573821926;
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
	ASSERT_TRUE(utils::assert_vector(actual, expected));
}

TEST(ParseETagsTestCase, SingleETagTest)
{
	auto e_tags = R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")";
	std::vector<std::string> expected = {R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")"};
	auto actual = utils_http::parse_etags(e_tags);
	ASSERT_TRUE(utils::assert_vector(actual, expected));
}

TEST(ParseETagsTestCase, MultipleETagsTest)
{
	auto e_tags = R"(W/"67ab43", "54ed21", "7892dd")";
	std::vector<std::string> expected = {R"(W/"67ab43")", R"("54ed21")", R"("7892dd")"};
	auto actual = utils_http::parse_etags(e_tags);
	ASSERT_TRUE(utils::assert_vector(actual, expected));
}

__UNIT_TESTS_END__
