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
 * render/exceptions.h
 *
 * Purpose:
 * 	This module contains generic exceptions used by template backends. Although,
 * 	Wasp template language also internally uses these exceptions, other exceptions
 * 	specific to the DTL should not be added here.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./backends/base.h"
#include "../core/exceptions.h"
#include "./internal/token.h"


__RENDER_BEGIN__

/// The exception used when a template does not exist.
///
/// backend: the template backend class used when raising this exception.
///
/// tried: a list of sources that were tried when finding the template.
class TemplateDoesNotExist : public core::BaseException
{
private:
	std::vector<std::string> _tried;
	backends::BaseBackend* _backend;

protected:
	// Use only when initializing of a derived exception!
	TemplateDoesNotExist(
		const char* message, int line, const char* function, const char* file, const char* type
	);
public:
	explicit TemplateDoesNotExist(
		const char* message,
		const std::vector<std::string>& tried = {},
		backends::BaseBackend* backend = nullptr,
		int line = 0, const char* function = "", const char* file = ""
	);
	explicit TemplateDoesNotExist(
		const std::string& message,
		const std::vector<std::string>& tried = {},
		backends::BaseBackend* backend = nullptr,
		int line = 0, const char* function = "", const char* file = ""
	);
	[[nodiscard]] std::vector<std::string> tried() const;
	[[nodiscard]] backends::BaseBackend* backend() const;
};


/// The exception used for syntax errors during parsing or rendering.
class TemplateSyntaxError : public core::BaseException
{
private:
	internal::token_t _token;

protected:
	// Use only when initializing of a derived exception!
	TemplateSyntaxError(
		const char* message, int line, const char* function, const char* file, const char* type
	);
public:
	explicit TemplateSyntaxError(
		const char* message,
		const internal::token_t& token,
		int line = 0, const char* function = "", const char* file = ""
	);
	explicit TemplateSyntaxError(
		const std::string& message,
		const internal::token_t& token,
		int line = 0, const char* function = "", const char* file = ""
	);
};

DEF_WASP_EXCEPTION_WITH_BASE(ContentNotRenderedError, core::BaseException, "Content is not rendered");
DEF_WASP_EXCEPTION_WITH_BASE(VariableDoesNotExist, core::BaseException, "Variable does not exist");
DEF_WASP_EXCEPTION_WITH_BASE(FilterDoesNotExist, core::BaseException, "Filter does not exist");

__RENDER_END__
