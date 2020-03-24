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

/**
 * utils/functional.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// C++ libraries.
#include <vector>
#include <functional>

// Module definitions.
#include "./_def_.h"


__FUNCTIONAL_BEGIN__

template <typename _InputT, typename _ReturnT>
extern std::vector<_ReturnT> map(
	const std::vector<_InputT>& input,
	std::function<_ReturnT(const _InputT&)> apply
)
{
	std::vector<_ReturnT> result;
	for (const auto& item : input)
	{
		result.push_back(apply(item));
	}

	return result;
}

__FUNCTIONAL_END__
