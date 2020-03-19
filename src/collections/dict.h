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
 * collections/dict.h
 *
 * Purpose:
 * Template container based on std::map with additional
 * methods (extended std::map).
 */

#pragma once

// C++ libraries.
#include <map>
#include <vector>

// Module definitions.
#include "./_def_.h"


__COLLECTIONS_BEGIN__

template <typename _Key, typename _Val>
class Dict
{
protected:
	/// An actual container.
	std::map<_Key, _Val> _map;

public:
	typedef typename std::map<_Key, _Val>::iterator iterator;
	typedef typename std::map<_Key, _Val>::reverse_iterator reverse_iterator;

	/// Constructs empty dictionary.
	Dict()
	{
	}

	/// Constructs Dict instance from std::map.
	explicit Dict(const std::map<_Key, _Val>& src_map)
	{
		this->_map = src_map;
	}

	/// Returns value by key if it exists, otherwise returns _default.
	///
	/// @param key: dict key which holds some value.
	/// @param _default: a default value to be returned if key does not exist.
	/// @return value by given key.
	virtual _Val get(_Key key, _Val _default = _Val()) const
	{
		if (this->contains(key))
		{
			return this->_map.at(key);
		}

		return _default;
	}

	/// Sets new value by given key.
	///
	/// @param key: new key or existing key.
	/// @param value: new value.
	virtual void set(_Key key, _Val value)
	{
		this->_map[key] = value;
	}

	/// Removes value from dict by it's key.
	///
	/// @param key: key to be removed.
	virtual void remove(_Key key)
	{
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
	virtual bool contains(_Key key) const
	{
		return this->_map.find(key) != this->_map.end();
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
		for (auto it = this->begin(); it != this->end(); it++)
		{
			keys.push_back(it->first);
		}

		return keys;
	}

	/// Returns begin iterator.
	iterator begin()
	{
		return this->_map.begin();
	}

	/// Returns end iterator.
	iterator end()
	{
		return this->_map.end();
	}

	/// Returns reversed begin iterator.
	reverse_iterator rbegin()
	{
		return this->_map.rbegin();
	}

	/// Returns reversed end iterator.
	reverse_iterator rend()
	{
		return this->_map.rend();
	}

	_Val& operator[] (const _Key& key)
	{
		return this->_map[key];
	}
};

__COLLECTIONS_END__
