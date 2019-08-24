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
 * request_parameters definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_REQUEST_PARAMETERS_H
#define WASP_HTTP_REQUEST_PARAMETERS_H

#include <vector>

#include "../globals.h"
#include "../collections/dict.h"
#include "../collections/multi_dict.h"


__WASP_BEGIN__

template <typename _Key, typename _Val>
class RequestParameters
{
private:
	Dict<_Key, _Val> _dict;
	MultiValueDict<_Key, _Val> _multiDict;

public:
	explicit RequestParameters() = default;

	explicit RequestParameters(Dict<_Key, _Val> dict, MultiValueDict<_Key, _Val> multiDict)
	{
		this->_dict = dict;
		this->_multiDict = multiDict;
	}

	_Val get(_Key key, _Val _default = _Val{})
	{
		return this->_dict.get(key, _default);
	}

	std::vector<_Val> getList(_Key key, std::vector<_Val> _default = std::vector<_Val>{})
	{
		return this->_multiDict.get(key, _default);
	}

	bool contains(_Key key)
	{
		return this->_dict.contains(key);
	}

	bool containsList(_Key key)
	{
		return this->_multiDict.contains(key);
	}
};

__WASP_END__


#endif // WASP_HTTP_REQUEST_PARAMETERS_H
