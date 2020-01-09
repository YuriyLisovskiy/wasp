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
 * multi_dict.h
 * Purpose: template container based on collections::Dict with additional
 * 			methods (extended collections::Dict). Main difference is that
 * 			MultiValueDict can contain multiple values in one key.
 */

#pragma once

// C++ libraries.
#include <vector>
#include <initializer_list>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./dict.h"
#include "../core/exceptions.h"


__COLLECTIONS_BEGIN__

template <typename _Key, typename _Val>
class MultiValueDict : public Dict<_Key, std::vector<_Val>>
{
protected:

	/// Wrapper for throwing MultiValueDictError exception.
	void _throw(const std::string& msg, int line, const char* function, const char* file) override
	{
		throw core::MultiValueDictError(
			std::string("unable to ") + msg + std::string(", MultiValueDict instance is immutable"),
			line, function, file
		);
	}

public:

	/// Constructs immutable empty multi-value dictionary.
	MultiValueDict() : Dict<_Key, std::vector<_Val>>()
	{
	}

	/// Constructs empty multi-value dictionary with mutability parameter.
	explicit MultiValueDict(bool is_mutable) : Dict<_Key, std::vector<_Val>>(is_mutable)
	{
	}

	/// Constructs MultiValueDict instance from std::map.
	/// Makes multi-value dictionary immutable by default.
	explicit MultiValueDict(const std::map<_Key, std::vector<_Val>>& src_map)
		: Dict<_Key, std::vector<_Val>>(src_map, false)
	{
	}

	/// Constructs MultiValueDict instance from std::map with mutability parameter.
	MultiValueDict(const std::map<_Key, std::vector<_Val>>& srcMap, bool is_mutable)
		: Dict<_Key, std::vector<_Val>>(srcMap, is_mutable)
	{
	}

	/// Returns first value from vector by key if it exists, otherwise returns _default.
	///
	/// @param key: dict key which holds std::vector of values.
	/// @param _default: a default value to be returned if key does not exist.
	/// @return first value of std::vector by given key.
	_Val get(_Key key, _Val _default = _Val())
	{
		if (this->contains(key))
		{
			std::vector<_Val> vec = this->_map[key];
			if (vec.size() > 0)
			{
				return vec.at(vec.size() - 1);
			}
		}

		return _default;
	}

	/// Returns vector of values by key if it exists, otherwise returns _default vector.
	///
	/// @param key: dict key which holds std::vector of values.
	/// @param _default: a default vector of values to be returned if key does not exist.
	/// @return std::vector of values by given key.
	std::vector<_Val> get(_Key key, std::vector<_Val> _default) override
	{
		if (this->contains(key))
		{
			return this->_map[key];
		}

		return _default;
	}

	/// Sets new vector of values by given key if instance is mutable.
	///
	/// @param key: new key or existing key.
	/// @param value: new value to construct std::vector.
	void set(_Key key, _Val value)
	{
		if (!this->_is_mutable)
		{
			this->_throw("set new value", _ERROR_DETAILS_);
		}

		this->_map[key] = std::vector<_Val>{value};
	}

	/// Sets new vector of values by given key if instance is mutable.
	///
	/// @param key: new key or existing key.
	/// @param value: new std::vector.
	void set(_Key key, std::vector<_Val> value) override
	{
		if (!this->_is_mutable)
		{
			this->_throw("set new value", _ERROR_DETAILS_);
		}

		this->_map[key] = value;
	}

	/// Appends new value by given key if instance is mutable.
	///	If key does not exist, creates new key-value pair.
	///
	/// @param key: new key or existing key.
	/// @param value: new value to append to std::vector.
	void append(_Key key, _Val val)
	{
		if (!this->_is_mutable)
		{
			this->_throw("append new value", _ERROR_DETAILS_);
		}

		if (this->contains(key))
		{
			this->_map[key].push_back(val);
		}
		else
		{
			this->set(key, val);
		}
	}

	/// Appends new vector of values by given key if instance is mutable.
	///	If key does not exist, creates new key-value pair.
	///
	/// @param key: new key or existing key.
	/// @param value: new std::vector of values to append to std::vector.
	void append(_Key key, std::vector<_Val> vec)
	{
		if (!this->_is_mutable)
		{
			this->_throw("append new value", _ERROR_DETAILS_);
		}

		if (this->contains(key))
		{
			for (const auto& item : vec)
			{
				this->_map[key].push_back(item);
			}
		}
		else
		{
			this->set(key, vec);
		}
	}
};

__COLLECTIONS_END__
