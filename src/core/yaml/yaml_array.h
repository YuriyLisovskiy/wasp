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
 * core/yaml/yaml_array.h
 *
 * Purpose:
 *  TODO: implement docs for core/yaml/yaml_array.h
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./yaml_object.h"


__YAML_BEGIN__

class YAMLArray final : public IYAMLObject
{
private:
	std::vector<std::shared_ptr<IYAMLObject>> _objects;

	template <typename ValueT>
	ValueT* _get_val(int idx)
	{
		if (idx < this->_objects.size())
		{
			return dynamic_cast<ValueT*>(this->_objects[idx].get());
		}

		return nullptr;
	}

public:
	const static std::string DEFAULT_INDENT;

public:
	[[nodiscard]] std::string indent_string(
		const std::string& indent, bool prepend_new_line
	) const override;

	YAMLArray() = default;

	void add(const std::shared_ptr<YAMLValue>& val);
	void add(const std::shared_ptr<YAMLObject>& val);
	void add(const std::string& key, const std::shared_ptr<YAMLArray>& val);

	template <typename ValueT, typename = std::enable_if<
		std::is_fundamental<ValueT>::value ||
		std::is_same<std::basic_string<char>, ValueT>::value ||
		std::is_same<const char*, ValueT>::value
	>>
	void add(ValueT val)
	{
		this->_objects.push_back(std::make_shared<YAMLValue>(val));
	}

	void remove_at(int idx);
	std::shared_ptr<IYAMLObject> pop_at(int idx);

	YAMLObject* get_object(int idx);
	YAMLArray* get_array(int idx);

	bool get_bool(int idx);
	char get_char(int idx);
	long get_int(int idx);
	double get_double(int idx);
	std::string get_string(int idx);

	[[nodiscard]] std::string to_string() const override;

	[[nodiscard]] size_t size() const;
	[[nodiscard]] bool is_empty() const;
	void clear();
};

__YAML_END__
