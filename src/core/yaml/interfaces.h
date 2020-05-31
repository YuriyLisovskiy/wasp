/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * core/yaml/interfaces.h
 *
 * Purpose:
 *  TODO: implement docs for core/yaml/interfaces.h
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__YAML_BEGIN__

class IYAMLObject
{
public:
	[[nodiscard]] virtual std::string indent_string(
		const std::string& indent, bool prepend_new_line
	) const = 0;
	[[nodiscard]] virtual std::string to_string() const = 0;
	virtual ~IYAMLObject() = default;
};

__YAML_END__
