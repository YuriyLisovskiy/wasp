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
 * Purpose: template render base.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"


__RENDER_BEGIN__

class BaseTemplate
{
public:

};


class BaseEngine
{
protected:
	std::vector<std::string> _dirs;
	bool _use_app_dirs;

public:
	/// Create and return a template for the given source code.
	///
	/// This method is optional.
	virtual BaseTemplate* from_string(const std::string& template_code);

	/// Load and return a template for the given path.
	///
	/// Throws TemplateDoesNotExist if no such template exists.
	virtual BaseTemplate* get_template(const std::string& template_path) = 0;

	/// Initializes a std::vector of directories to search for templates.
	virtual void template_dirs(std::vector<std::string>& dirs, bool use_app_dirs);
};

__RENDER_END__
