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
 * cookie.h
 * Purpose: represents an http cookies as object.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"


__HTTP_BEGIN__

class Cookie
{
private:
	std::string _name;
	std::string _value;
	std::string _expires;
	std::string _domain;
	std::string _path;
	bool _is_secure;
	bool _is_http_only;

public:
	Cookie();
	Cookie(
		std::string name,
		std::string value,
		std::string expires,
		std::string domain = "",
		std::string path = "/",
		bool is_secure = true,
		bool is_http_only = false
	);

	std::string to_string();

	bool operator==(const Cookie& right);
};

__HTTP_END__
