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
 * core/parsers/cookie_parser.h
 *
 * Purpose: parses cookies from http request.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>

// Module definitions.
#include "../_def_.h"


__CORE_INTERNAL_BEGIN__

struct cookie_parser final
{
	enum state
	{
		req_key,
		req_val
	};

	std::map<std::string, std::string> result;

	void parse(const std::string& content);
};

__CORE_INTERNAL_END__
