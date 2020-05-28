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
 * core/yaml/yaml_value.h
 *
 * Purpose:
 *  TODO: implement docs for core/yaml/yaml_value.h
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./interfaces.h"


__YAML_INTERNAL_BEGIN__

class YAMLValue final : IYAMLObject
{
private:
	std::string _value;

	[[nodiscard]] std::string _indent_string(
		const std::string& indent
	) const override;

public:
	explicit YAMLValue(const std::string& val);

	[[nodiscard]] std::string get() const;
};

__YAML_INTERNAL_END__
