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

#include <vector>

#include <gtest/gtest.h>

#include "../../../../src/core/types/array.h"

using namespace wasp;


TEST(ArrayTestCase, AssignTest)
{
	using Array = core::types::Array;
	using namespace core::types;

	Array null_arr;
	ASSERT_FALSE(null_arr);

	Array arr;
	arr = std::vector<int>{10, 2, 5};
	ASSERT_TRUE(arr);

	ASSERT_EQ(*(arr[0]->__cast__<Value<int>>()), 10);
	ASSERT_EQ(*(arr[1]->__cast__<Value<int>>()), 2);
	ASSERT_EQ(*(arr[2]->__cast__<Value<int>>()), 5);

	arr.push_back(10.501);
	arr.push_back(std::string("hello"));
	arr.push_back("c-string");

	ASSERT_EQ(*(arr[3]->__cast__<Value<double>>()), 10.501);
	ASSERT_EQ(*(arr[4]->__cast__<Value<std::string>>()), std::string("hello"));
	ASSERT_EQ(*(arr[5]->__cast__<Value<const char*>>()), "c-string");
}
