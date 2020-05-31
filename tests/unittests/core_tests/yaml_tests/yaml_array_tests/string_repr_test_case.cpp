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


TEST(CoreTests_YamlTests_YamlArrayTests_StringReprTestCase, TestToString)
{
	auto obj = std::make_shared<YAMLObject>();
	obj->put("integer", 1);
	obj->put("multi_line_text", "some text\nwith\nnewlines!");

	auto inner_arr = std::make_shared<YAMLArray>();
	inner_arr->add(10);
	inner_arr->add('c');
	inner_arr->add("Hello, Home!");

	auto arr = YAMLArray();
	arr.add(obj);
	arr.add("Some single line text");
	arr.add("inner_array", inner_arr);
	arr.add("word");
	arr.add(7);
	arr.add(20.145788f);

	auto ind = YAMLArray::DEFAULT_INDENT;

	std::string expected = "---\n"
	    "\n"
	    "- integer: 1\n" +
		ind + "multi_line_text: |-\n" +
		ind + ind + "some text\n" +
		ind + ind + "with\n" +
		ind + ind + "newlines!\n" +
	    "- Some single line text\n" +
	    "- inner_array:\n" +
		ind + ind + "- 10\n" +
		ind + ind + "- c\n" +
		ind + ind + "- Hello, Home!\n" +
		"- word\n" +
	    "- 7\n" +
		"- 20.145788";
	auto actual = arr.to_string();

	std::cout << actual;

	ASSERT_EQ(actual, expected);
}
