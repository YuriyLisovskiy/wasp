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

#include <map>

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

// MultiValueDict testing.
TEST(MultiValueDictTestCase, TestGetItemExists)
{
	collections::MultiValueDict<int, int> testDict(std::map<int, std::vector<int>>{
		{1, {2, 3, 4}},
		{4, {5, 6, 7}},
		{7, {8, 9, 10}}
	});
	ASSERT_EQ(testDict.get(1, 0), 4);
	ASSERT_EQ(testDict.get(4, 0), 7);
	ASSERT_EQ(testDict.get(7, 0), 10);
}

TEST(MultiValueDictTestCase, TestGetItemNotExists)
{
	collections::MultiValueDict<int, int> testDict(std::map<int, std::vector<int>>{
		{1, {2, 3, 4}},
	});
	ASSERT_EQ(testDict.get(3, 0), 0);
}

TEST(MultiValueDictTestCase, TestSet)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, 5);
	ASSERT_EQ(testDict.get(3, 0), 5);
}

TEST(MultiValueDictTestCase, TestSetVector)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_EQ(testDict.get(3, 0), 0);
	testDict.set(3, std::vector<int>{5, 6, 7});
	ASSERT_EQ(testDict.get(3), 7);
}

TEST(MultiValueDictTestCase, TestAppendToNonExistent)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_EQ(testDict.get(3, {}), {});
	testDict.append(3, 5);
	ASSERT_EQ(testDict.get(3), 5);
	testDict.append(3, 7);
	ASSERT_EQ(testDict.get(3), 7);
	testDict.append(3, 100);
	ASSERT_EQ(testDict.get(3), 100);
}

TEST(MultiValueDictTestCase, TestAppendToExistent)
{
	collections::MultiValueDict<int, int> testDict(std::map<int, std::vector<int>>{
		{1, {2, 3}}
	});
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3}));
	testDict.append(1, 5);
	ASSERT_EQ(testDict.get(1), 5);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3, 5}));
}

TEST(MultiValueDictTestCase, TestAppendVectorToNonExistent)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_EQ(testDict.get(3, {}), {});
	testDict.append(3, {5, 6, 7});
	ASSERT_EQ(testDict.get(3), 7);
}

TEST(MultiValueDictTestCase, TestAppendVectorToExistent)
{
	collections::MultiValueDict<int, int> testDict(
		std::map<int, std::vector<int>>{{1, {2, 3}}}
	);
	ASSERT_EQ(testDict.get(1), 3);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3}));
	testDict.append(1, {5, 6, 7});
	ASSERT_EQ(testDict.get(1), 7);
	ASSERT_TRUE(vectorsAreEqual(testDict.get(1, std::vector<int>{}), std::vector<int>{2, 3, 5, 6, 7}));
}

TEST(MultiValueDictTestCase, TestClear)
{
	collections::MultiValueDict<int, int> testDict(
		std::map<int, std::vector<int>>{{1, {2, 3, 4}}}
	);
	ASSERT_FALSE(testDict.is_empty());
	testDict.clear();
	ASSERT_TRUE(testDict.is_empty());
}

TEST(MultiValueDictTestCase, TestSize)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_EQ(testDict.size(), 0);
	testDict.append(1, {2, 3, 4});
	ASSERT_EQ(testDict.size(), 1);
	testDict.append(2, {5, 6, 7});
	ASSERT_EQ(testDict.size(), 2);
}

TEST(MultiValueDictTestCase, TestRemove)
{
	collections::MultiValueDict<int, int> testDict(
		std::map<int, std::vector<int>>{{1, {2, 3, 4}}}
	);
	ASSERT_EQ(testDict.get(1, 0), 4);
	testDict.remove(1);
	ASSERT_EQ(testDict.get(1, 0), 0);
}

TEST(MultiValueDictTestCase, TestContains)
{
	collections::MultiValueDict<int, int> testDict(
		std::map<int, std::vector<int>>{{1, {2, 3, 4}}}
	);
	ASSERT_TRUE(testDict.contains(1));
	ASSERT_FALSE(testDict.contains(2));
}

TEST(MultiValueDictTestCase, TestIsEmpty)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_TRUE(testDict.is_empty());
}

TEST(MultiValueDictTestCase, TestIsNotEmpty)
{
	collections::MultiValueDict<int, int> testDict(
		std::map<int, std::vector<int>>{{1, {2, 3, 4}}}
	);
	ASSERT_FALSE(testDict.is_empty());
}

TEST(MultiValueDictTestCase, TestBeginNotThrows)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_NO_THROW(testDict.begin());
}

TEST(MultiValueDictTestCase, TestEndNotThrows)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_NO_THROW(testDict.end());
}

TEST(MultiValueDictTestCase, TestRBeginNotThrows)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_NO_THROW(testDict.rbegin());
}

TEST(MultiValueDictTestCase, TestREndNotThrows)
{
	collections::MultiValueDict<int, int> testDict;
	ASSERT_NO_THROW(testDict.rend());
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_COLLECTIONS_TESTS_MULTI_DICT_TESTS_H
