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
 * core/object/type.h
 *
 * Purpose: represents basic metadata of class inherited from Object.
 */

#pragma once

// C++ libraries.
#include <string>
#include <cxxabi.h>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../string/str.h"
#include "../utility.h"


__OBJECT_BEGIN__

class Object;

class Type final
{
private:

	friend class Object;

	std::string _name;
	std::string _namespace;
	std::vector<std::string> _attrs;

	explicit Type(const Object& obj);

public:
	[[nodiscard]] std::string name() const;

	[[nodiscard]] std::string namespace_() const;

	[[nodiscard]] std::vector<std::string> attributes() const;

	[[nodiscard]] static std::string type_name(const Object& obj);

	template<typename _T>
	[[nodiscard]] static std::string type_name()
	{
		return utility::demangle(typeid(_T).name());
	}

	template<typename _T>
	[[nodiscard]] static std::string type()
	{
		std::string pre = std::is_fundamental<_T>::value ? "built-in type" : "class";
		return "<" + pre + " '" + Type::type_name<_T>() + "'>";
	}

	bool operator==(const Type& other) const;

	friend std::ostream& operator<<(std::ostream& out, const Type& obj);
};

__OBJECT_END__
