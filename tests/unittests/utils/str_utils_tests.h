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

#ifndef WASP_UNIT_TESTS_UTILS_STR_TESTS_H
#define WASP_UNIT_TESTS_UTILS_STR_TESTS_H

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/utils/str.h"


__UNIT_TESTS_BEGIN__

TEST(StrUtilsUrlSplitTypeTestCase, TestSuccess)
{
	std::string schemeActual, dataActual;
	wasp::str::urlSplitType("http://example.com/file.html", schemeActual, dataActual);
	ASSERT_EQ(schemeActual, "http");
	ASSERT_EQ(dataActual, "//example.com/file.html");
}

TEST(StrUtilsUrlSplitTypeTestCase, TestMissingColon)
{
	std::string schemeActual, dataActual;
	wasp::str::urlSplitType("http//example.com/file.html", schemeActual, dataActual);
	ASSERT_EQ(schemeActual, "");
	ASSERT_EQ(dataActual, "http//example.com/file.html");
}

TEST(StrUtilsUrlSplitTypeTestCase, TestSlashBeforeColon)
{
	std::string schemeActual, dataActual;
	wasp::str::urlSplitType("http/://example.com/file.html", schemeActual, dataActual);
	ASSERT_EQ(schemeActual, "");
	ASSERT_EQ(dataActual, "http/://example.com/file.html");
}

TEST(StrUtilsUrlSplitTypeTestCase, TestEmptyInput)
{
	std::string schemeActual, dataActual;
	wasp::str::urlSplitType("", schemeActual, dataActual);
	ASSERT_EQ(schemeActual, "");
	ASSERT_EQ(dataActual, "");
}

TEST(StrUtilsSplitTestCase, TestSplitByDefaultChar)
{
	std::string toSplit("Alphanumeric and printable shellcode");
	std::vector<std::string> expected = {"Alphanumeric", "and", "printable", "shellcode"};
	auto actual = wasp::str::split(toSplit);
	ASSERT_EQ(expected.size(), actual.size());
	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST(StrUtilsSplitTestCase, TestSplit)
{
	std::string toSplit("Alphanumeric:and: printable : shellcode");
	std::vector<std::string> expected = {"Alphanumeric", "and", " printable ", " shellcode"};
	auto actual = wasp::str::split(toSplit, ':');
	ASSERT_EQ(expected.size(), actual.size());
	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST(StrUtilsJoinVectorTestCase, TestJoinStringVectorWithSpace)
{
	std::string expected = "Alphanumeric and printable shellcode";
	std::vector<std::string> strVec = {"Alphanumeric", "and", "printable", "shellcode"};
	ASSERT_EQ(wasp::str::join<std::string>(" ", strVec), expected);
}

TEST(StrUtilsJoinVectorTestCase, TestJoinIntVectorWithComma)
{
	std::string expected = "{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}";
	std::vector<int> intVec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	ASSERT_EQ("{" + wasp::str::join<int>(", ", intVec) + "}", expected);
}

TEST(StrUtilsJoinVectorTestCase, TestJoinCharVectorWithputSpace)
{
	std::string expected = "Alphanumeric";
	std::vector<char> intVec = {'A', 'l', 'p', 'h', 'a', 'n', 'u', 'm', 'e', 'r', 'i', 'c'};
	ASSERT_EQ(wasp::str::join("", intVec), expected);
}

// Dict
TEST(StrUtilsJoinDictTestCase, TestJoinStringVectorWithSpace)
{
	std::string expected = "Alphanumeric and printable shellcode";
	wasp::Dict<std::string, std::string> strDict = wasp::Dict<std::string, std::string>(
		std::map<std::string, std::string>{
			{"Alphanumeric", "and"},
			{"printable", "shellcode"}
		}
	);
	ASSERT_EQ(wasp::str::join(" ", strDict), expected);
}

TEST(StrUtilsJoinDictTestCase, TestJoinIntDictWithAsJson)
{
	std::string expected = "{\n\t\"key1\": \"some value\",\n\t\"some other key\": \"2019\"\n}";
	wasp::Dict<std::string, std::string> strDict = wasp::Dict<std::string, std::string>(
		std::map<std::string, std::string>{
			{"key1", "some value"},
			{"some other key", "2019"}
		}
	);
	std::string actual = wasp::str::join<std::string, std::string>(
		",\n\t",
		strDict,
		[](const std::pair<std::string, std::string>& p) -> std::string { return "\"" + p.first + "\": \"" + p.second + "\""; }
	);
	ASSERT_EQ("{\n\t" + actual + "\n}", expected);
}

// Trim functions
TEST(StrUtilsLTrimTestCase, TestLeftTrimmingInPlace)
{
	std::string expected = "Hello, World   ";
	std::string actual = "       " + expected;
	wasp::str::ltrim(actual);
	ASSERT_EQ(actual, expected);

	expected = "Hello, World  - ---";
	actual = "------" + expected;
	wasp::str::ltrim(actual, '-');
	ASSERT_EQ(actual, expected);
}

TEST(StrUtilsRTrimTestCase, TestRightTrimmingInPlace)
{
	std::string expected = "      Hello, World";
	std::string actual = expected + "       ";
	wasp::str::rtrim(actual);
	ASSERT_EQ(actual, expected);

	expected = "--------Hello, World";
	actual = expected + "------";
	wasp::str::rtrim(actual, '-');
	ASSERT_EQ(actual, expected);
}

TEST(StrUtilsTrimTestCase, TestTrimmingInPlace)
{
	std::string expected = "Hello, World      --- - ---";
	std::string actual = "       " + expected + "      ";
	wasp::str::trim(actual);
	ASSERT_EQ(actual, expected);

	expected = "Hello, World";
	actual = "------" + expected + "----";
	wasp::str::trim(actual, '-');
	ASSERT_EQ(actual, expected);
}

TEST(StrUtilsLTrimTestCase, TestLeftTrimmingCopy)
{
	std::string expected = "Hello, World   ";
	ASSERT_EQ(wasp::str::ltrim("       " + expected), expected);

	expected = "Hello, World  - ---";
	ASSERT_EQ(wasp::str::ltrim("------" + expected, '-'), expected);
}

TEST(StrUtilsRTrimTestCase, TestRightTrimmingCopy)
{
	std::string expected = "      Hello, World";
	ASSERT_EQ(wasp::str::rtrim(expected + "       "), expected);

	expected = "--------Hello, World";
	ASSERT_EQ(wasp::str::rtrim(expected + "------", '-'), expected);
}

TEST(StrUtilsTrimTestCase, TestTrimmingCopy)
{
	std::string expected = "Hello, World      --- - ---";
	ASSERT_EQ(wasp::str::trim("       " + expected + "      "), expected);

	expected = "Hello, World";
	ASSERT_EQ(wasp::str::trim("------" + expected + "----", '-'), expected);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILS_STR_TESTS_H
