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
 * MultiValueDict
 * TODO: write docs.
 */

#ifndef WASP_MULTI_VALUE_DICT_H
#define WASP_MULTI_VALUE_DICT_H

#include <vector>

#include "../globals.h"
#include "dict.h"


__WASP_BEGIN__

template <typename _Key, typename _Val>
class MultiValueDict : public Dict<_Key, std::vector<_Val>>
{
public:
	_Val get(_Key key, _Val _default) override
	{
		if (this->contains(key))
		{
			auto vec = this->_map[key];
			if (vec.size() > 0)
			{
				return vec.at(vec.size() - 1);
			}
		}
		return _default;
	}

	void set(_Key key, _Val value) override
	{
		if (!this->_isMutable)
		{
			this->_throw("set new value", _ERROR_DETAILS_);
		}
		this->_map[key] = std::vector<_Val>{value};
	}

	std::vector<_Val> getList(_Key key, std::vector<_Val> _default = {})
	{
		if (this->contains(key))
		{
			auto vec = this->_map[key];
			if (vec.size() > 0)
			{
				return vec;
			}
		}
		return _default;
	}

	void setList(_Key key, std::vector<_Val> list)
	{
		if (!this->_isMutable)
		{
			this->_throw("set new value", _ERROR_DETAILS_);
		}
		this->_map[key] = list;
	}

	void appendList(_Key key, _Val val)
	{
		if (!this->_isMutable)
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
};

__WASP_END__


#endif // WASP_MULTI_VALUE_DICT_H
