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

#ifndef WASP_UTILS_QUERY_SET_H
#define WASP_UTILS_QUERY_SET_H

#include <map>

#include "../globals.h"


__WASP_BEGIN__

template <typename _Key, typename _Val>
class QuerySet
{
private:
	std::map<_Key, _Val> _map;

public:
	void set(_Key key, _Val val)
	{
		this->_map[key] = val;
	}

	_Val get(_Key key)
	{
		if (this->contains(key))
		{
			return this->_map[key];
		}
		return _Val();
	}

	bool contains(_Key key)
	{
		return this->_map.find(key) != this->_map.end();
	}
};

__WASP_END__


#endif // WASP_UTILS_QUERY_SET_H
