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

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "../../../../src/render/internal/utility.h"

using namespace wasp;


TEST(UtilityTestCase, TestSplitParams)
{
	using namespace wasp::render::internal;
	token_t first{};
	first.content = "23";

	token_t second{};
	second.content = "hello | def(one=1, two='zzz')";

	std::vector<token_t> expected = {first, second};
	std::string params = "(23, hello | def(one=1, two='zzz'))";
	std::vector<token_t> actual;
	size_t pos;

	ASSERT_TRUE(split_params(params, 1, pos, actual));
	ASSERT_EQ(expected.size(), actual.size());
	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i].content, actual[i].content);
	}
}

TEST(UtilityTestCase, TestSplitParamsMissingLastBracket)
{
	using namespace wasp::render::internal;
	std::string params = "(23, hello | def(one=1, two='zzz')";
	std::vector<token_t> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, 1, pos, actual));
	ASSERT_EQ(pos, params.size());
}

TEST(UtilityTestCase, TestSplitParamsMissingOpenBracket)
{
	using namespace wasp::render::internal;
	std::string params = "23, hello | def)";
	std::vector<token_t> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, 1, pos, actual));
	ASSERT_EQ(pos, 0);
}

TEST(UtilityTestCase, TestSplitParamsTooShortContent)
{
	using namespace wasp::render::internal;
	std::string params = "(";
	std::vector<token_t> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, 1, pos, actual));
	ASSERT_EQ(pos, 0);
}

TEST(UtilityTestCase, TestSplitParamsRedundantComma)
{
	using namespace wasp::render::internal;
	std::string params = "(23, hello | def(one=1, two='zzz'),)";
	std::vector<token_t> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, 1, pos, actual));
	ASSERT_EQ(pos, params.size());
}
