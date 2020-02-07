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
 * An implementation of core/encoding.h
 */

#include "./encoding.h"


__ENCODING_BEGIN__

const uint ASCII = 0x0;

std::string encode_url(const std::string& url)
{
	enum url_state_enum
	{
		s_scheme,
		s_slash_after_scheme,
		s_host,
		s_path,
		s_query,
		s_fragment

	} state = url_state_enum::s_scheme;

	std::ostringstream oss;
	oss.fill('0');
	oss << std::hex;
	for (const char& c : url)
	{
		switch (state)
		{
			case url_state_enum::s_scheme:
				if (c == '/')
				{
					state = url_state_enum::s_slash_after_scheme;
				}

				oss << c;
				break;
			case url_state_enum::s_slash_after_scheme:
				if (c == '/')
				{
					state = url_state_enum::s_host;
				}

				oss << c;
				break;
			case url_state_enum::s_host:
				if (c == '/')
				{
					state = url_state_enum::s_path;
				}

				oss << c;
				break;
			case url_state_enum::s_path:
				if (c == '?')
				{
					oss << c;
					state = url_state_enum::s_query;
				}
				else
				{
					internal::escape(oss, c, "/");
				}
				break;
			case url_state_enum::s_query:
				if (c == '#')
				{
					oss << c;
					state = url_state_enum::s_fragment;
				}
				else
				{
					internal::escape(oss, c, "&=");
				}
				break;
			case url_state_enum::s_fragment:
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

std::string encode(const std::string& _str, uint encoding, Mode mode)
{
	std::string result;

	// TODO: add more encodings.
	switch (encoding)
	{
		case ASCII:
			result = encode_ascii(_str, mode);
			break;
		default:
			#define wasp_name_of(v) std::string(#v)
			throw core::EncodingError("unknown encoding: " + wasp_name_of(encoding), _ERROR_DETAILS_);
			#undef wasp_name_of
	}

	return result;
}

std::string encode_ascii(const std::string& _str, Mode mode)
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
					throw core::EncodingError(
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

__ENCODING_END__


__ENCODING_INTERNAL_BEGIN__

void escape(std::ostringstream& stream, char c, const std::string& safe)
{
	if (std::isalnum((unsigned char) c) || c == '-' || c == '_' || c == '.' || c == '~' || safe.find(c) != -1)
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

__ENCODING_INTERNAL_END__
