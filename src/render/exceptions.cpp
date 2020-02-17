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
 * An implementation of render/exceptions.h
 */

#include "./exceptions.h"


__RENDER_BEGIN__

// TemplateDoesNotExist
TemplateDoesNotExist::TemplateDoesNotExist(
	const char* message, int line, const char* function, const char* file, const char* type
) : BaseException(message, line, function, file, type)
{
	this->_tried = {};
	this->_backend = nullptr;
}

TemplateDoesNotExist::TemplateDoesNotExist(
	const char* message,
	const std::vector<std::string>& tried,
	backends::BaseBackend* backend,
	int line, const char* function, const char* file
)
	: TemplateDoesNotExist(message, line, function, file, "TemplateDoesNotExist")
{
	this->_tried = tried;
	this->_backend = backend;
}

TemplateDoesNotExist::TemplateDoesNotExist(
	const std::string& message,
	const std::vector<std::string>& tried,
	backends::BaseBackend* backend,
	int line, const char *function, const char *file
) : TemplateDoesNotExist(message.c_str(), tried, backend, line, function, file)
{
}

std::vector<std::string> TemplateDoesNotExist::tried() const
{
	return this->_tried;
}

backends::BaseBackend* TemplateDoesNotExist::backend() const
{
	return this->_backend;
}


// TemplateSyntaxError
TemplateSyntaxError::TemplateSyntaxError(
	const char* message, int line, const char* function, const char* file, const char* type
) : BaseException(message, line, function, file, type)
{
}

TemplateSyntaxError::TemplateSyntaxError(
	const char* message,
	internal::token_t& token,
	int line, const char* function, const char* file
)
	: TemplateSyntaxError(message, line, function, file, "TemplateSyntaxError")
{
	this->_token = std::move(token);
}

TemplateSyntaxError::TemplateSyntaxError(
	const std::string& message,
	internal::token_t& token,
	int line, const char *function, const char *file
) : TemplateSyntaxError(message.c_str(), token, line, function, file)
{
}

__RENDER_END__
