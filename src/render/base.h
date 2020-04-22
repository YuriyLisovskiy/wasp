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
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"
#include "../core/object/object.h"


__RENDER_BEGIN__

class IContext
{
public:
	typedef std::map<std::string, std::shared_ptr<core::object::Object>> scope_t;

	virtual ~IContext() = default;
	virtual std::shared_ptr<core::object::Object> find_var(
		const std::string& key
	) = 0;
	virtual void push_var(
		const std::string& key,
		const std::shared_ptr<core::object::Object>& val
	) = 0;
	virtual void push_scope(const scope_t& scope) = 0;
	virtual void pop_scope() = 0;
};


class ITemplate
{
public:
	virtual ~ITemplate() = default;

	/// Renders template code using given context.
	virtual std::string render(IContext* context) = 0;
};


class IEngine
{
public:
	virtual ~IEngine() = default;

	/// Return a pointer to compiled BaseTemplate object for the given template code,
	/// handling template inheritance recursively.
	virtual std::shared_ptr<ITemplate> from_string(const std::string& template_code) = 0;

	/// Return a pointer to compiled BaseTemplate object for the given template name,
	/// handling template inheritance recursively.
	virtual std::shared_ptr<ITemplate> get_template(const std::string& template_name) = 0;

	virtual void load_libraries() = 0;
};


class ILoader
{
public:
	virtual ~ILoader() = default;
	virtual std::shared_ptr<ITemplate> get_template(
		const std::string& template_path,
		const std::vector<std::string>& dirs,
		IEngine* engine
	) = 0;
	virtual std::map<std::string, std::shared_ptr<ITemplate>> cache_templates(
		const std::vector<std::string>& dirs,
		IEngine* engine
	) = 0;
};

__RENDER_END__
