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
 * render/base.h
 *
 * Purpose: template render base.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"
#include "../core/object/object.h"


__RENDER_BEGIN__

class IContext
{
public:
	virtual core::object::Object* find_var(const std::string& key) = 0;
	virtual ~IContext() = default;
};


class ITemplate
{
public:
	virtual ~ITemplate() = default;

	/// Renders template code using given context.
	virtual std::string render(IContext* context) = 0;
};


class BaseEngine
{
public:
	virtual ~BaseEngine() = default;

	/// Return a pointer to compiled BaseTemplate object for the given template code,
	/// handling template inheritance recursively.
	virtual ITemplate* from_string(const std::string& template_code) = 0;

	/// Return a pointer to compiled BaseTemplate object for the given template name,
	/// handling template inheritance recursively.
	virtual ITemplate* get_template(const std::string& template_name) = 0;

	/// Render the template specified by template_name with the given context.
	virtual std::string render_to_string(
		const std::string& template_name, IContext* context = nullptr
	) = 0;
};


class ILoader
{
public:
	virtual ~ILoader() = default;
	virtual ITemplate* get_template(
		const std::string& template_path,
		const std::vector<std::string>& dirs,
		BaseEngine* engine
	) = 0;
};

__RENDER_END__
