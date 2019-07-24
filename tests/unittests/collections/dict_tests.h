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

/*
 * dict_tests
 * TODO: write docs.
 */

#ifndef WASP_DICT_TESTS_H
#define WASP_DICT_TESTS_H

#include <gtest/gtest.h>

#include "../../../src/collections/dict.h"


// Mutable dict testing.
TEST(MutableDictTestCase, TestGetItemExists)
{
	wasp::Dict<int, int> testDict({
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
	wasp::Dict<int, int> testDict({
		{1, 2}
	}, true);
	ASSERT_EQ(testDict.get(3, 0), 0);
}

TEST(MutableDictTestCase, TestSet)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, 5);
	ASSERT_EQ(testDict.get(3, 0), 5);
}

TEST(MutableDictTestCase, TestRemove)
{
	wasp::Dict<int, int> testDict({
		{1, 2}
	}, true);
	ASSERT_EQ(testDict.get(1, 0), 2);
	testDict.remove(1);
	ASSERT_EQ(testDict.get(1, 0), 0);
}

TEST(MutableDictTestCase, TestContains)
{
	wasp::Dict<int, int> testDict({
		{1, 2}
	}, true);
	ASSERT_TRUE(testDict.contains(1));
	ASSERT_FALSE(testDict.contains(2));
}

TEST(MutableDictTestCase, TestIsMutable)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_TRUE(testDict.isMutable());
}

TEST(MutableDictTestCase, TestIsEmpty)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_TRUE(testDict.isEmpty());
}

TEST(MutableDictTestCase, TestIsNotEmpty)
{
	wasp::Dict<int, int> testDict({{1, 2}}, true);
	ASSERT_FALSE(testDict.isEmpty());
}

TEST(MutableDictTestCase, TestBeginNotThrows)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.begin());
}

TEST(MutableDictTestCase, TestEndNotThrows)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.end());
}

TEST(MutableDictTestCase, TestRBeginNotThrows)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rbegin());
}

TEST(MutableDictTestCase, TestREndNotThrows)
{
	wasp::Dict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rend());
}


// Immutable dict features testing.
TEST(ImmutableDictTestCase, TestSetThrows)
{
	wasp::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.set(1, 2), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestRemoveThrows)
{
	wasp::Dict<int, int> testDict({{1, 2}}, false);
	ASSERT_THROW(testDict.remove(1), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestIsImmutable)
{
	wasp::Dict<int, int> testDict(false);
	ASSERT_FALSE(testDict.isMutable());
}

TEST(ImmutableDictTestCase, TestBeginThrows)
{
	wasp::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.begin(), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestEndThrows)
{
	wasp::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.end(), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestRBeginThrows)
{
	wasp::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.rbegin(), wasp::DictError);
}

TEST(ImmutableDictTestCase, TestREndThrows)
{
	wasp::Dict<int, int> testDict(false);
	ASSERT_THROW(testDict.rend(), wasp::DictError);
}

#endif // WASP_DICT_TESTS_H
