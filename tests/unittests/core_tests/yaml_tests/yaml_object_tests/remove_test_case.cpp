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


TEST(CoreTests_YamlTests_YamlObjectTests_RemoveTestCase, TestRemoveAt)
{
	auto obj = YAMLObject();
	obj.put("object", std::make_shared<YAMLObject>());
	obj.put("number", 10);
	obj.put("string", "Hello, Home!");
	ASSERT_EQ(obj.size(), 3);

	obj.remove("number");
	ASSERT_EQ(obj.size(), 2);

	obj.remove("object");
	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_RemoveTestCase, TestPopAt)
{
	auto inner_obj = std::make_shared<YAMLObject>();
	auto obj = YAMLObject();
	obj.put("object", inner_obj);
	obj.put("number", 10);
	obj.put("string", "Hello, Home!");
	ASSERT_EQ(obj.size(), 3);

	auto pop_num = dynamic_cast<YAMLValue*>(obj.pop("number").get());
	ASSERT_EQ(pop_num->get(), "10");
	ASSERT_EQ(obj.size(), 2);

	auto pop_obj = dynamic_cast<YAMLObject*>(obj.pop("object").get());
	ASSERT_EQ(pop_obj, inner_obj.get());
	ASSERT_EQ(obj.size(), 1);
}

TEST(CoreTests_YamlTests_YamlObjectTests_RemoveTestCase, TestClear)
{
	auto obj = YAMLObject();
	obj.put("obj1", std::make_shared<YAMLObject>());
	obj.put("obj2", std::make_shared<YAMLObject>());
	obj.put("obj3", std::make_shared<YAMLObject>());
	obj.put("obj4", std::make_shared<YAMLObject>());
	ASSERT_EQ(obj.size(), 4);

	obj.clear();
	ASSERT_EQ(obj.size(), 0);
}
