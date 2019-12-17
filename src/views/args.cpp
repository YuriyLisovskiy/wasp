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
 * An implementation of args.h.
 */

#include "args.h"


__VIEWS_BEGIN__

Args::Args(const std::map<std::string, std::string>& src)
{
	this->_map = src;
}

int Args::get_int(const std::string& key, int _default)
{
	if (this->contains(key))
	{
		std::string value = this->_map[key];
		if (Args::is_number(value))
		{
			return std::stoi(this->_map[key]);
		}
	}

	return _default;
}

std::string Args::get_str(const std::string& key, const std::string& _default)
{
	if (this->contains(key))
	{
		return this->_map[key];
	}

	return _default;
}

bool Args::contains(const std::string& key)
{
	return this->_map.find(key) != this->_map.end();
}

bool Args::is_number(const std::string& val)
{
	int dot_found = false;
	for (char ch : val)
	{
		if (ch == '.')
		{
			if (dot_found)
			{
				return false;
			}

			dot_found = true;
		}
		else if (!std::isdigit(ch))
		{
			return false;
		}
	}

	return true;
}

__VIEWS_END__
