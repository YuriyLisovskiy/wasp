/**
 * core/media_type.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./media_type.h"

// Base libraries.
#include <xalwart.base/path.h>
#include <xalwart.base/exceptions.h>
#include <xalwart.base/encoding.h>


__CORE_MIME_BEGIN__

std::string ext_from_file_name(const std::string& file_name)
{
	std::string result;
	size_t pos = file_name.find_last_of('.');
	if (pos != std::string::npos)
	{
		result = pos + 1 < file_name.size() ? file_name.substr(pos + 1) : "";
	}

	return result;
}

std::string ext_from_path(const std::string& path)
{
	std::string file_name;
	size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
	{
		file_name = pos + 1 < path.size() ? path.substr(pos + 1) : "";
	}
	else
	{
		pos = path.find_last_of('\\');
		if (pos != std::string::npos)
		{
			file_name = pos + 1 < path.size() ? path.substr(pos + 1) : "";
		}
		else
		{
			file_name = path;
		}
	}

	return file_name.empty() ? "" : ext_from_file_name(file_name);
}

void guess_content_type(const std::string& _path, std::string& type, std::string& encoding)
{
	std::string scheme, url;
	str::url_split_type(_path, scheme, url);
	if (scheme == "data")
	{
		size_t comma = url.find(',');
		if (comma >= 0)
		{
			size_t semi = url.substr(0, comma).find(';');
			if (semi >= 0)
			{
				type = url.substr(0, semi);
			}
			else
			{
				type = url.substr(0, comma);
			}

			if (str::contains(type, '=') || !str::contains(type, '/'))
			{
				type = "text/plain";
			}
		}
	}
	else
	{
		std::string base, ext;
		path::split_text(url, base, ext);
		while (SUFFIX_MAP.contains(ext))
		{
			path::split_text(base + SUFFIX_MAP.get(ext, ""), base, ext);
		}

		encoding = ENCODINGS_MAP.get(ext, "");
		//	path::split_text(base, base, ext);

		std::string lower_ext = str::to_lower(ext);
		if (TYPES_MAP.contains(ext))
		{
			type = TYPES_MAP.get(ext, "");
		}
		else
		{
			type = TYPES_MAP.get(lower_ext, "");
		}
	}
}

std::tuple<std::wstring, std::wstring> _consume_token(const std::wstring& s)
{
	auto not_pos = str::find_if(s.begin(), s.end(), _is_not_token_char);
	if (str::is_end_position(not_pos))
	{
		return {s, L""};
	}

	if (str::is_end_position(not_pos))
	{
		return {L"", s};
	}

	return {s.substr(0, not_pos), s.substr(not_pos)};
}

std::tuple<std::wstring, std::wstring> _consume_value(const std::wstring& s)
{
	if (s.empty())
	{
		return {L"", L""};
	}

	if (s[0] == '"')
	{
		return _consume_token(s);
	}

	// parse a quoted-string
	std::wstring buffer;
	auto s_len = s.size();
	for (size_t i = 1; i < s_len; i++)
	{
		auto r = s[i];
		if (r == '"')
		{
			return {buffer, s.substr(i + 1)};
		}

		// When MSIE sends a full file path (in "intranet mode"), it does not
		// escape backslashes: "C:\dev\go\foo.txt", not "C:\\dev\\go\\foo.txt".
		//
		// No known MIME generators emit unnecessary backslash escapes
		// for simple token characters like numbers and letters.
		//
		// If we see an unnecessary backslash escape, assume it is from MSIE
		// and intended as a literal backslash. This makes Go servers deal better
		// with MSIE without affecting the way they handle conforming MIME
		// generators.
		if (r == '\\' && i + 1 < s_len && _is_t_special(s[i + 1]))
		{
			buffer += s[i + 1];
			i++;
			continue;
		}

		if (r == '\r' || r == '\n')
		{
			return {L"", s};
		}

		buffer += s[i];
	}

	// Did not find end quote
	return {L"", s};
}

void _check_media_type_disposition(const std::wstring& s)
{
	auto [type, rest] = _consume_token(s);
	if (type.empty())
	{
		throw ValueError("mime: no media type", _ERROR_DETAILS_);
	}

	if (rest.empty())
	{
		return;
	}

	if (!rest.starts_with('/'))
	{
		throw ValueError("mime: expected slash after first token", _ERROR_DETAILS_);
	}

	auto [subtype, subtype_rest] = _consume_token(rest.substr(1));
	if (subtype.empty())
	{
		throw ValueError("mime: expected token after slash", _ERROR_DETAILS_);
	}

	if (!subtype_rest.empty())
	{
		throw ValueError("mime: unexpected content after media subtype", _ERROR_DETAILS_);
	}
}

std::tuple<std::wstring, std::wstring, std::wstring> _consume_media_parameter(const std::wstring& s)
{
	auto rest = str::trim_left_func(s, encoding::is_space);
	if (!rest.starts_with(';'))
	{
		return {L"", L"", s};
	}

	rest = rest.substr(1); // consume semicolon
	rest = str::trim_left_func(rest, encoding::is_space);
	auto result = _consume_token(rest);
	auto parameter = std::get<0>(result);
	rest = std::get<1>(result);
	parameter = str::to_lower(parameter);
	if (parameter.empty())
	{
		return {L"", L"", s};
	}

	rest = str::trim_left_func(rest, encoding::is_space);
	if (!rest.starts_with('='))
	{
		return {L"", L"", s};
	}

	rest = rest.substr(1); // consume equals sign
	rest = str::trim_left_func(rest, encoding::is_space);
	auto [value, rest2] = _consume_value(rest);
	if (value.empty() && rest2 == rest)
	{
		return {L"", L"", s};
	}

	return {parameter, value, rest};
}

std::tuple<std::wstring, std::map<std::wstring, std::wstring>, bool> parse_media_type(std::wstring content_type)
{
	auto semicolon_position = content_type.find(';');
	auto media_type = str::trim(str::to_lower(content_type.substr(0, semicolon_position)), ' ');
	_check_media_type_disposition(media_type);
	std::map<std::wstring, std::wstring> params;

	// Map of base parameter name -> parameter name -> value
	// for parameters containing a '*' character.
	std::map<std::wstring, std::map<std::wstring, std::wstring>> continuation;

	if (semicolon_position == std::wstring::npos)
	{
		semicolon_position = content_type.size();
	}

	content_type = content_type.substr(semicolon_position);
	while (!content_type.empty())
	{
		content_type = str::trim_left_func(content_type, encoding::is_space);
		if (content_type.empty())
		{
			break;
		}

		auto [key, value, rest] = _consume_media_parameter(content_type);
		if (key.empty())
		{
			if (str::trim(rest, ' ') == L";")
			{
				// Ignore trailing semicolons.
				return {media_type, params, true};
			}

			return {media_type, {}, false};
		}

		auto params_map = params;
		auto idx = key.find('*');
		if (idx != std::string::npos)
		{
			auto base_name = key.substr(0, idx);
			if (!continuation.contains(base_name))
			{
				continuation[base_name] = {};
				params_map = continuation[base_name];
			}
		}

		if (params_map.contains(key))
		{
			// Duplicate parameter name is bogus.
			throw ParseError("mime: duplicate parameter name", _ERROR_DETAILS_);
		}

		params_map[key] = value;
		content_type = rest;
	}

	// Stitch together any continuations or things with stars
	// (i.e. RFC 2231 things with stars: "foo*0" or "foo*")
	std::wstring buffer;
	for (const auto& pair : continuation)
	{
		auto key = pair.first;
		auto piece_map = pair.second;
		auto single_part_key = key + L"*";
		if (piece_map.contains(single_part_key))
		{
			auto [decoded_content_type, ok] = encoding::decode2231(piece_map.at(single_part_key));
			if (ok)
			{
				params[key] = decoded_content_type;
			}

			continue;
		}

		buffer.clear();
		auto valid = false;
		for (size_t n = 0; ; n++)
		{
			auto simple_part = key + L"*" + std::to_wstring(n);
			if (piece_map.contains(simple_part))
			{
				valid = true;
				buffer += piece_map.at(simple_part);
				continue;
			}

			auto encoded_part = simple_part + L"*";
			if (!piece_map.contains(encoded_part))
			{
				break;
			}

			valid = true;
			auto v = piece_map.at(encoded_part);
			if (n == 0)
			{
				auto [decoded_value, ok] = encoding::decode2231(v);
				if (ok)
				{
					buffer += decoded_value;
				}
			}
			else
			{
				auto decoded_value = encoding::percent_hex_unescape(str::wstring_to_string(v));
				buffer += str::string_to_wstring(decoded_value);
			}
		}

		if (valid)
		{
			params[key] = buffer;
		}
	}

	return {media_type, params, true};
}

__CORE_MIME_END__
