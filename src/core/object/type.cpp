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

#include "./type.h"

// Wasp libraries.
#include "./object.h"


__OBJECT_BEGIN__

Type::Type(const Object& obj)
{
	auto full_name = str::rsplit(Type::type_name(obj), ':', 1);
	if (full_name.size() == 2)
	{
		str::ltrim(full_name.back(), ":");
		str::rtrim(full_name.front(), ":");
		this->_namespace = full_name.front();
	}

	this->_name = full_name.back();
}

std::string Type::name()
{
	return this->_name;
}

std::string Type::namespace_()
{
	return this->_namespace;
}

bool Type::operator==(const Type& other) const
{
	return this->_name == other._name;
}

std::ostream& operator<< (std::ostream &out, const Type& obj)
{
	out << "<class '" + obj._name + "'>";
	return out;
}

[[nodiscard]] std::string Type::type_name(const Object& obj)
{
	return Type::get_full_type_name(typeid(obj).name());
}

std::string Type::get_full_type_name(const std::string& type_name)
{
	int status;
	std::string name = type_name;
	char* full_name = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
	if (status == 0)
	{
		name = full_name;
	}

	return name;
}

__OBJECT_END__
