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
 * dict.h
 * Purpose: template container based on std::map with additional
 * 			methods (extended std::map).
 */

#ifndef WASP_COLLECTIONS_DICT_H
#define WASP_COLLECTIONS_DICT_H

// C++ libraries.
#include <map>
#include <vector>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"


__COLLECTIONS_BEGIN__

template <typename _Key, typename _Val>
class Dict
{
protected:

	/// Identifies if dictionary is mutable or not.
	bool _is_mutable;

	/// An actual container.
	std::map<_Key, _Val> _map;

	/// Wrapper for throwing DictError exception.
	virtual void _throw(const std::string& msg, int line, const char* function, const char* file)
	{
		throw core::DictError(
			std::string("unable to ") + msg + std::string(", Dict instance is immutable"),
			line, function, file
		);
	}

public:
	typedef typename std::map<_Key, _Val>::const_iterator const_iterator;
	typedef typename std::map<_Key, _Val>::const_reverse_iterator const_reverse_iterator;

	typedef typename std::map<_Key, _Val>::iterator iterator;
	typedef typename std::map<_Key, _Val>::reverse_iterator reverse_iterator;

	/// Constructs immutable empty dictionary.
	Dict() : _is_mutable(false)
	{
	}

	/// Constructs empty dictionary with mutability parameter.
	explicit Dict(bool is_mutable) : _is_mutable(is_mutable)
	{
	}

	/// Constructs Dict instance from std::map.
	/// Makes dictionary immutable by default.
	explicit Dict(const std::map<_Key, _Val>& src_map) : _is_mutable(false)
	{
		this->_map = src_map;
	}

	/// Constructs Dict instance from std::map with mutability parameter.
	Dict(const std::map<_Key, _Val>& src_map, bool is_mutable) : _is_mutable(is_mutable)
	{
		this->_map = src_map;
	}

	/// Returns value by key if it exists, otherwise returns _default.
	///
	/// @param key: dict key which holds some value.
	/// @param _default: a default value to be returned if key does not exist.
	/// @return value by given key.
	virtual _Val get(_Key key, _Val _default = _Val())
	{
		if (this->contains(key))
		{
			return this->_map[key];
		}

		return _default;
	}

	/// Sets new value by given key if instance is mutable.
	///
	/// @param key: new key or existing key.
	/// @param value: new value.
	virtual void set(_Key key, _Val value)
	{
		if (!this->_is_mutable)
		{
			this->_throw("set new value", _ERROR_DETAILS_);
		}

		this->_map[key] = value;
	}

	/// Removes value from dict by it's key.
	///
	/// @param key: key to be removed.
	virtual void remove(_Key key)
	{
		if (!this->_is_mutable)
		{
			this->_throw("remove value", _ERROR_DETAILS_);
		}

		if (this->contains(key))
		{
			this->_map.erase(key);
		}
	}

	/// Removes all values from dictionary.
	void clear()
	{
		this->_map.clear();
	}

	/// Returns dictionary's current size.
	size_t size()
	{
		return this->_map.size();
	}

	/// Checks if dictionary contains value by given key.
	///
	/// @param key: key to check.
	/// @return true if value exists, otherwise returns false.
	virtual bool contains(_Key key)
	{
		return this->_map.find(key) != this->_map.end();
	}

	/// Checks if dictionary is mutable.
	///
	/// @return true if dictionary can be changed, otherwise returns false.
	bool is_mutable()
	{
		return this->_is_mutable;
	}

	/// Checks if dictionary is empty.
	///
	/// @return true if dict is empty, otherwise returns false.
	bool is_empty()
	{
		return this->_map.empty();
	}

	/// Returns all keys which dictionary contains.
	///
	/// @return std::vector of keys.
	virtual std::vector<_Key> keys()
	{
		std::vector<_Key> keys;
		for (auto it = this->cbegin(); it != this->cend(); it++)
		{
			keys.push_back(it->first);
		}

		return keys;
	}

	/// Returns constant begin iterator.
	const_iterator cbegin()
	{
		return this->_map.cbegin();
	}

	/// Returns constant end iterator.
	const_iterator cend()
	{
		return this->_map.cend();
	}

	/// Returns constant reversed begin iterator.
	const_reverse_iterator crbegin()
	{
		return this->_map.crbegin();
	}

	/// Returns constant reversed end iterator.
	const_reverse_iterator crend()
	{
		return this->_map.crend();
	}

	/// Returns begin iterator.
	///	Works only if dictionary is mutable, otherwise
	///		an exception will be thrown.
	iterator begin()
	{
		if (!this->_is_mutable)
		{
			this->_throw("retrieve non-constant iterator", _ERROR_DETAILS_);
		}

		return this->_map.begin();
	}

	/// Returns end iterator.
	///	Works only if dictionary is mutable, otherwise
	///		an exception will be thrown.
	iterator end()
	{
		if (!this->_is_mutable)
		{
			this->_throw("retrieve non-constant iterator", _ERROR_DETAILS_);
		}
		return this->_map.end();
	}

	/// Returns reversed begin iterator.
	///	Works only if dictionary is mutable, otherwise
	///		an exception will be thrown.
	reverse_iterator rbegin()
	{
		if (!this->_is_mutable)
		{
			this->_throw("retrieve non-constant reverse iterator", _ERROR_DETAILS_);
		}

		return this->_map.rbegin();
	}

	/// Returns reversed end iterator.
	///	Works only if dictionary is mutable, otherwise
	///		an exception will be thrown.
	reverse_iterator rend()
	{
		if (!this->_is_mutable)
		{
			this->_throw("retrieve non-constant reverse iterator", _ERROR_DETAILS_);
		}

		return this->_map.rend();
	}
};

__COLLECTIONS_END__


#endif // WASP_COLLECTIONS_DICT_H
