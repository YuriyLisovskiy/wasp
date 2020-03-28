/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * An implementation of core/utility.h
 */

// C++ libraries.
#include <cxxabi.h>
#include <memory>

// Header.
#include "./utility.h"

__UTILITY_BEGIN__

// Statuses:
// 0 - operation succeeded
// 1 - a memory allocation failure occurred
// 2 - mangled_name is not a valid name under the C++ ABI mangling rules
// 3 - one of the arguments is invalid
std::string demangle(const char* name)
{
	int status = -4;
	std::unique_ptr<char, void(*)(void*)> res {
		abi::__cxa_demangle(name, nullptr, nullptr, &status),
		std::free
	};

	return status == 0 ? res.get() : name;
}

__UTILITY_END__
