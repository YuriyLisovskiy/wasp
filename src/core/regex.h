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
 * regex.h
 * Purpose: C++ regular expression's utilities.
 */

#pragma once

// C++ libraries.
#include <regex>
#include <vector>
#include <string>

// Module definitions.
#include "./_def_.h"


__RGX_BEGIN__

class Regex final
{
private:
	bool _is_matched;
	bool _is_searched;
	std::string _to_match;
	std::regex _expr;
	std::smatch _matches;

	std::vector<std::string> _get_groups();

public:
	explicit Regex(const std::string& expr);
	bool match(const std::string& to_match);
	bool search(const std::string& to_search);
	std::vector<std::string> groups();
};

__RGX_END__
