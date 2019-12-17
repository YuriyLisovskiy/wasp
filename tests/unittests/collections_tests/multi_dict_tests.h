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

#include "../_def_.h"
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
	collections::MultiValueDict<int, int> testDict({
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
	collections::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}},
	}, true);
	ASSERT_EQ(testDict.get(3, 0), 0);
}

TEST(MutableMultiValueDictTestCase, TestSet)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, 5);
	ASSERT_EQ(testDict.get(3, 0), 5);
}

TEST(MutableMultiValueDictTestCase, TestSetVector)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, std::vector<int>{5, 6, 7});
	ASSERT_EQ(testDict.get(3), 7);
}

TEST(MutableMultiValueDictTestCase, TestAppendToNonExistent)
{
	collections::MultiValueDict<int, int> testDict(true);
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
	collections::MultiValueDict<int, int> testDict({{1, {2, 3}}}, true);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3}));
	testDict.append(1, 5);
	ASSERT_EQ(testDict.get(1), 5);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3, 5}));
}

TEST(MutableMultiValueDictTestCase, TestAppendVectorToNonExistent)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.get(3, {}), {});
	testDict.append(3, {5, 6, 7});
	ASSERT_EQ(testDict.get(3), 7);
}

TEST(MutableMultiValueDictTestCase, TestAppendVectorToExistent)
{
	collections::MultiValueDict<int, int> testDict({{1, {2, 3}}}, true);
	ASSERT_EQ(testDict.get(1), 3);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3}));
	testDict.append(1, {5, 6, 7});
	ASSERT_EQ(testDict.get(1), 7);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3, 5, 6, 7}));
}

TEST(MutableMultiValueDictTestCase, TestClear)
{
	collections::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}}
	}, true);
	ASSERT_FALSE(testDict.is_empty());
	testDict.clear();
	ASSERT_TRUE(testDict.is_empty());
}

TEST(MutableMultiValueDictTestCase, TestSize)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_EQ(testDict.size(), 0);
	testDict.append(1, {2, 3, 4});
	ASSERT_EQ(testDict.size(), 1);
	testDict.append(2, {5, 6, 7});
	ASSERT_EQ(testDict.size(), 2);
}

TEST(MutableMultiValueDictTestCase, TestRemove)
{
	collections::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}}
	}, true);
	ASSERT_EQ(testDict.get(1, 0), 4);
	testDict.remove(1);
	ASSERT_EQ(testDict.get(1, 0), 0);
}

TEST(MutableMultiValueDictTestCase, TestContains)
{
	collections::MultiValueDict<int, int> testDict({
		{1, {2, 3, 4}}
	}, true);
	ASSERT_TRUE(testDict.contains(1));
	ASSERT_FALSE(testDict.contains(2));
}

TEST(MutableMultiValueDictTestCase, TestIsMutable)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_TRUE(testDict.is_mutable());
}

TEST(MutableMultiValueDictTestCase, TestIsEmpty)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_TRUE(testDict.is_empty());
}

TEST(MutableMultiValueDictTestCase, TestIsNotEmpty)
{
	collections::MultiValueDict<int, int> testDict({{1, {2, 3, 4}}}, true);
	ASSERT_FALSE(testDict.is_empty());
}

TEST(MutableMultiValueDictTestCase, TestBeginNotThrows)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.begin());
}

TEST(MutableMultiValueDictTestCase, TestEndNotThrows)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.end());
}

TEST(MutableMultiValueDictTestCase, TestRBeginNotThrows)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rbegin());
}

TEST(MutableMultiValueDictTestCase, TestREndNotThrows)
{
	collections::MultiValueDict<int, int> testDict(true);
	ASSERT_NO_THROW(testDict.rend());
}


// Immutable MultiValueDict features testing.
TEST(ImmutableMultiValueDictTestCase, TestSetThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.set(1, 2), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestSetVectorThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.set(1, {2, 3, 4}), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestAppendThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.append(1, 4), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestAppendVectorThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.append(1, {2, 3, 4}), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestRemoveThrows)
{
	collections::MultiValueDict<int, int> testDict({{1, {2, 3, 4}}}, false);
	ASSERT_THROW(testDict.remove(1), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestIsImmutable)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_FALSE(testDict.is_mutable());
}

TEST(ImmutableMultiValueDictTestCase, TestBeginThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.begin(), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestEndThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.end(), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestRBeginThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.rbegin(), core::MultiValueDictError);
}

TEST(ImmutableMultiValueDictTestCase, TestREndThrows)
{
	collections::MultiValueDict<int, int> testDict(false);
	ASSERT_THROW(testDict.rend(), core::MultiValueDictError);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_COLLECTIONS_TESTS_MULTI_DICT_TESTS_H
