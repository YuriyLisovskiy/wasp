#include <utility>

/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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

/*
 * exceptions implementation.
 * TODO: write docs.
 */

#include "exceptions.h"


__WASP_BEGIN__


// Base WaspError
WaspError::WaspError(const char* message, int line, const char* function, const char* file)
	: _message(message), _line(line), _function(function), _file(file), _exceptionType("WaspError")
{
	this->init();
}

WaspError::WaspError(const char *message, int line, const char *function, const char *file, const char* exceptionType)
	: _message(message), _line(line), _function(function), _file(file), _exceptionType(exceptionType)
{
	this->init();
}

void WaspError::init()
{
	this->_fullMessage = this->_exceptionType + ": " + std::string(this->_message);
}

const char* WaspError::what() const noexcept
{
	return this->_fullMessage.c_str();
}

const int WaspError::line() const noexcept
{
	return this->_line;
}

const char* WaspError::function() const noexcept
{
	return this->_function;
}

const char* WaspError::file() const noexcept
{
	return this->_file;
}

// HttpError
HttpError::HttpError(const char* message, int line, const char* function, const char* file, const char* type)
	: WaspError(message, line, function, file, type)
{
}

HttpError::HttpError(const char* message, int line, const char* function, const char* file)
	: HttpError(message, line, function, file, "HttpError")
{
}

HttpError::HttpError(const std::string& message, int line, const char *function, const char *file)
	: HttpError(message.c_str(), line, function, file)
{
}

// DictError
DictError::DictError(const char* message, int line, const char* function, const char* file, const char* type)
	: WaspError(message, line, function, file, type)
{
}

DictError::DictError(const char* message, int line, const char* function, const char* file)
	: DictError(message, line, function, file, "DictError")
{
}

	DictError::DictError(const std::string& message, int line, const char *function, const char *file)
	: DictError(message.c_str(), line, function, file)
{
}

// MultiValueDictError
MultiValueDictError::MultiValueDictError(const char* message, int line, const char* function, const char* file, const char* type)
	: DictError(message, line, function, file, type)
{
}

MultiValueDictError::MultiValueDictError(const char* message, int line, const char* function, const char* file)
	: MultiValueDictError(message, line, function, file, "MultiValueDictError")
{
}

MultiValueDictError::MultiValueDictError(const std::string& message, int line, const char *function, const char *file)
	: MultiValueDictError(message.c_str(), line, function, file)
{
}

// CookieError
CookieError::CookieError(const char* message, int line, const char* function, const char* file, const char* type)
	: WaspError(message, line, function, file, type)
{
}

CookieError::CookieError(const char* message, int line, const char* function, const char* file)
	: CookieError(message, line, function, file, "CookieError")
{
}

CookieError::CookieError(const std::string& message, int line, const char *function, const char *file)
	: CookieError(message.c_str(), line, function, file)
{
}

// ValueError
ValueError::ValueError(const char* message, int line, const char* function, const char* file, const char* type)
	: WaspError(message, line, function, file, type)
{
}

ValueError::ValueError(const char* message, int line, const char* function, const char* file)
	: ValueError(message, line, function, file, "ValueError")
{
}

ValueError::ValueError(const std::string& message, int line, const char *function, const char *file)
	: ValueError(message.c_str(), line, function, file)
{
}

__WASP_END__
