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


TEST(CoreTests_YamlTests_YamlObjectTests_StringReprTestCase, TestToString)
{
	/*
	auto obj = std::make_shared<YAMLObject>();
	obj->put("integer", 1);
	obj->put("multi_line_text", "some text\nwith\nnewlines!");

	auto inner_arr = std::make_shared<YAMLArray>();
	inner_arr->add(10);
	inner_arr->add('c');
	inner_arr->add("Hello, Home!");
*/
	auto obj = YAMLObject();

	auto json_arr = std::make_shared<YAMLArray>();
	json_arr->add("rigid");
	json_arr->add("better for data interchange");
	obj.put("json", json_arr);

	auto yaml_arr = std::make_shared<YAMLArray>();
	yaml_arr->add("slim and flexible");
	yaml_arr->add("better for configuration");
	obj.put("yaml", yaml_arr);

	auto object = std::make_shared<YAMLObject>();
	object->put("key", "value");

	auto array = std::make_shared<YAMLArray>();

	auto null_value = std::make_shared<YAMLObject>();
	null_value->put("null_value", "");
	array->add(null_value);

	auto boolean = std::make_shared<YAMLObject>();
	boolean->put("boolean", true);
	array->add(boolean);

	auto integer = std::make_shared<YAMLObject>();
	integer->put("integer", 1);
	array->add(integer);

	object->put("array", array);

	obj.put("object", object);

	obj.put("content", "We can\nauto convert\nline breaks");

	auto ind = YAMLArray::DEFAULT_INDENT;
	std::string expected = "---\n\njson:\n" +
		ind + "- rigid\n" +
		ind + "- better for data interchange\n" +
		"yaml:\n" +
		ind + "- slim and flexible\n" +
		ind + "- better for configuration\n" +
		"object:\n" +
		ind + "key: value\n" +
		ind + "array:\n" +
		ind + ind + "- null_value:\n" +
		ind + ind + "- boolean: true\n" +
		ind + ind + "- integer: 1\n" +
		"content: |-\n" +
		ind + "We can\n" +
		ind + "auto convert\n" +
		ind + "line breaks";

	auto actual = obj.to_string();
	ASSERT_EQ(actual, expected);
}
