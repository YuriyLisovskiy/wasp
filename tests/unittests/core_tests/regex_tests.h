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

#ifndef WASP_UNIT_TESTS_UTILITY_TESTS_REGEX_TESTS_H
#define WASP_UNIT_TESTS_UTILITY_TESTS_REGEX_TESTS_H

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/core/regex.h"


__UNIT_TESTS_BEGIN__

TEST(RegexTestCase, TestETagPattern)
{
	auto expr = core::rgx::Regex(R"(((?:W\/)?"[^"]*"))");
	ASSERT_TRUE(expr.search(R"("bfc13a64729c4290ef5b2c2730249c88ca92d82d")"));
	ASSERT_EQ(expr.groups().size(), 1);

	ASSERT_TRUE(expr.search(R"(W/"0815")"));
	ASSERT_EQ(expr.groups().size(), 1);
}

TEST(ArgRegexTestCase, TestDoubleSquareBracketsInPattern)
{
	auto expr = core::rgx::ArgRegex(R"(<<hello>>([\w\s]+))");
	ASSERT_TRUE(expr.match("<<hello>>Double square brackets check"));
	ASSERT_FALSE(expr.match("Double square brackets check"));
	ASSERT_EQ(expr.groups().size(), 0);
}

TEST(ArgRegexTestCase, TestSingleSquareBracketsInPattern)
{
	auto expr = core::rgx::ArgRegex(R"(\w{2}\s{5}<hello>([\w\s]+))");
	ASSERT_FALSE(expr.match("<<hello>>Double square brackets check"));
	ASSERT_TRUE(expr.search("SS     Double square brackets check"));
	ASSERT_EQ(expr.groups().size(), 1);
	ASSERT_EQ(expr.group("hello"), "Double square brackets check");
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILITY_TESTS_REGEX_TESTS_H
