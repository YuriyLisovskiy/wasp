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

std::string YAMLObject::indent_string(const std::string& indent) const
{
	std::string res;
	size_t i = 0;
	for (const auto& obj : this->_objects)
	{
		if (i == 0)
		{
			res += " " + obj.first + ":" + obj.second->indent_string(indent + YAMLObject::DEFAULT_INDENT);
		}
		else
		{
			res += "\n" + indent + obj.first + ":" + obj.second->indent_string(indent + YAMLObject::DEFAULT_INDENT);
		}

		i++;
	}

	return res;
}

void YAMLObject::put(
	const std::string& key,
	const std::shared_ptr<YAMLValue>& val
)
{
	this->_objects[key] = val;
}

void YAMLObject::put(
	const std::string& key,
	const std::shared_ptr<YAMLObject>& val
)
{
	this->_objects[key] = val;
}

void YAMLObject::put(
	const std::string& key,
	const std::shared_ptr<YAMLArray>& val
)
{
	this->_objects[key] = val;
}

void YAMLObject::put(const std::string& key, const std::string& val)
{
	this->_objects[key] = std::make_shared<YAMLValue>(val);
}

void YAMLObject::put(const std::string& key, const char* val)
{
	this->_objects[key] = std::make_shared<YAMLValue>(val);
}

void YAMLObject::remove(const std::string& key)
{
	if (this->_objects.find(key) != this->_objects.end())
	{
		this->_objects.erase(key);
	}
}

std::shared_ptr<IYAMLObject> YAMLObject::pop(const std::string& key)
{
	if (this->_objects.find(key) != this->_objects.end())
	{
		auto obj = this->_objects[key];
		this->_objects.erase(key);
		return obj;
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
	return "---\n" + this->indent_string("");
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
