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

#include <gtest/gtest.h>

#include "../../../src/core/path.h"

using namespace wasp;


TEST(PathTestCase, DirnameTest)
{
	std::string expected("/foo");
	std::string fullPath("/foo/bar");
	ASSERT_EQ(core::path::dirname(fullPath), expected);

	expected = "";
	fullPath = "bar";
	ASSERT_EQ(core::path::dirname(fullPath), expected);

	expected = "/";
	fullPath = "/bar";
	ASSERT_EQ(core::path::dirname(fullPath), expected);
}

TEST(PathTestCase, DirnameTrimDotsTest)
{
	std::string expected("/foo");
	std::string fullPath("/foo/./");
	ASSERT_EQ(core::path::dirname(fullPath), expected);

	expected = "/foo/..";
	fullPath = "/foo/../";
	ASSERT_EQ(core::path::dirname(fullPath), expected);

	expected = "/foo/../..";
	fullPath = "/foo/../../";
	ASSERT_EQ(core::path::dirname(fullPath), expected);

	expected = "/foo/..";
	fullPath = "/foo/.././";
	ASSERT_EQ(core::path::dirname(fullPath), expected);

	expected = "/foo/...";
	fullPath = "/foo/.../";
	ASSERT_EQ(core::path::dirname(fullPath), expected);
}

TEST(PathTestCase, FilenameTest)
{
	std::string expected("bar.txt");
	std::string fullPath("/foo/bar.txt");
	ASSERT_EQ(core::path::basename(fullPath), expected);

	expected = "bar.txt";
	fullPath = "bar.txt";
	ASSERT_EQ(core::path::basename(fullPath), expected);

	expected = "bar.txt";
	fullPath = "/bar.txt";
	ASSERT_EQ(core::path::basename(fullPath), expected);
}

TEST(PathTestCase, JoinTest)
{
	std::string left("/foo/");
	std::string right("//bar/");
	std::string expected("/foo/bar/");
	auto actual = core::path::join(left, right);
	ASSERT_EQ(expected, actual);
}

TEST(PathTestCase, JoinVectorTest)
{
	std::vector<std::string> paths {
		"", "/foo/", "//bar/", "", "/"
	};
	std::string expected("./foo/bar");
	auto actual = core::path::join(paths);
	ASSERT_EQ(expected, actual);
}
