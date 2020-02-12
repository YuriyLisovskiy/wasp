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
 * core/types/value.h
 *
 * Purpose:
 * Wrapper for fundamental types and classes which inherit Object.
 *
 * Provides arithmetic operations for fundamentals and some helper
 * methods.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../object/object.h"


__TYPES_BEGIN__

template <typename _T>
class Value : public object::Object
{
protected:
	_T _internal_value;

public:
	Value() = default;

	~Value() override = default;

	explicit Value(_T val) : Value()
	{
		this->_internal_value = std::move(val);
	}

	[[nodiscard]] std::string __str__() const override
	{
		if constexpr (std::is_same_v<_T, std::string>)
		{
			return this->_internal_value;
		}
		else if constexpr (std::is_fundamental<_T>::value)
		{
			return std::to_string(this->_internal_value);
		}
		else if constexpr (std::is_base_of<Object, _T>::value)
		{
			return this->_internal_value.__str__();
		}

		return "<" + this->__type__().name() + " object at " + this->__address__() + ">";
	}

	Value<_T> operator+(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value + right._internal_value);
	}

	Value<_T> operator-(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value - right._internal_value);
	}

	Value<_T> operator*(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value * right._internal_value);
	}

	Value<_T> operator/(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value / right._internal_value);
	}

	Value<_T> operator%(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value % right._internal_value);
	}

	Value<_T> operator&(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value & right._internal_value);
	}

	Value<_T> operator|(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value | right._internal_value);
	}

	Value<_T> operator~()
	{
		return Value<_T>(~this->_internal_value);
	}

	Value<_T> operator^(const Value<_T>& right)
	{
		return Value<_T>(this->_internal_value ^ right._internal_value);
	}

	_T& operator*()
	{
		return this->_internal_value;
	}
};

__TYPES_END__
