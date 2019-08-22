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

#ifndef WASP_UNIT_TESTS_UTILS_PATH_TESTS_H
#define WASP_UNIT_TESTS_UTILS_PATH_TESTS_H

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/utils/path.h"


__UNIT_TESTS_BEGIN__

TEST(PathTestCase, baseName)
{
	std::string expected("/bar");
	std::string fullPath("/foo/bar");
	ASSERT_EQ(wasp::path::baseName(fullPath), expected);

	expected = "bar";
	fullPath = "bar";
	ASSERT_EQ(wasp::path::baseName(fullPath), expected);

	expected = "/bar";
	fullPath = "/bar";
	ASSERT_EQ(wasp::path::baseName(fullPath), expected);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILS_PATH_TESTS_H