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
 * core/types/utility.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// C++ libraries.
#include <type_traits>

// Module definitions.
#include "./_def_.h"


__TYPES_BEGIN__

template <typename Base, typename Derived>
constexpr bool is_base_of()
{
	if constexpr (std::is_pointer<Base>::value && std::is_pointer<Derived>::value)
	{
		using T1 = typename std::remove_pointer<Base>::type;
		using T2 = typename std::remove_pointer<Derived>::type;
		return std::is_base_of<T1, T2>::value;
	}

	if constexpr (std::is_pointer<Base>::value )
	{
		using T1 = typename std::remove_pointer<Base>::type;
		return std::is_base_of<T1, Derived>::value;
	}

	if constexpr (std::is_pointer<Derived>::value)
	{
		using T2 = typename std::remove_pointer<Derived>::type;
		return std::is_base_of<Base, T2>::value;
	}

	return std::is_base_of<Base, Derived>::value;
}

template <typename Derived, typename Base>
constexpr bool is_base_of(const Base&)
{
	return is_base_of<Base, Derived>();
}

template <typename LeftT, typename RightT>
constexpr bool is_instance(const RightT& value)
{
	if constexpr (std::is_pointer<RightT>::value)
	{
		using T = typename std::remove_pointer<RightT>::type;
		return std::is_same<LeftT, T>::value || std::is_base_of<LeftT, T>::value;
	}

	return std::is_same<LeftT, RightT>::value || std::is_base_of<LeftT, RightT>::value;
}

__TYPES_END__
