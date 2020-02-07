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
 * render/template.h
 *
 * Purpose:
 * 	Renders html template with C++.
 *
 * 	This class is a part of Wasp default render engine and is
 * 	main implementation of rendering process, for custom renderer,
 * 	please, inherit from ITemplate interface and implement
 * 	required methods.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./base.h"


__RENDER_BEGIN__

class Template : public ITemplate
{
protected:
	std::string _template_code;

public:
	Template(const std::string& code, BaseEngine* engine);

	/// Renders template code using given context.
	std::string render(IContext* context) override;
};

__RENDER_END__
