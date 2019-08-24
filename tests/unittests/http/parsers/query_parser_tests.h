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

#ifndef WASP_UNIT_TESTS_HTTP_PARSERS_QUERY_PARSER_TESTS_H
#define WASP_UNIT_TESTS_HTTP_PARSERS_QUERY_PARSER_TESTS_H

#include <gtest/gtest.h>
#include <map>
#include <vector>

#include "../../globals.h"
#include "../../../../src/http/parsers/query_parser.h"

__INTERNAL_BEGIN__

TEST(QueryParserTestCase, get)
{
	auto expected = std::map<std::string, std::string> {
		std::pair<std::string, std::string>("hello", "world"),
		std::pair<std::string, std::string>("world", "hello"),
		std::pair<std::string, std::string>("hello_key", "hello_value"),
		std::pair<std::string, std::string>("world_key", "world_value"),
	};
	auto actual = QueryParser().parse("hello=world&world=hello&hello_key=hello_value&world_key=world_value");

	for (const auto& item : expected)
	{
		ASSERT_TRUE(actual->contains(item.first));
		ASSERT_EQ(item.second, actual->get(item.first));
	}
	delete actual;
}

TEST(QueryParserTestCase, getList)
{
	std::string key = "hello";
	auto expected = std::vector<std::string> {
		"world", "hello", "hello_value"
	};
	auto parameters = QueryParser().parse("hello=world&hello=hello&hello=hello_value");

	ASSERT_TRUE(parameters->containsList(key));

	auto actual = parameters->getList(key);
	delete parameters;

	ASSERT_EQ(actual.size(), expected.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

__INTERNAL_END__


#endif // WASP_UNIT_TESTS_HTTP_PARSERS_QUERY_PARSER_TESTS_H
