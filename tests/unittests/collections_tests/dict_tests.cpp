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

#include "../../../src/collections/dict.h"

using namespace wasp;


// Dict testing.
TEST(DictTestCase, TestGetItemExists)
{
	collections::Dict<int, int> testDict({
		{1, 2},
		{2, 3},
		{3, 4}
	});
	ASSERT_EQ(testDict.get(1, 0), 2);
	ASSERT_EQ(testDict.get(2, 0), 3);
	ASSERT_EQ(testDict.get(3, 0), 4);
}

TEST(DictTestCase, TestGetItemNotExists)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2}
	});
	ASSERT_EQ(testDict.get(3, 0), 0);
}

TEST(DictTestCase, TestSet)
{
	collections::Dict<int, int> testDict;
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, 5);
	ASSERT_EQ(testDict.get(3, 0), 5);
}

TEST(DictTestCase, TestRemove)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2}
	});
	ASSERT_EQ(testDict.get(1, 0), 2);
	testDict.remove(1);
	ASSERT_EQ(testDict.get(1, 0), 0);
}

TEST(DictTestCase, TestClear)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2}, {3, 4}
	});
	ASSERT_FALSE(testDict.is_empty());
	testDict.clear();
	ASSERT_TRUE(testDict.is_empty());
}

TEST(DictTestCase, TestSize)
{
	collections::Dict<int, int> testDict;
	ASSERT_EQ(testDict.size(), 0);
	testDict.set(1, 2);
	ASSERT_EQ(testDict.size(), 1);
	testDict.set(2, 5);
	ASSERT_EQ(testDict.size(), 2);
}

TEST(DictTestCase, TestContains)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2}
	});
	ASSERT_TRUE(testDict.contains(1));
	ASSERT_FALSE(testDict.contains(2));
}

TEST(DictTestCase, TestIsEmpty)
{
	collections::Dict<int, int> testDict;
	ASSERT_TRUE(testDict.is_empty());
}

TEST(DictTestCase, TestIsNotEmpty)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2}
	});
	ASSERT_FALSE(testDict.is_empty());
}

TEST(DictTestCase, TestBeginNotThrows)
{
	collections::Dict<int, int> testDict;
	ASSERT_NO_THROW(testDict.begin());
}

TEST(DictTestCase, TestEndNotThrows)
{
	collections::Dict<int, int> testDict;
	ASSERT_NO_THROW(testDict.end());
}

TEST(DictTestCase, TestRBeginNotThrows)
{
	collections::Dict<int, int> testDict;
	ASSERT_NO_THROW(testDict.rbegin());
}

TEST(DictTestCase, TestREndNotThrows)
{
	collections::Dict<int, int> testDict;
	ASSERT_NO_THROW(testDict.rend());
}

TEST(DictTestCase, TestGetKeys)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2},
		{2, 3},
		{3, 4}
	});
	std::vector<int> expected = {1, 2, 3};
	auto actual = testDict.keys();

	ASSERT_EQ(expected.size(), actual.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST(DictTestCase, TestGetSize)
{
	collections::Dict<int, int> testDict(std::map<int, int>{
		{1, 2},
		{2, 3},
		{3, 4}
	});
	auto expected = 3;
	auto actual = testDict.size();

	ASSERT_EQ(expected, actual);
}
