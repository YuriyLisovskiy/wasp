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


__WASP_BEGIN__

template <typename _Key, typename _Val>
class QueryDict
{
private:
	std::map<_Key, _Val> _map;

public:
	typedef typename std::map<_Key, _Val>::const_iterator const_iterator;
	typedef typename std::map<_Key, _Val>::const_reverse_iterator const_reverse_iterator;

	QueryDict() = default;
	explicit QueryDict(const std::map<_Key, _Val>& srcMap)
	{
		this->_map = srcMap;
	}

	_Val get(_Key key)
	{
		return this->_map[key];
	}

	bool contains(_Key key)
	{
		return this->_map.find(key) != this->_map.end();
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
};

__WASP_END__


#endif // WASP_UTILS_QUERY_DICT_H
