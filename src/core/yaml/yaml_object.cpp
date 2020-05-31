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
 * An implementation of core/yaml/yaml_object.h
 */

// Header.
#include "./yaml_object.h"

// Framework modules.
#include "./yaml_array.h"


__YAML_BEGIN__

const std::string YAMLObject::DEFAULT_INDENT = "  ";

std::string YAMLObject::indent_string(
	const std::string& indent, bool prepend_new_line
) const
{
	std::string res;
	if (prepend_new_line)
	{
		res += "\n";
	}

	size_t i = 0, n_minus_1 = this->_objects.size() - 1;
	for (const auto& obj : this->_objects)
	{
		if (!prepend_new_line && i == 0)
		{
			res += " ";
		}
		else
		{
			res += indent;
		}

		res += obj.first + ":" + obj.second->indent_string(
			indent + YAMLObject::DEFAULT_INDENT, true
		);
		if (i++ < n_minus_1)
		{
			res += "\n";
		}
	}

	return res;
}

void YAMLObject::put(
	const std::string& key,
	const std::shared_ptr<YAMLValue>& val
)
{
	this->_objects.emplace_back(key, val);
}

void YAMLObject::put(
	const std::string& key,
	const std::shared_ptr<YAMLObject>& val
)
{
	this->_objects.emplace_back(key, val);
}

void YAMLObject::put(
	const std::string& key,
	const std::shared_ptr<YAMLArray>& val
)
{
	this->_objects.emplace_back(key, val);
}

void YAMLObject::put(const std::string& key, const std::string& val)
{
	this->_objects.emplace_back(key, std::make_shared<YAMLValue>(val));
}

void YAMLObject::put(const std::string& key, const char* val)
{
	this->_objects.emplace_back(key, std::make_shared<YAMLValue>(val));
}

void YAMLObject::remove(const std::string& key)
{
	for (size_t i = 0; i < this->_objects.size(); i++)
	{
		if (this->_objects[i].first == key)
		{
			this->_objects.erase(this->_objects.begin() + i);
			break;
		}
	}
}

std::shared_ptr<IYAMLObject> YAMLObject::pop(const std::string& key)
{
	for (size_t i = 0; i < this->_objects.size(); i++)
	{
		if (this->_objects[i].first == key)
		{
			auto obj = this->_objects[i].second;
			this->_objects.erase(this->_objects.begin() + i);
			return obj;
		}
	}

	return nullptr;
}

YAMLObject* YAMLObject::get_object(const std::string& key)
{
	return this->_get_val<YAMLObject>(key);
}

YAMLArray* YAMLObject::get_array(const std::string& key)
{
	return this->_get_val<YAMLArray>(key);
}

bool YAMLObject::get_bool(const std::string& key)
{
	auto val = this->_get_val<YAMLValue>(key);
	if (val)
	{
		auto str = val->get();
		return str == "true" || str == "Yes";
	}

	return false;
}

char YAMLObject::get_char(const std::string& key)
{
	char res = '\0';
	auto val = this->_get_val<YAMLValue>(key);
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

long YAMLObject::get_int(const std::string& key)
{
	auto val = this->_get_val<YAMLValue>(key);
	if (val)
	{
		return std::stol(val->get(), nullptr, 10);
	}

	return 0;
}

double YAMLObject::get_double(const std::string& key)
{
	auto val = this->_get_val<YAMLValue>(key);
	if (val)
	{
		return std::stod(val->get());
	}

	return 0;
}

std::string YAMLObject::get_string(const std::string& key)
{
	auto val = this->_get_val<YAMLValue>(key);
	if (val)
	{
		return val->get();
	}

	return "";
}

std::string YAMLObject::to_string() const
{
	return "---\n" + this->indent_string("", true);
}

size_t YAMLObject::size() const
{
	return this->_objects.size();
}

bool YAMLObject::is_empty() const
{
	return this->_objects.empty();
}

void YAMLObject::clear()
{
	this->_objects.clear();
}

__YAML_END__
