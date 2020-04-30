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
#include <utility>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./type.h"


__OBJECT_BEGIN__

struct Attr
{
private:
	typedef std::function<Object*()> attr_getter;
	typedef std::function<void(Object*)> attr_setter;

	attr_getter _getter = nullptr;
	attr_setter _setter = nullptr;

public:
	Attr() = default;

	explicit Attr(attr_getter getter) : _getter(std::move(getter))
	{
	}

	explicit Attr(attr_setter setter) : _setter(std::move(setter))
	{
	}

	Attr(
		attr_getter getter, attr_setter setter
	) : _getter(std::move(getter)), _setter(std::move(setter))
	{
	}

	Object* get()
	{
		if (this->_getter)
		{
			return this->_getter();
		}

		return nullptr;
	}

	[[nodiscard]] Object* get() const
	{
		if (this->_getter)
		{
			return this->_getter();
		}

		return nullptr;
	}

	void set(Object* val_ptr)
	{
		if (this->_setter)
		{
			this->_setter(val_ptr);
		}
	}
};

class Object
{
private:
	std::string _object_address;

protected:
	[[nodiscard]] std::string __address__() const;

public:
	std::map<std::string, Attr> __attrs__;

	Object();
	virtual ~Object() = default;

	virtual Object* __get_attr__(const char* attr_name) const;
	virtual void __set_attr__(const char* attr_name, Object* ptr);
	bool __has_attr__(const char* attr_name) const;

	/// Returns 0 if objects are equal, -1 if 'this' is less
	///  than 'other' otherwise returns 1.
	/// Can be overridden.
	virtual short __cmp__(const Object* other) const;

	[[nodiscard]] virtual unsigned long __hash__() const;

	[[nodiscard]] Type __type__() const;

	template <typename _CastT>
	_CastT __cast__() const
	{
		if constexpr (std::is_pointer<_CastT>::value)
		{
			return ((_CastT)this);
		}

		return *((_CastT*)this);
	}

	[[nodiscard]] virtual std::string __str__() const;
	[[nodiscard]] virtual std::string __repr__() const;

	bool operator<(const Object& other) const;
	bool operator==(const Object& other) const;
	bool operator!=(const Object& other) const;
	bool operator>(const Object& other) const;
	bool operator<=(const Object& other) const;
	bool operator>=(const Object& other) const;

	friend std::ostream& operator<<(std::ostream& out, Object& obj);

	explicit virtual operator bool () const;
};

__OBJECT_END__
