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
 * TODO: write docs.
 */

#include <iostream>

#include "str.h"


__UTILS_STR_INTERNAL_BEGIN__

int normalizeExp(double* val)
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
	std::string lastNumber;
	std::string tempArg;
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
						lastNumber += *fmt;
						fmt++;
					}
					if (*fmt == '!')
					{
						fmt++;
						if (*fmt != '!' && values.find(lastNumber) != values.end())
						{
							stream << values[lastNumber];
						}
						else
						{
							std::stringstream tempStream;
							switch (*fmt)
							{
								case '!':
									tempArg = "!";
									break;
								case 'c':
									tempArg = std::to_string(va_arg(args, int));
									break;
								case 's':
									tempArg = std::string(va_arg(args, const char*));
									break;
								case 'd':
									tempArg = std::to_string(va_arg(args, int));
									break;
								case 'x':
									tempStream << std::hex << va_arg(args, int);
									tempArg = tempStream.str();
									tempStream.clear();
									break;
								case 'f':
									tempArg = wasp::str::ftoaFixed(va_arg(args, double));
									break;
								case 'e':
									tempArg = wasp::str::ftoaSci(va_arg(args, double));
									break;
							}
							values[lastNumber] = tempArg;
							stream << tempArg;
						}
						fmt++;
						lastNumber.clear();
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

std::string ftoaFixed(double value)
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

	int exponent = wasp::str::internal::normalizeExp(&value);
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

std::string ftoaSci(double value)
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
	int exponent = wasp::str::internal::normalizeExp(&value);
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

void urlSplitType(const std::string& url, std::string& scheme, std::string& data)
{
	bool _break = false, colonFound = false;
	auto it = url.begin();
	while (it != url.end() && !_break)
	{
		switch (*it)
		{
			case '/':
				_break = true;
				break;
			case ':':
				colonFound = true;
				_break = true;
				break;
			default:
				scheme += *it;
				break;
		}
		it++;
	}
	if (!scheme.empty() && colonFound)
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
	std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c){ return std::tolower(c); });
	return res;
}

__UTILS_STR_END__
