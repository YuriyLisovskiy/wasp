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

#include "../../../../../src/core/yaml/yaml_array.h"

using namespace xw::core::yaml;


TEST(CoreTests_YamlTests_YamlArrayTests_AddObjectTestCase, TestAddObject)
{
	auto array = YAMLArray();
	array.add(std::make_shared<YAMLObject>());

	ASSERT_EQ(array.size(), 1);

	auto another_obj = std::make_shared<YAMLObject>();
	array.add(another_obj);
	ASSERT_EQ(array.size(), 2);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddObjectTestCase, TestAddArray)
{
	auto array = YAMLArray();
	array.add("some_array1", std::make_shared<YAMLArray>());

	ASSERT_EQ(array.size(), 1);

	auto another_arr = std::make_shared<YAMLArray>();
	array.add("some_array2", another_arr);
	ASSERT_EQ(array.size(), 2);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddObjectTestCase, TestAddArrayAndObject)
{
	auto array = YAMLArray();
	array.add("some_array", std::make_shared<YAMLArray>());

	ASSERT_EQ(array.size(), 1);

	auto another_obj = std::make_shared<YAMLObject>();
	array.add(another_obj);
	ASSERT_EQ(array.size(), 2);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddTestCase, TestAddInteger)
{
	auto array = YAMLArray();
	array.add(10);

	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddTestCase, TestAddDouble)
{
	auto array = YAMLArray();
	array.add(10.5);

	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddTestCase, TestAddFloat)
{
	auto array = YAMLArray();
	array.add(10.5f);

	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddTestCase, TestAddChar)
{
	auto array = YAMLArray();
	array.add('c');

	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddTestCase, TestAddString)
{
	auto array = YAMLArray();
	array.add<std::string>(std::string("Hello, Home"));

	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_AddTestCase, TestAddCString)
{
	auto array = YAMLArray();
	array.add("Hello, Home");

	ASSERT_EQ(array.size(), 1);
}
