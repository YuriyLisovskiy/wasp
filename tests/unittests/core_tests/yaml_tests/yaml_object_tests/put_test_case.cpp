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


TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddObject)
{
	auto obj = YAMLObject();
	obj.put("some_value", std::make_shared<YAMLObject>());

	ASSERT_EQ(obj.size(), 1);

	auto another_obj = std::make_shared<YAMLObject>();
	obj.put("some_another_value", another_obj);
	ASSERT_EQ(obj.size(), 2);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddArray)
{
	auto obj = YAMLObject();
	obj.put("some_array1", std::make_shared<YAMLArray>());

	ASSERT_EQ(obj.size(), 1);

	auto another_arr = std::make_shared<YAMLArray>();
	obj.put("some_array2", another_arr);
	ASSERT_EQ(obj.size(), 2);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddArrayAndObject)
{
	auto obj = YAMLObject();
	obj.put("some_array", std::make_shared<YAMLArray>());

	ASSERT_EQ(obj.size(), 1);

	auto another_obj = std::make_shared<YAMLObject>();
	obj.put("some_value", another_obj);
	ASSERT_EQ(obj.size(), 2);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddInteger)
{
	auto obj = YAMLObject();
	obj.put("some_value", 10);

	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddDouble)
{
	auto obj = YAMLObject();
	obj.put("some_value", 10.5);

	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddFloat)
{
	auto obj = YAMLObject();
	obj.put("some_value", 10.5f);

	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddChar)
{
	auto obj = YAMLObject();
	obj.put("some_value", 'c');

	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddString)
{
	auto obj = YAMLObject();
	obj.put("some_value", std::string("Hello, Home"));

	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_AddObjectTestCase, TestAddCString)
{
	auto obj = YAMLObject();
	obj.put("some_value", "Hello, Home");

	ASSERT_EQ(obj.size(), 1);
}
