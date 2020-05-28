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
 * An implementation of core/yaml/yaml_array.h
 */

// Header.
#include "./yaml_array.h"

// Framework modules.
// TODO:


__YAML_BEGIN__

std::string YAMLArray::_indent_string(const std::string& indent) const
{
	// TODO: _indent_string(const std::string& indent)
	return "TODO";
}

YAMLObject* YAMLArray::get_object(int idx)
{
	return this->_get_val<YAMLObject>(idx);
}

YAMLArray* YAMLArray::get_array(int idx)
{
	return this->_get_val<YAMLArray>(idx);
}

bool YAMLArray::get_bool(int idx)
{
	auto val = this->_get_val<internal::YAMLValue>(idx);
	if (val)
	{
		auto str = val->get();
		return str == "true" || str == "Yes" || str == "yes";
	}

	return false;
}

char YAMLArray::get_char(int idx)
{
	char res = '\0';
	auto val = this->_get_val<internal::YAMLValue>(idx);
	if (val)
	{
		auto str = val->get();
		if (!str.empty())
		{
			res = str[0];
		}
	}

	return res;
}

long YAMLArray::get_int(int idx)
{
	auto val = this->_get_val<internal::YAMLValue>(idx);
	if (val)
	{
		return std::stol(val->get(), nullptr, 10);
	}

	return 0;
}

double YAMLArray::get_double(int idx)
{
	auto val = this->_get_val<internal::YAMLValue>(idx);
	if (val)
	{
		return std::stod(val->get());
	}

	return 0;
}

std::string YAMLArray::get_string(int idx)
{
	auto val = this->_get_val<internal::YAMLValue>(idx);
	if (val)
	{
		return val->get();
	}

	return "";
}

std::string YAMLArray::to_string() const
{
	return "---\n" + this->_indent_string("");
}

size_t YAMLArray::size() const
{
	return this->_objects.size();
}

__YAML_END__
