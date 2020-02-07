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
 * views/args.h
 *
 * Purpose: class that contains url arguments based on std::map.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>
#include <cxxabi.h>
#include <iostream>
#include <functional>

// Module definitions.
#include "./_def_.h"


__VIEWS_BEGIN__

class Args
{
private:
	std::map<std::string, std::string> _map;

public:
	Args() = default;
	explicit Args(const std::map<std::string, std::string>& src);

	int get_int(const std::string& key, int _default = 0);
	std::string get_str(const std::string& key, const std::string& _default = "");

	bool contains(const std::string& key);

private:
	static bool is_number(const std::string& val);
};

__VIEWS_END__
