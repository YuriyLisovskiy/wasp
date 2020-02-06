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

#pragma once

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../base.h"
#include "../../apps/interface.h"


__BACKENDS_BEGIN__

class IBackend
{
public:
	/// Returns backend name.
	virtual std::string name() = 0;

	/// Create and return a template for the given source code.
	///
	/// This method is optional, throws NotImplementedException by default.
	virtual ITemplate* from_string(const std::string& template_code) = 0;

	/// Load and return a template for the given path.
	///
	/// Throws TemplateDoesNotExist if no such template exists.
	virtual ITemplate* get_template(const std::string& template_path) = 0;

	/// Initializes a std::vector of directories to search for templates.
	virtual std::vector<std::string> template_dirs(
			const std::vector<apps::IAppConfig*>& apps
	) = 0;
};

__BACKENDS_END__
