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

#ifndef WASP_UNIT_TESTS_COLLECTIONS_TESTS_DICT_TESTS_H
#define WASP_UNIT_TESTS_COLLECTIONS_TESTS_DICT_TESTS_H

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/collections/dict.h"


__UNIT_TESTS_BEGIN__

// Mutable dict testing.
TEST(MutableDictTestCase, TestGetItemExists)
{
	wasp::collections::Dict<int, int> testDict({
		{1, 2},
		{2, 3},
		{3, 4}
	}, true);
	ASSERT_EQ(testDict.get(1, 0), 2);
	ASSERT_EQ(testDict.get(2, 0), 3);
	ASSERT_EQ(testDict.get(3, 0), 4);
}

TEST(MutableDictTestCase, TestGetItemNotExists)
{
	wasp::collections::Dict<int, int> testDict({
		{1, 2}
	}, true);
	ASSERT_EQ(testDict.get(3, 0), 0);
}

TEST(MutableDictTestCase, TestSet)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, 5);
	ASSERT_EQ(testDict.get(3, 0), 5);
}

TEST(MutableDictTestCase, TestRemove)
{
	wasp::collections::Dict<int, int> testDict({
		{1, 2}
	}, true);
	ASSERT_EQ(testDict.get(1, 0), 2);
	testDict.remove(1);
	ASSERT_EQ(testDict.get(1, 0), 0);
}

TEST(MutableDictTestCase, TestClear)
{
	wasp::collections::Dict<int, int> testDict({
		{1, 2}, {3, 4}
	}, true);
	ASSERT_FALSE(testDict.is_empty());
	testDict.clear();
	ASSERT_TRUE(testDict.is_empty());
}

TEST(MutableDictTestCase, TestSize)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_EQ(testDict.size(), 0);
	testDict.set(1, 2);
	ASSERT_EQ(testDict.size(), 1);
	testDict.set(2, 5);
	ASSERT_EQ(testDict.size(), 2);
}

TEST(MutableDictTestCase, TestContains)
{
	wasp::collections::Dict<int, int> testDict({
		{1, 2}
	}, true);
	ASSERT_TRUE(testDict.contains(1));
	ASSERT_FALSE(testDict.contains(2));
}

TEST(MutableDictTestCase, TestIsMutable)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_TRUE(testDict.is_mutable());
}

TEST(MutableDictTestCase, TestIsEmpty)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_TRUE(testDict.is_empty());
}

TEST(MutableDictTestCase, TestIsNotEmpty)
{
	wasp::collections::Dict<int, int> testDict({{1, 2}}, true);
	ASSERT_FALSE(testDict.is_empty());
}

TEST(MutableDictTestCase, TestBeginNotThrows)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.begin());
}

TEST(MutableDictTestCase, TestEndNotThrows)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.end());
}

TEST(MutableDictTestCase, TestRBeginNotThrows)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rbegin());
}

TEST(MutableDictTestCase, TestREndNotThrows)
{
	wasp::collections::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rend());
}


// Immutable dict features testing.
TEST(ImmutableDictTestCase, TestSetThrows)
{
	wasp::collections::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.set(1, 2), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestRemoveThrows)
{
	wasp::collections::Dict<int, int> testDict({{1, 2}}, false);
	ASSERT_THROW(testDict.remove(1), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestIsImmutable)
{
	wasp::collections::Dict<int, int> testDict(false);
	ASSERT_FALSE(testDict.is_mutable());
}

TEST(ImmutableDictTestCase, TestBeginThrows)
{
	wasp::collections::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.begin(), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestEndThrows)
{
	wasp::collections::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.end(), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestRBeginThrows)
{
	wasp::collections::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.rbegin(), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestREndThrows)
{
	wasp::collections::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.rend(), wasp::DictError);
}

TEST(DictTestCase, TestGetKeys)
{
	wasp::collections::Dict<int, int> testDict({
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
	wasp::collections::Dict<int, int> testDict({
		{1, 2},
		{2, 3},
		{3, 4}
	});
	auto expected = 3;
	auto actual = testDict.size();

	ASSERT_EQ(expected, actual);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_COLLECTIONS_TESTS_DICT_TESTS_H
