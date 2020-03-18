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
 * render/backends/base.h
 *
 * Purpose: base classes for backends.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <utility>
#include <memory>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../base.h"
#include "./interfaces.h"
#include "../../core/exceptions.h"
#include "../../core/path.h"
#include "../../core/object/object.h"
#include "../../apps/interfaces.h"


__BACKENDS_BEGIN__

class BaseBackend : public IBackend, public core::object::Object
{
protected:
	std::string _name;
	std::vector<std::string> _dirs;
	bool _use_app_dirs;

	BaseBackend(
		std::vector<std::string> dirs,
		bool use_app_dirs
	);

public:
	/// Returns backend name.
	std::string name() override;

	/// Create and return a template for the given source code.
	///
	/// This method is optional, throws NotImplementedException by default.
	std::shared_ptr<ITemplate> from_string(const std::string& template_code) override;

	/// Load and return a template for the given path.
	///
	/// Throws TemplateDoesNotExist if no such template exists.
	std::shared_ptr<ITemplate> get_template(const std::string& template_path) override = 0;

	/// Initializes a std::vector of directories to search for templates.
	std::vector<std::string> template_dirs(
		const std::vector<apps::IAppConfig*>& apps
	) override;
};

__BACKENDS_END__
