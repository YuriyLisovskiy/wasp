/**
 * core/parsers/tests_query_parser.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <map>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/core/parsers/query_parser.h"
#include "../../../src/http/request.h"

using namespace xw;


template <typename _Container>
bool containers_are_equal(const _Container& lhs, const _Container& rhs)
{
	return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

TEST(QueryParserTestCase, get)
{
	auto expected = std::map<std::string, std::string> {
		std::pair<std::string, std::string>("hello", "world"),
		std::pair<std::string, std::string>("world", "hello"),
		std::pair<std::string, std::string>("hello_key", "hello_value"),
		std::pair<std::string, std::string>("world_key", "world_value"),
	};
	parsers::query_parser parser;
	parser.parse("hello=world&world=hello&hello_key=hello_value&world_key=world_value");

	http::HttpRequest::Parameters<std::string, xw::string> actual(parser.dict, parser.multi_dict);

	for (const auto& item : expected)
	{
		ASSERT_TRUE(actual.contains(item.first));
		ASSERT_EQ(item.second, actual.get(item.first));
	}
}

TEST(QueryParserTestCase, getList)
{
	std::string key = "hello";
	auto expected = std::vector<std::string> {
		"world", "hello", "hello_value"
	};
	parsers::query_parser parser;
	parser.parse("hello=world&hello=hello&hello=hello_value");
	http::HttpRequest::Parameters<std::string, xw::string> parameters(parser.dict, parser.multi_dict);

	ASSERT_TRUE(parameters.contains_list(key));

	auto actual = parameters.get_list(key);

	ASSERT_EQ(actual.size(), expected.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST(QueryParserTestCase, keys)
{
	auto expected = std::vector<std::string> {
		"hello",
		"hello_key",
		"world",
		"world_key",
	};
	parsers::query_parser parser;
	parser.parse("hello=world&world=hello&hello_key=hello_value&world_key=world_value");
	http::HttpRequest::Parameters<std::string, xw::string> parsed(parser.dict, parser.multi_dict);
	auto actual = parsed.keys();

	ASSERT_TRUE(containers_are_equal(expected, actual));
}

TEST(QueryParserTestCase, size)
{
	auto expected = 4;
	parsers::query_parser parser;
	parser.parse("hello=world&world=hello&hello_key=hello_value&world_key=world_value");
	http::HttpRequest::Parameters<std::string, xw::string> parsed(parser.dict, parser.multi_dict);
	auto actual = parsed.size();

	ASSERT_EQ(expected, actual);
}

TEST(QueryParserTestCase, empty)
{
	parsers::query_parser parser;
	parser.parse("");
	http::HttpRequest::Parameters<std::string, xw::string> parsed(parser.dict, parser.multi_dict);

	ASSERT_TRUE(parsed.empty());
}
