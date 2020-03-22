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

#ifndef WASP_UNIT_TESTS_RENDER_TESTS_INTERNAL_TESTS_UTILITY_TESTS_H
#define WASP_UNIT_TESTS_RENDER_TESTS_INTERNAL_TESTS_UTILITY_TESTS_H

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "../../_def_.h"
#include "../../../../src/render/internal/utility.h"


__UNIT_TESTS_BEGIN__

TEST(UtilityTestCase, TestSplitParams)
{
	using namespace wasp::render::internal;
	std::vector<std::string> expected = {
		"23", " hello | def(one=1, two='zzz')"
	};
	std::string params = "(23, hello | def(one=1, two='zzz'))";
	std::vector<std::string> actual;
	size_t pos;

	ASSERT_TRUE(split_params(params, pos, actual));
	ASSERT_EQ(expected.size(), actual.size());
	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST(UtilityTestCase, TestSplitParamsMissingLastBracket)
{
	using namespace wasp::render::internal;
	std::string params = "(23, hello | def(one=1, two='zzz')";
	std::vector<std::string> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, pos, actual));
	ASSERT_EQ(pos, params.size());
}

TEST(UtilityTestCase, TestSplitParamsMissingOpenBracket)
{
	using namespace wasp::render::internal;
	std::string params = "23, hello | def(one=1, two='zzz'))";
	std::vector<std::string> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, pos, actual));
	ASSERT_EQ(pos, 1);
}

TEST(UtilityTestCase, TestSplitParamsTooShortContent)
{
	using namespace wasp::render::internal;
	std::string params = "(";
	std::vector<std::string> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, pos, actual));
	ASSERT_EQ(pos, 0);
}

TEST(UtilityTestCase, TestSplitParamsRedundantComma)
{
	using namespace wasp::render::internal;
	std::string params = "(23, hello | def(one=1, two='zzz'),)";
	std::vector<std::string> actual;
	size_t pos;
	ASSERT_FALSE(split_params(params, pos, actual));
	ASSERT_EQ(pos, params.size() - 1);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_RENDER_TESTS_INTERNAL_TESTS_UTILITY_TESTS_H
