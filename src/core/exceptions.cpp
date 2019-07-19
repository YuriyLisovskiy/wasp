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


// Base Wasp exception
WaspException::WaspException(const char *message, int line, const char *function, const char *file)
	: _message(message), _line(line), _function(function), _file(file)
{
	this->_exceptionType = "WaspException";
}

const char* WaspException::what() const noexcept
{
	return (std::string(this->_exceptionType) + ": " + std::string(this->_message)).c_str();
}

const int WaspException::line() const noexcept
{
	return this->_line;
}

const char* WaspException::function() const noexcept
{
	return this->_function;
}

const char* WaspException::file() const noexcept
{
	return this->_file;
}

// WaspHttpError
WaspHttpError::WaspHttpError(const char* message, int line, const char* function, const char* file)
	: WaspException(message, line, function, file)
{
	this->_exceptionType = "WaspHttpError";
}

WaspHttpError::WaspHttpError(const std::string& message, int line, const char *function, const char *file)
	: WaspHttpError(message.c_str(), line, function, file)
{
}

// QueryDictError
QueryDictError::QueryDictError(const char* message, int line, const char* function, const char* file)
	: WaspException(message, line, function, file)
{
	this->_exceptionType = "QueryDictError";
}

QueryDictError::QueryDictError(const std::string& message, int line, const char *function, const char *file)
	: QueryDictError(message.c_str(), line, function, file)
{
}

// CookieError
CookieError::CookieError(const char* message, int line, const char* function, const char* file)
	: WaspException(message, line, function, file)
{
	this->_exceptionType = "CookieError";
}

CookieError::CookieError(const std::string& message, int line, const char *function, const char *file)
	: CookieError(message.c_str(), line, function, file)
{
}

// ValueError
ValueError::ValueError(const char* message, int line, const char* function, const char* file)
	: WaspException(message, line, function, file)
{
	this->_exceptionType = "ValueError";
}

ValueError::ValueError(const std::string& message, int line, const char *function, const char *file)
	: ValueError(message.c_str(), line, function, file)
{
}

__WASP_END__
