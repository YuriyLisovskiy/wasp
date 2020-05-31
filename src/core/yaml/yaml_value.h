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


__YAML_BEGIN__

class YAMLValue final : public IYAMLObject
{
private:
	std::string _value;

public:
	const static std::string DEFAULT_INDENT;

public:
	[[nodiscard]] std::string indent_string(
		const std::string& indent, bool /* ignored */
	) const override;

	YAMLValue() = default;

	template <typename ValueT, typename = std::enable_if<
		std::is_fundamental<ValueT>::value &&
		!std::is_same<std::basic_string<char>, ValueT>::value
	>>
	explicit YAMLValue(ValueT val)
	{
		if constexpr (std::is_same<bool, ValueT>::value)
		{
			this->_value = val ? "true" : "false";
		}
		else if constexpr (std::is_same<char, ValueT>::value)
		{
			this->_value = std::string(1, val);
		}
		else
		{
			this->_value = std::to_string(val);
		}
	}

	explicit YAMLValue(const std::string& val)
	{
		this->_value = val;
	}

	explicit YAMLValue(const char* val)
	{
		this->_value = val;
	}

	[[nodiscard]] std::string get() const;

	[[nodiscard]] std::string to_string() const override;
};

__YAML_END__
