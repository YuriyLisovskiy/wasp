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

#include "encoding.h"


__WASP_ENCODING_BEGIN__

const char* ASCII = "ascii";

std::string encodeUrl(const std::string& url)
{
	enum UrlState
	{
		Scheme,
		SlashAfterScheme,
		Host,
		Path,
		Query,
		Fragment
	} state = UrlState::Scheme;

	std::ostringstream oss;
	oss.fill('0');
	oss << std::hex;

	for (const char& c : url)
	{
		switch (state)
		{
			case UrlState::Scheme:
				if (c == '/')
				{
					state = UrlState::SlashAfterScheme;
				}
				oss << c;
				break;
			case UrlState::SlashAfterScheme:
				if (c == '/')
				{
					state = UrlState::Host;
				}
				oss << c;
				break;
			case UrlState::Host:
				if (c == '/')
				{
					state = UrlState::Path;
				}
				oss << c;
				break;
			case UrlState::Path:
				if (c == '?')
				{
					oss << c;
					state = UrlState::Query;
				}
				else
				{
					internal::escape(oss, c, "/");
				}
				break;
			case UrlState::Query:
				if (c == '#')
				{
					oss << c;
					state = UrlState::Fragment;
				}
				else
				{
					internal::escape(oss, c, "&=");
				}
				break;
			case UrlState::Fragment:
				internal::escape(oss, c);
				break;
		}
	}

	return oss.str();
}

std::string quote(const std::string& _str, const std::string& safe)
{
	std::ostringstream oss;
	oss.fill('0');
	oss << std::hex;
	for (const auto& _char : _str)
	{
		internal::escape(oss, _char, safe);
	}
	return oss.str();
}

std::string encode(const std::string& _str, const char* encoding, Mode mode)
{
	std::string result;
	if (encoding == ASCII)
	{
		result = internal::encodeAscii(_str, mode);
	}
	// TODO: add more encodings here.
	else
	{
		throw EncodingError("unknown encoding: " + std::string(encoding), _ERROR_DETAILS_);
	}
	return result;
}

__WASP_ENCODING_END__


__WASP_ENCODING_INTERNAL_BEGIN__

void escape(std::ostringstream& stream, char c, const std::string& safe)
{
	if (isalnum((unsigned char) c) || c == '-' || c == '_' || c == '.' || c == '~' || safe.find(c) != -1)
	{
		stream << c;
	}
	else
	{
		stream << std::uppercase;
		stream << '%' << std::setw(2) << int((unsigned char) c);
		stream << std::nouppercase;
	}
}

std::string encodeAscii(const std::string& _str, Mode mode)
{
	std::string res;
	for (size_t i = 0; i < _str.size(); i++)
	{
		auto _char = static_cast<unsigned char>(_str[i]);
		switch (mode)
		{
			case Mode::STRICT:
				if (_char > 127)
				{
					throw EncodingError(
						"'ascii' codec can't encode character in position " + std::to_string(i) + ": ordinal not in range [0;127]",
						_ERROR_DETAILS_
					);
				}
				break;
			case Mode::IGNORE:
				continue;
			case Mode::REPLACE:
				res += '?';
				continue;
		}
		res += _char;
	}
	return res;
}

__WASP_ENCODING_INTERNAL_END__
