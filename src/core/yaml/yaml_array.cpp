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


__YAML_BEGIN__

const std::string YAMLArray::DEFAULT_INDENT = "  ";

std::string YAMLArray::indent_string(const std::string& indent) const
{
	std::string res, curr_indent;
	auto default_indent_size = YAMLArray::DEFAULT_INDENT.size();
	if (indent.size() >= default_indent_size)
	{
		curr_indent = indent.substr(0, indent.size() - default_indent_size);
	}
	else
	{
		curr_indent = indent;
	}

	for (const auto& obj : this->_objects)
	{
		res += "\n" + curr_indent + "-" + obj->indent_string(curr_indent + YAMLArray::DEFAULT_INDENT);
	}

	return res;
}

void YAMLArray::add(const std::shared_ptr<YAMLValue>& val)
{
	this->_objects.push_back(val);
}

void YAMLArray::add(const std::shared_ptr<YAMLObject>& val)
{
	this->_objects.push_back(val);
}

void YAMLArray::add(const std::string& key, const std::shared_ptr<YAMLArray>& val)
{
	auto obj = std::make_shared<YAMLObject>();
	obj->put(key, val);
	this->_objects.push_back(obj);
}

void YAMLArray::remove_at(int idx)
{
	if (idx < this->_objects.size())
	{
		this->_objects.erase(this->_objects.begin() + idx);
	}
}

std::shared_ptr<IYAMLObject> YAMLArray::pop_at(int idx)
{
	if (idx < this->_objects.size())
	{
		auto item = this->_objects.at(idx);
		this->_objects.erase(this->_objects.begin() + idx);
		return item;
	}

	return nullptr;
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
	auto val = this->_get_val<YAMLValue>(idx);
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
	auto val = this->_get_val<YAMLValue>(idx);
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
	auto val = this->_get_val<YAMLValue>(idx);
	if (val)
	{
		return std::stol(val->get(), nullptr, 10);
	}

	return 0;
}

double YAMLArray::get_double(int idx)
{
	auto val = this->_get_val<YAMLValue>(idx);
	if (val)
	{
		return std::stod(val->get());
	}

	return 0;
}

std::string YAMLArray::get_string(int idx)
{
	auto val = this->_get_val<YAMLValue>(idx);
	if (val)
	{
		return val->get();
	}

	return "";
}

std::string YAMLArray::to_string() const
{
	return "---\n" + this->indent_string("");
}

size_t YAMLArray::size() const
{
	return this->_objects.size();
}

bool YAMLArray::is_empty() const
{
	return this->_objects.empty();
}

void YAMLArray::clear()
{
	this->_objects.clear();
}

__YAML_END__
