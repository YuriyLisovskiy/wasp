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

#ifndef WASP_UNIT_TESTS_COLLECTIONS_TESTS_MULTI_DICT_TESTS_H
#define WASP_UNIT_TESTS_COLLECTIONS_TESTS_MULTI_DICT_TESTS_H

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/collections/multi_dict.h"


__UNIT_TESTS_BEGIN__

template <typename _T>
bool vectorsAreEqual(const std::vector<_T>& left, const std::vector<_T>& right)
{
	bool result = true;
	if (left.size() != right.size())
	{
		result = false;
	}
	else
	{
		for (size_t i = 0; i < left.size(); i++)
		{
			if (left[i] != right[i])
			{
				result = false;
				break;
			}
		}
	}
	return result;
}

// Mutable MultiValueDict testing.
TEST(MutableMultiValueDictTestCase, TestGetItemExists)
{
	wasp::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}},
		{4, {5, 6, 7}},
		{7, {8, 9, 10}}
	}, true);
	ASSERT_EQ(testDict.get(1, 0), 4);
	ASSERT_EQ(testDict.get(4, 0), 7);
	ASSERT_EQ(testDict.get(7, 0), 10);
}

TEST(MutableMultiValueDictTestCase, TestGetItemNotExists)
{
	wasp::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}},
	}, true);
	ASSERT_EQ(testDict.get(3, 0), 0);
}

TEST(MutableMultiValueDictTestCase, TestSet)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, 5);
	ASSERT_EQ(testDict.get(3, 0), 5);
}

TEST(MutableMultiValueDictTestCase, TestSetVector)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, std::vector<int>{5, 6, 7});
	ASSERT_EQ(testDict.get(3), 7);
}

TEST(MutableMultiValueDictTestCase, TestAppendToNonExistent)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, {}), {});
	testDict.append(3, 5);
	ASSERT_EQ(testDict.get(3), 5);
	testDict.append(3, 7);
	ASSERT_EQ(testDict.get(3), 7);
	testDict.append(3, 100);
	ASSERT_EQ(testDict.get(3), 100);
}

TEST(MutableMultiValueDictTestCase, TestAppendToExistent)
{
	wasp::MultiValueDict<int, int> testDict({{1, {2, 3}}}, true);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3}));
	testDict.append(1, 5);
	ASSERT_EQ(testDict.get(1), 5);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3, 5}));
}

TEST(MutableMultiValueDictTestCase, TestAppendVectorToNonExistent)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, {}), {});
	testDict.append(3, {5, 6, 7});
	ASSERT_EQ(testDict.get(3), 7);
}

TEST(MutableMultiValueDictTestCase, TestAppendVectorToExistent)
{
	wasp::MultiValueDict<int, int> testDict({{1, {2, 3}}}, true);
	ASSERT_EQ(testDict.get(1), 3);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3}));
	testDict.append(1, {5, 6, 7});
	ASSERT_EQ(testDict.get(1), 7);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3, 5, 6, 7}));
}

TEST(MutableMultiValueDictTestCase, TestClear)
{
	wasp::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}}
	}, true);
	ASSERT_FALSE(testDict.isEmpty());
	testDict.clear();
	ASSERT_TRUE(testDict.isEmpty());
}

TEST(MutableMultiValueDictTestCase, TestSize)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.size(), 0);
	testDict.append(1, {2, 3, 4});
	ASSERT_EQ(testDict.size(), 1);
	testDict.append(2, {5, 6, 7});
	ASSERT_EQ(testDict.size(), 2);
}

TEST(MutableMultiValueDictTestCase, TestRemove)
{
	wasp::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}}
	}, true);
	ASSERT_EQ(testDict.get(1, 0), 4);
	testDict.remove(1);
	ASSERT_EQ(testDict.get(1, 0), 0);
}

TEST(MutableMultiValueDictTestCase, TestContains)
{
	wasp::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}}
	}, true);
	ASSERT_TRUE(testDict.contains(1));
	ASSERT_FALSE(testDict.contains(2));
}

TEST(MutableMultiValueDictTestCase, TestIsMutable)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_TRUE(testDict.isMutable());
}

TEST(MutableMultiValueDictTestCase, TestIsEmpty)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_TRUE(testDict.isEmpty());
}

TEST(MutableMultiValueDictTestCase, TestIsNotEmpty)
{
	wasp::MultiValueDict<int, int> testDict({{1, {2, 3, 4}}}, true);
	ASSERT_FALSE(testDict.isEmpty());
}

TEST(MutableMultiValueDictTestCase, TestBeginNotThrows)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.begin());
}

TEST(MutableMultiValueDictTestCase, TestEndNotThrows)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.end());
}

TEST(MutableMultiValueDictTestCase, TestRBeginNotThrows)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rbegin());
}

TEST(MutableMultiValueDictTestCase, TestREndNotThrows)
{
	wasp::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rend());
}


// Immutable MultiValueDict features testing.
TEST(ImmutableMultiValueDictTestCase, TestSetThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.set(1, 2), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestSetVectorThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.set(1, {2, 3, 4}), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestAppendThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.append(1, 4), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestAppendVectorThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.append(1, {2, 3, 4}), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestRemoveThrows)
{
	wasp::MultiValueDict<int, int> testDict({{1, {2, 3, 4}}}, false);
	ASSERT_THROW(testDict.remove(1), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestIsImmutable)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_FALSE(testDict.isMutable());
}

TEST(ImmutableMultiValueDictTestCase, TestBeginThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.begin(), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestEndThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.end(), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestRBeginThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.rbegin(), wasp::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestREndThrows)
{
	wasp::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.rend(), wasp::MultiValueDictError);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_COLLECTIONS_TESTS_MULTI_DICT_TESTS_H
