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


TEST(CoreTests_YamlTests_YamlArrayTests_RemoveTestCase, TestRemoveAt)
{
	auto array = YAMLArray();
	array.add(std::make_shared<YAMLObject>());
	array.add(10);
	array.add("Hello, Home!");
	ASSERT_EQ(array.size(), 3);

	array.remove_at(1);
	ASSERT_EQ(array.size(), 2);

	array.remove_at(0);
	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_RemoveTestCase, TestPopAt)
{
	auto obj = std::make_shared<YAMLObject>();
	auto array = YAMLArray();
	array.add(obj);
	array.add(10);
	array.add("Hello, Home!");
	ASSERT_EQ(array.size(), 3);

	auto pop_num = dynamic_cast<YAMLValue*>(array.pop_at(1).get());
	ASSERT_EQ(pop_num->get(), "10");
	ASSERT_EQ(array.size(), 2);

	auto pop_obj = dynamic_cast<YAMLObject*>(array.pop_at(0).get());
	ASSERT_EQ(pop_obj, obj.get());
	ASSERT_EQ(array.size(), 1);
}

TEST(CoreTests_YamlTests_YamlArrayTests_RemoveTestCase, TestClear)
{
	auto array = YAMLArray();
	array.add(std::make_shared<YAMLObject>());
	array.add(std::make_shared<YAMLObject>());
	array.add(std::make_shared<YAMLObject>());
	array.add(std::make_shared<YAMLObject>());
	ASSERT_EQ(array.size(), 4);

	array.clear();
	ASSERT_EQ(array.size(), 0);
}
