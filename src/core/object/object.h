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
 * core/object/object.h
 *
 * Purpose:
 * 	Main parent for all derived classes which need to be rendered
 * 	or for some other purposes.
 */

#pragma once

// C++ libraries.
#include <string>
#include <iostream>
#include <cxxabi.h>
#include <sstream>
#include <cstdint>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./type.h"


__OBJECT_BEGIN__

/// Represents an object with default methods.
///
/// 'compare_to' -> int;
/// 'get_hash_code' -> unsigned long
/// 'to_string' -> std::string;
/// 'get_type' -> Type.
class Object
{
private:
	Type* _object_type = nullptr;

public:
	virtual ~Object();

	/// Returns 0 if objects are equal, -1 if 'this' is less
	///  than 'other' otherwise returns 1.
	/// Can be overridden.
	[[nodiscard]] virtual int compare_to(const Object& other) const;

	/// Returns object's address as unsigned long integer.
	/// Hash code value will be different for different program executions.
	/// Overriding of this method is recommended for objects' comparison.
	[[nodiscard]] virtual unsigned long get_hash_code() const;

	/// Returns <'ObjectType' object at 'ObjectAddress'> by default.
	/// Can be overridden.
	[[nodiscard]] virtual std::string to_string();

	/// Returns Type object with object meta information.
	Type get_type();

	/// 'less than' operator.
	/// Uses Object::compareTo method.
	bool operator<(const Object& other) const;

	/// 'equals' operator.
	/// Uses Object::compareTo method.
	bool operator==(const Object& other) const;

	/// 'greater than' operator.
	/// Uses Object::compareTo method.
	bool operator>(const Object& other) const;

	/// 'less than or equals' operator.
	/// Uses 'less than' and 'equals' operators.
	bool operator<=(const Object& other) const;

	/// 'greater than or equals' operator.
	/// Uses 'greater than' and 'equals' operators.
	bool operator>=(const Object& other) const;

	/// 'left bitwise shift' operator for std::ostream.
	/// Uses Object::toString method.
	friend std::ostream& operator<< (std::ostream &out, Object& obj);
};

__OBJECT_END__
