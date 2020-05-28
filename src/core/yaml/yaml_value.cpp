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


__YAML_INTERNAL_BEGIN__

std::string YAMLValue::_indent_string(const std::string& indent) const
{
	return this->_value;
}

YAMLValue::YAMLValue(const std::string& val)
{
	this->_value = val;
}

std::string YAMLValue::get() const
{
	return this->_value;
}

__YAML_INTERNAL_END__
