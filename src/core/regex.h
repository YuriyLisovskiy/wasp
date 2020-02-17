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
 * core/regex.h
 *
 * Purpose: C++ regular expression's utilities.
 */

#pragma once

// C++ libraries.
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./string/str.h"


__RGX_BEGIN__

class Regex final
{
private:
	bool _is_matched;
	bool _is_searched;
	std::string _to_match;
	std::regex _expr;
	std::smatch _matches;
	std::vector<std::string> _groups;

public:
	explicit Regex(const std::string& expr);
	explicit Regex(
		const std::string& expr,
		std::regex_constants::syntax_option_type sot
	);
	bool match(const std::string& to_match);
	bool search(const std::string& to_search);
	std::vector<std::string> groups();
	std::string group(size_t pos);

	static std::string escape(const std::string& input);
};


class ArgRegex final
{
private:
	enum state_enum
	{
		s_str,
		s_check_if_arg,
		s_arg_name,
		s_regex
	};

	std::smatch _matches;

	bool _is_matched;
	bool _is_searched;
	std::string _to_match;
	std::string::const_iterator _to_match_iter_start;

	std::string _s;
	std::string _orig;
	std::regex _rgx;

	std::vector<std::string> _pattern_parts;
	std::vector<std::string> _keys;

	std::map<std::string, std::string> _groups;

public:
	explicit ArgRegex(const std::string& rgx);
	bool match(const std::string& to_match);
	bool search_all(const std::string& to_search);
	void search_iter(const std::string& to_search);
	bool next();
	std::map<std::string, std::string> groups();
	std::string group(const std::string& key, const std::string& default_val = "");
	std::vector<std::string> parts();
	std::string original();

private:
	void _make_groups();
	virtual std::string _parse(const std::string& pattern);
};

__RGX_END__
