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

#ifndef WASP_UNIT_TESTS_UTILS_H
#define WASP_UNIT_TESTS_UTILS_H

#include <vector>

#include "./_def_.h"


__TEST_UTILS_BEGIN__

template <typename _T>
bool assert_vector(const std::vector<_T>& actual, const std::vector<_T>& expected)
{
	if (actual.size() != expected.size())
	{
		return false;
	}

	for (size_t i = 0; i < actual.size(); i++)
	{
		if (actual[i] != expected[i])
		{
			return false;
		}
	}

	return true;
}

__TEST_UTILS_END__


#endif // WASP_UNIT_TESTS_UTILS_H
