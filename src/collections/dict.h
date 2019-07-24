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

/*
 * QuerySet definition.
 * TODO: write docs
 */

#ifndef WASP_UTILS_QUERY_DICT_H
#define WASP_UTILS_QUERY_DICT_H

#include <map>

#include "../globals.h"
#include "../core/exceptions.h"


__WASP_BEGIN__

template <typename _Key, typename _Val>
class Dict
{
protected:
	bool _isMutable;
	std::map<_Key, _Val> _map;

	void _throw(const std::string& msg, int line, const char* function, const char* file)
	{
		throw DictError(
			std::string("unable to ") + msg + std::string(", Dict instance is immutable"), line, function, file
		);
	}

public:
	typedef typename std::map<_Key, _Val>::const_iterator const_iterator;
	typedef typename std::map<_Key, _Val>::const_reverse_iterator const_reverse_iterator;

	typedef typename std::map<_Key, _Val>::iterator iterator;
	typedef typename std::map<_Key, _Val>::reverse_iterator reverse_iterator;

	Dict() : _isMutable(false)
	{
	}

	explicit Dict(bool isMutable) : _isMutable(isMutable)
	{
	}

	explicit Dict(const std::map<_Key, _Val>& srcMap) : _isMutable(false)
	{
		this->_map = srcMap;
	}

	Dict(const std::map<_Key, _Val>& srcMap, bool isMutable) : _isMutable(isMutable)
	{
		this->_map = srcMap;
	}

	virtual _Val get(_Key key, _Val _default)
	{
		if (this->contains(key))
		{
			return this->_map[key];
		}
		return _default;
	}

	virtual void set(_Key key, _Val value)
	{
		if (!this->_isMutable)
		{
			this->_throw("set new value", _ERROR_DETAILS_);
		}
		this->_map[key] = value;
	}

	virtual void remove(_Key key)
	{
		if (!this->_isMutable)
		{
			this->_throw("remove value", _ERROR_DETAILS_);
		}
		if (this->contains(key))
		{
			this->_map.erase(key);
		}
	}

	virtual bool contains(_Key key)
	{
		return this->_map.find(key) != this->_map.end();
	}

	bool isMutable()
	{
		return this->_isMutable;
	}

	bool isEmpty()
	{
		return this->_map.empty();
	}

	const_iterator cbegin()
	{
		return this->_map.cbegin();
	}

	const_iterator cend()
	{
		return this->_map.cend();
	}

	const_reverse_iterator crbegin()
	{
		return this->_map.crbegin();
	}

	const_reverse_iterator crend()
	{
		return this->_map.crend();
	}

	iterator begin()
	{
		if (!this->_isMutable)
		{
			this->_throw("retrieve non-constant iterator", _ERROR_DETAILS_);
		}
		return this->_map.begin();
	}

	iterator end()
	{
		if (!this->_isMutable)
		{
			this->_throw("retrieve non-constant iterator", _ERROR_DETAILS_);
		}
		return this->_map.end();
	}

	reverse_iterator rbegin()
	{
		if (!this->_isMutable)
		{
			this->_throw("retrieve non-constant reverse iterator", _ERROR_DETAILS_);
		}
		return this->_map.rbegin();
	}

	reverse_iterator rend()
	{
		if (!this->_isMutable)
		{
			this->_throw("retrieve non-constant reverse iterator", _ERROR_DETAILS_);
		}
		return this->_map.rend();
	}
};

__WASP_END__


#endif // WASP_UTILS_QUERY_DICT_H
