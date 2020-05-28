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
 * core/yaml/yaml_object.h
 *
 * Purpose:
 *  TODO: implement docs for core/yaml/yaml_object.h
 */

#pragma once

// C++ libraries.
#include <map>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./yaml_value.h"


__YAML_BEGIN__

class YAMLArray;

class YAMLObject final : internal::IYAMLObject
{
private:
	std::map<std::string, std::shared_ptr<internal::IYAMLObject>> _objects;

	template <typename ValueT>
	ValueT* _get_val(const std::string& key)
	{
		if (this->_objects.find(key) != this->_objects.end())
		{
			return dynamic_cast<ValueT*>(this->_objects[key].get());
		}

		return nullptr;
	}

	[[nodiscard]] std::string _indent_string(
		const std::string& indent
	) const override;

public:
	YAMLObject() = default;

	template <typename ValueT, typename = std::enable_if<
		std::is_same<YAMLObject, ValueT>::value ||
		std::is_same<YAMLArray, ValueT>::value
	>>
	void put_object(
		const std::string& key,
		const std::shared_ptr<ValueT>& val
	)
	{
		this->_objects[key] = val;
	}

	template <typename ValueT, typename = std::enable_if<
		std::is_fundamental<ValueT>::value ||
		std::is_same<std::string, ValueT>::value ||
		std::is_same<const char*, ValueT>::value
	>>
	void put(const std::string& key, const ValueT& val)
	{
		if constexpr (
			std::is_same<std::string, ValueT>::value ||
			std::is_same<const char*, ValueT>::value
		)
		{
			this->_objects[key] = std::make_shared<internal::YAMLValue>(val);
		}
		else
		{
			this->_objects[key] = std::make_shared<internal::YAMLValue>(std::to_string(val));
		}
	}

	YAMLObject* get_object(const std::string& key);
	YAMLArray* get_array(const std::string& key);

	bool get_bool(const std::string& key);
	char get_char(const std::string& key);
	long get_int(const std::string& key);
	double get_double(const std::string& key);
	std::string get_string(const std::string& key);

	[[nodiscard]] std::string to_string() const;
};

__YAML_END__
