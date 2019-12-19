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

#ifndef WASP_UNIT_TESTS_UTILITY_TESTS_PATH_TESTS_H
#define WASP_UNIT_TESTS_UTILITY_TESTS_PATH_TESTS_H

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/utility/path.h"


__UNIT_TESTS_BEGIN__

TEST(PathTestCase, baseName)
{
	std::string expected("/bar");
	std::string fullPath("/foo/bar");
	ASSERT_EQ(path::base(fullPath), expected);

	expected = "bar";
	fullPath = "bar";
	ASSERT_EQ(path::base(fullPath), expected);

	expected = "/bar";
	fullPath = "/bar";
	ASSERT_EQ(path::base(fullPath), expected);
}

TEST(PathTestCase, JoinTest)
{
	std::string left("/foo/");
	std::string right("//bar/");
	std::string expected("/foo/bar/");
	auto actual = path::join(left, right);
	ASSERT_EQ(expected, actual);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILITY_TESTS_PATH_TESTS_H
