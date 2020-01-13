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

/**
 * utils.h
 * Purpose: core utilities.
 */

#pragma once

// C++ libraries.
#include <vector>

// Module definitions.
#include "./_def_.h"


__UTILITY_BEGIN__

template <typename _ItemT>
extern bool contains(const _ItemT& to_check, const std::vector<_ItemT>& items)
{
	for (const auto& item : items)
	{
		if (item == to_check)
		{
			return true;
		}
	}

	return false;
}

__UTILITY_END__
