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


TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetInvalidCast)
{
	auto array = YAMLArray();
	array.add(std::make_shared<YAMLObject>());

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(array.get_array(0), nullptr);
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetObject)
{
	auto obj = std::make_shared<YAMLObject>();
	auto array = YAMLArray();
	array.add(obj);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(obj.get(), array.get_object(0));
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetArray)
{
	auto arr = std::make_shared<YAMLArray>();
	auto array = YAMLArray();
	array.add("some_array", arr);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(arr.get(), array.get_object(0)->get_array("some_array"));
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetBool)
{
	auto value = true;
	auto array = YAMLArray();
	array.add(value);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(value, array.get_bool(0));
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetChar)
{
	char value = 'c';
	auto array = YAMLArray();
	array.add(value);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(value, array.get_char(0));
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetInt)
{
	int value = 10;
	auto array = YAMLArray();
	array.add(value);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(value, array.get_int(0));
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetDouble)
{
	auto value = 10.5;
	auto array = YAMLArray();
	array.add(value);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(value, array.get_double(0));

	float float_value = 10.5f;
	array.add(float_value);

	ASSERT_EQ(array.size(), 2);
	ASSERT_EQ(float_value, array.get_double(1));
}

TEST(CoreTests_YamlTests_YamlArrayTests_GetTestCase, TestGetString)
{
	auto value = std::string("Hello, Home!");
	auto array = YAMLArray();
	array.add(value);

	ASSERT_EQ(array.size(), 1);
	ASSERT_EQ(value, array.get_string(0));
}
