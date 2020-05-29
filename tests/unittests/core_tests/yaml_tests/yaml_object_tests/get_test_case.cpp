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


TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetInvalidCast)
{
	auto obj = YAMLObject();
	obj.put("some_obj", std::make_shared<YAMLObject>());

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(obj.get_array("some_obj"), nullptr);
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetObject)
{
	auto inner_obj = std::make_shared<YAMLObject>();
	auto obj = YAMLObject();
	obj.put("some_obj", inner_obj);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(inner_obj.get(), obj.get_object("some_obj"));
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetArray)
{
	auto arr = std::make_shared<YAMLArray>();
	auto obj = YAMLObject();
	obj.put("some_array", arr);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(arr.get(), obj.get_array("some_array"));
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetBool)
{
	auto value = true;
	auto obj = YAMLObject();
	obj.put("some_value", value);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(value, obj.get_bool("some_value"));
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetChar)
{
	char value = 'c';
	auto obj = YAMLObject();
	obj.put("some_value", value);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(value, obj.get_char("some_value"));
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetInt)
{
	int value = 10;
	auto obj = YAMLObject();
	obj.put("some_value", value);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(value, obj.get_int("some_value"));
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetDouble)
{
	auto value = 10.5;
	auto obj = YAMLObject();
	obj.put("some_double_value", value);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(value, obj.get_double("some_double_value"));

	float float_value = 10.5f;
	obj.put("some_float_value", float_value);

	ASSERT_EQ(obj.size(), 2);
	ASSERT_EQ(float_value, obj.get_double("some_float_value"));
}

TEST(CoreTests_YamlTests_YamlObjectTests_GetTestCase, TestGetString)
{
	auto value = std::string("Hello, Home!");
	auto obj = YAMLObject();
	obj.put("some_value", value);

	ASSERT_EQ(obj.size(), 1);
	ASSERT_EQ(value, obj.get_string("some_value"));
}
