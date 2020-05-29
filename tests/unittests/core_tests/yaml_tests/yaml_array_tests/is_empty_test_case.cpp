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


TEST(CoreTests_YamlTests_YamlArrayTests_IsEmptyTestCase, TestIsEmpty)
{
	auto array = YAMLArray();

	ASSERT_TRUE(array.is_empty());
}

TEST(CoreTests_YamlTests_YamlArrayTests_IsEmptyTestCase, TestIsNotEmpty)
{
	auto array = YAMLArray();
	array.add(std::make_shared<YAMLObject>());

	ASSERT_FALSE(array.is_empty());
}
