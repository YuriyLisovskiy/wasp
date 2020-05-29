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

class YAMLObject final : public IYAMLObject
{
private:
	std::map<std::string, std::shared_ptr<IYAMLObject>> _objects;

	template <typename ValueT>
	ValueT* _get_val(const std::string& key)
	{
		if (this->_objects.find(key) != this->_objects.end())
		{
			return dynamic_cast<ValueT*>(this->_objects[key].get());
		}

		return nullptr;
	}

public:
	const static std::string DEFAULT_INDENT;

public:
	[[nodiscard]] std::string indent_string(
		const std::string& indent
	) const override;

	YAMLObject() = default;

	void put(
		const std::string& key,
		const std::shared_ptr<YAMLValue>& val
	);
	void put(
		const std::string& key,
		const std::shared_ptr<YAMLObject>& val
	);
	void put(
		const std::string& key,
		const std::shared_ptr<YAMLArray>& val
	);
	void put(const std::string& key, const std::string& val);
	void put(const std::string& key, const char* val);

	template <typename ValueT, typename = std::enable_if<
		std::is_fundamental<ValueT>::value
	>>
	void put(const std::string& key, ValueT val)
	{
		this->_objects[key] = std::make_shared<YAMLValue>(val);
	}

	void remove(const std::string& key);
	std::shared_ptr<IYAMLObject> pop(const std::string& key);

	YAMLObject* get_object(const std::string& key);
	YAMLArray* get_array(const std::string& key);

	bool get_bool(const std::string& key);
	char get_char(const std::string& key);
	long get_int(const std::string& key);
	double get_double(const std::string& key);
	std::string get_string(const std::string& key);

	[[nodiscard]] std::string to_string() const override;

	[[nodiscard]] size_t size() const;
	[[nodiscard]] bool is_empty() const;
	void clear();
};

__YAML_END__
