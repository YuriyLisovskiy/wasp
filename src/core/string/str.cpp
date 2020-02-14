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
 * An implementation of core/string/str.h
 */

#include "./str.h"


__STR_BEGIN__

void url_split_type(const std::string& url, std::string& scheme, std::string& data)
{
	bool _break = false, colon_found = false;
	auto it = url.begin();
	while (it != url.end() && !_break)
	{
		switch (*it)
		{
			case '/':
				_break = true;
				break;
			case ':':
				colon_found = true;
				_break = true;
				break;
			default:
				scheme += *it;
				break;
		}

		it++;
	}

	if (!scheme.empty() && colon_found)
	{
		data = std::string(it, url.end());
	}
	else
	{
		scheme = "";
		data = url;
	}
}

bool contains(const std::string& _str, char _char)
{
	return _str.find(_char) != std::string::npos;
}

std::string lower(const std::string& _str)
{
	std::string res(_str);
	std::transform(
			res.begin(),
			res.end(),
			res.begin(),
			[](unsigned char c){ return std::tolower(c); }
	);
	return res;
}

std::string upper(const std::string& _str)
{
	std::string res(_str);
	std::transform(
			res.begin(),
			res.end(),
			res.begin(),
			[](unsigned char c){ return std::toupper(c); }
	);
	return res;
}

std::vector<std::string> split(const std::string& str, char delimiter)
{
	std::vector<std::string> result;
	if (str.empty())
	{
		return result;
	}

	std::string current;
	for (const char& _char : str)
	{
		if (_char == delimiter)
		{
			result.push_back(current);
			current.clear();
		}
		else
		{
			current += _char;
		}
	}

	result.push_back(current);
	return result;
}

std::vector<std::string> rsplit(const std::string& str, char delimiter, size_t n)
{
	if (n < 0)
	{
		return split(str, delimiter);
	}

	std::vector<std::string> result;
	std::string current;
	size_t split_count = 0;
	long str_last_pos = (long)str.size() - 1;
	long int i;
	for (i = str_last_pos; i >= 0 && split_count < n; i--)
	{
		if (str[i] == delimiter)
		{
			std::reverse(current.begin(), current.end());
			result.insert(result.begin(), current);
			current.clear();
			split_count++;
		}
		else
		{
			current += str[i];
		}
	}

	result.insert(result.begin(), str.substr(0, i + 1));
	return result;
}

bool starts_with(const std::string& src, const std::string& prefix)
{
	if (src.size() < prefix.size())
	{
		return false;
	}

	for (size_t i = 0 ; i < prefix.size(); i++)
	{
		if (src[i] != prefix[i])
		{
			return false;
		}
	}

	return true;
}

bool ends_with(const std::string& src, const std::string& suffix)
{
	if (src.size() < suffix.size())
	{
		return false;
	}

	size_t j = src.size() - 1;
	for (long i = (long)suffix.size() - 1; i >= 0; i--)
	{
		if (src[j--] != suffix[i])
		{
			return false;
		}
	}

	return true;
}

void ltrim(std::string& s, const std::string& to_trim)
{
	while (starts_with(s, to_trim))
	{
		s.erase(s.begin(), s.begin() + to_trim.size());
	}
}

std::string ltrim(const std::string& s, const std::string& to_trim)
{
	std::string copy = s;
	ltrim(copy, to_trim);
	return copy;
}

void rtrim(std::string& s, const std::string& to_trim)
{
	while (ends_with(s, to_trim))
	{
		s.erase(
			s.end() - to_trim.size(),
			s.end()
		);
	}
}

std::string rtrim(const std::string& s, const std::string& to_trim)
{
	std::string copy = s;
	rtrim(copy, to_trim);
	return copy;
}

void trim(std::string& s, const std::string& to_trim)
{
	ltrim(s, to_trim);
	rtrim(s, to_trim);
}

std::string trim(const std::string& s, const std::string& to_trim)
{
	std::string copy = s;
	trim(copy, to_trim);
	return copy;
}

size_t count(const std::string& src, char ch)
{
	size_t res = 0;
	for (const auto& c : src)
	{
		if (c == ch)
		{
			res++;
		}
	}

	return res;
}

std::string cut_edges(
	const std::string& s, size_t left, size_t right, bool trim_whitespace
)
{
	std::string copy = s;
	if (s.size() >= left + right)
	{
		if (left)
		{
			copy.erase(0, left);
		}

		if (right)
		{
			copy.erase(copy.size() - right);
		}
	}

	if (trim_whitespace)
	{
		trim(copy);
	}

	return copy;
}

__STR_END__
