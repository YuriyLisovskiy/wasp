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
 * An implementation of core/object/object.h
 */

#include "./object.h"


__OBJECT_BEGIN__

Object::~Object()
{
	delete this->_object_type;
}

int Object::compare_to(const Object& other) const
{
	auto this_hash = this->get_hash_code();
	auto other_hash = other.get_hash_code();
	if (this_hash < other_hash)
	{
		return -1;
	}

	return this_hash == other_hash ? 0 : 1;
}

unsigned long Object::get_hash_code() const
{
	return reinterpret_cast<std::uintptr_t>(this);
}

[[nodiscard]] std::string Object::to_string()
{
	std::stringstream oss;
	oss << static_cast<const void*>(this);
	return "<" + this->get_type().name() + " object at " + oss.str() + ">";
}

Type Object::get_type()
{
	if (!this->_object_type)
	{
		this->_object_type = new Type(*this);
	}

	return *this->_object_type;
}

bool Object::operator<(const Object& other) const
{
	return this->compare_to(other) == -1;
}

bool Object::operator==(const Object& other) const
{
	return this->compare_to(other) == 0;
}

bool Object::operator>(const Object& other) const
{
	return this->compare_to(other) == 1;
}

bool Object::operator<=(const Object& other) const
{
	return *this < other || *this == other;
}

bool Object::operator>=(const Object& other) const
{
	return *this > other || *this == other;
}

std::ostream& operator<<(std::ostream& out, Object& obj)
{
	out << obj.to_string();
	return out;
}

__OBJECT_END__
