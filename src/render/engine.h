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
 * render/engine.h
 *
 * Purpose: Wasp's default template render engine.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./base.h"
#include "./builtins.h"
#include "./loaders.h"
#include "./template.h"
#include "./builtins.h"
#include "./exceptions.h"
#include "../core/logger.h"
#include "../core/string/str.h"
#include "../core/path.h"
#include "../collections/dict.h"


__RENDER_BEGIN__

class Engine : public BaseEngine
{
protected:
	const std::string APP_DIRNAME = "templates";

	std::vector<std::string> _dirs;
	bool _use_app_dirs;
	bool _debug;
	bool _auto_escape;
	std::vector<ILoader*> _loaders;
	bool _use_default_loaders;
	collections::Dict<std::string, Filter> _filters;
	backends::BaseBackend* _backend;

public:
	explicit Engine(
		backends::BaseBackend* backend,
		const std::vector<std::string>& dirs = {},
		bool use_app_dirs = false,
		bool debug = false,
		bool auto_escape = true,
		const std::vector<ILoader*>& loaders = {},
		const collections::Dict<std::string, Filter>& custom_filters = {},
		core::ILogger* logger = nullptr
	);

	~Engine() override;

	ITemplate* find_template(
		const std::string& name,
		const std::vector<std::string>& dirs
	);

	/// Return a pointer to compiled BaseTemplate object for the given template code,
	/// handling template inheritance recursively.
	ITemplate* from_string(const std::string& template_code) override;

	/// Return a pointer to compiled BaseTemplate object for the given template name,
	/// handling template inheritance recursively.
	ITemplate* get_template(const std::string& template_name) override;

	/// Render the template specified by template_name with the given context.
	std::string render_to_string(
		const std::string& template_name, const std::shared_ptr<IContext>& context = nullptr
	) override;

	/// Returns current backend.
	backends::BaseBackend* backend();
};

__RENDER_END__
