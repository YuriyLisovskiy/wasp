/**
 * views/args.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./args.h"


__VIEWS_BEGIN__

Args::Args(std::map<std::string, std::string> src) : _map(std::move(src))
{
}

int Args::get_int(const std::string& key, int _default) const
{
	if (this->contains(key))
	{
		std::string value = this->_map.at(key);
		if (_is_number(value))
		{
			return std::stoi(this->_map.at(key));
		}
	}

	return _default;
}

std::string Args::get_str(const std::string& key, const std::string& _default) const
{
	if (this->contains(key))
	{
		return this->_map.at(key);
	}

	return _default;
}

bool Args::contains(const std::string& key) const
{
	return this->_map.find(key) != this->_map.end();
}

bool Args::_is_number(const std::string& val)
{
	if (val.empty())
	{
		return false;
	}

	int dot_found = false;
	size_t i = 0;
	if (val[0] == '-' || val[0] == '+')
	{
		i++;
	}

	while (i < val.size())
	{
		char ch = val[i++];
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
