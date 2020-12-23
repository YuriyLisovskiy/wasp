/**
 * views/args.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./args.h"


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
