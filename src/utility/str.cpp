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
 * An implementation of str.h.
 */

#include "str.h"


__UTILS_STR_INTERNAL_BEGIN__

int normalize_exp(double* val)
{
	int exponent = 0;
	double value = *val;
	while (value >= 1.0)
	{
		value /= 10.0;
		++exponent;
	}

	while (value < 0.1)
	{
		value *= 10.0;
		--exponent;
	}

	*val = value;
	return exponent;
}

std::string _format(char const* fmt, va_list args)
{
	std::map<std::string, std::string> values;
	std::stringstream stream;
	std::string last_number;
	std::string temp_arg;
	char ch = *fmt++;
	while (ch)
	{
		if (ch == '{')
		{
			switch (*fmt++)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					fmt--;
					while (std::isdigit(*fmt))
					{
						last_number += *fmt;
						fmt++;
					}

					if (*fmt == '!')
					{
						fmt++;
						if (*fmt != '!' && values.find(last_number) != values.end())
						{
							stream << values[last_number];
						}
						else
						{
							std::stringstream tempStream;
							switch (*fmt)
							{
								case '!':
									temp_arg = "!";
									break;
								case 'c':
									temp_arg = std::to_string(va_arg(args, int));
									break;
								case 's':
									temp_arg = std::string(va_arg(args, const char*));
									break;
								case 'd':
									temp_arg = std::to_string(va_arg(args, int));
									break;
								case 'x':
									tempStream << std::hex << va_arg(args, int);
									temp_arg = tempStream.str();
									tempStream.clear();
									break;
								case 'f':
									temp_arg = ftoa_fixed(va_arg(args, double));
									break;
								case 'e':
									temp_arg = ftoa_sci(va_arg(args, double));
									break;
							}
							values[last_number] = temp_arg;
							stream << temp_arg;
						}
						fmt++;
						last_number.clear();
					}
					break;
				case '{':
					stream << '{';
					break;
				case '}':
					break;
			}
		}
		else if (ch == '}')
		{
			switch (*fmt)
			{
				case '}':
					stream << '}';
					break;

			}
		}
		else
		{
			stream << ch;
		}

		ch = *fmt++;
	}

	return stream.str();
}

__UTILS_STR_INTERNAL_END__


__UTILS_STR_BEGIN__

std::string ftoa_fixed(double value)
{
	if (value == 0.0)
	{
		return "0";
	}

	std::string result;
	if (value < 0.0)
	{
		result += '-';
		value = -value;
	}

	int exponent = internal::normalize_exp(&value);
	int places = 0;
	static const int width = 4;

	while (exponent > 0)
	{
		int digit = value * 10;
		result += std::to_string(digit) + '0';
		value = value * 10 - digit;
		++places;
		--exponent;
	}

	if (places == 0)
	{
		result += '0';
	}

	result += '.';
	while (exponent < 0 && places < width)
	{
		result += '0';
		--exponent;
		++places;
	}

	while (places < width)
	{
		int digit = value * 10.0;
		result += std::to_string(digit) + '0';
		value = value * 10.0 - digit;
		++places;
	}

	return result;
}

std::string ftoa_sci(double value)
{
	if (value == 0.0)
	{
		return "0";
	}

	std::string result;
	if (value < 0.0)
	{
		result += '-';
		value = -value;
	}

	static const int width = 4;
	int exponent = internal::normalize_exp(&value);
	int digit = value * 10.0;
	result += std::to_string(digit) + '0';
	value = value * 10.0 - digit;
	--exponent;

	result += '.';
	for (int i = 0; i < width; i++)
	{
		digit = value * 10.0;
		result += std::to_string(digit) + '0';
		value = value * 10.0 - digit;
	}

	return result + 'e' + std::to_string(exponent);
}

std::string format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::string result = wasp::str::internal::_format(fmt, args);
	va_end(args);
	return result;
}

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

	for (size_t i = suffix.size() - 1; i >= 0; i--)
	{
		if (src[i] != suffix[i])
		{
			return false;
		}
	}

	return true;
}

void ltrim(std::string& s, char ch)
{
	s.erase(
		s.begin(),
		std::find_if(s.begin(), s.end(), [ch](char chr) {return ch != chr;})
	);
}

void rtrim(std::string& s, char ch)
{
	s.erase(
		std::find_if(s.rbegin(), s.rend(), [ch](int chr) {return ch != chr;}).base(),
		s.end()
	);
}

void trim(std::string& s, char ch)
{
	ltrim(s, ch);
	rtrim(s, ch);
}

std::string ltrim(const std::string& s, char ch)
{
	std::string copy = s;
	ltrim(copy, ch);
	return copy;
}

std::string rtrim(const std::string& s, char ch)
{
	std::string copy = s;
	rtrim(copy, ch);
	return copy;
}

std::string trim(const std::string& s, char ch)
{
	std::string copy = s;
	trim(copy, ch);
	return copy;
}

__UTILS_STR_END__
