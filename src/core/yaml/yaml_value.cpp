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
 * An implementation of core/yaml/yaml_value.h
 */

// Header.
#include "./yaml_value.h"

// Framework modules.
#include "../string.h"


__YAML_BEGIN__

const std::string YAMLValue::DEFAULT_INDENT = "  ";

std::string YAMLValue::indent_string(
	const std::string& indent, bool /* ignored */
) const
{
	std::string res;
	if (this->_value.find('\n') != std::string::npos)
	{
		res = " |-";
		auto parts = str::split(this->_value, '\n');
		for (const auto& part : parts)
		{
			res.append("\n").append(indent).append(part);
		}
	}
	/*
	else if (this->_value.find(' ') != std::string::npos)
	{
		if (this->_value.find('\'') == std::string::npos)
		{
			res = " \'" + this->_value + "\'";
		}
		else if (this->_value.find('"') == std::string::npos)
		{
			res = " \"" + this->_value + "\"";
		}
		else
		{
			auto new_value = str::replace(this->_value, "'", "\\'");
			res = " \'" + new_value + "\'";
		}
	}
	*/
	else
	{
		if (!this->_value.empty())
		{
			res = " " + this->_value;
		}
	}

	return res;
}

std::string YAMLValue::get() const
{
	return this->_value;
}

std::string YAMLValue::to_string() const
{
	return this->_value;
}

__YAML_END__
