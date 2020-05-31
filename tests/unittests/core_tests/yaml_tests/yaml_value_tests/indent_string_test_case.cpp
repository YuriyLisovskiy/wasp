/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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

#include <gtest/gtest.h>

#include "../../../../../src/core/yaml/yaml_value.h"

using namespace xw::core::yaml;


TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestSimpleValue)
{
	auto value = YAMLValue("hello_world");

	ASSERT_EQ(value.indent_string("", true), " hello_world");
}

TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestIntegerValue)
{
	auto value = YAMLValue("10");

	ASSERT_EQ(value.indent_string("", true), " 10");
}

TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestDoubleValue)
{
	auto value = YAMLValue("10.57");

	ASSERT_EQ(value.indent_string("", true), " 10.57");
}

TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestContainsSingleQuotesValue)
{
	auto value = YAMLValue("world's first test");

	ASSERT_EQ(value.indent_string("", true), " world's first test");
}

TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestContainsDoubleQuotesValue)
{
	auto value = YAMLValue("world\"s first test");

	ASSERT_EQ(value.indent_string("", true), " world\"s first test");
}

TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestContainsMixedQuotesValue)
{
	auto value = YAMLValue("world's \"first\" test");

	ASSERT_EQ(value.indent_string("", true), " world's \"first\" test");
}

TEST(CoreTests_YamlTests_YamlValueTests_IndentStringTestCase, TestMultilineStringValue)
{
	auto idt = YAMLValue::DEFAULT_INDENT;
	auto value = YAMLValue("world's\nfirst\ntest");

	ASSERT_EQ(
		value.indent_string(idt, true),
		" |-\n" + idt + "world's\n" + idt + "first\n" + idt + "test"
	);
}
