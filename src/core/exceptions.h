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
 * exceptions definition.
 * TODO: write docs.
 */

#ifndef WASP_CORE_EXCEPTIONS_H
#define WASP_CORE_EXCEPTIONS_H

#include <exception>
#include <string>

#include "../globals.h"


__WASP_BEGIN__

class WaspException : public std::exception
{
protected:
	const char* _exceptionType;
	const char* _message;
	int _line;
	const char* _function;
	const char* _file;

public:
	WaspException(const char* message, int line, const char* function, const char* file);

	const char* what() const noexcept override;
	virtual const int line() const noexcept;
	virtual const char* function() const noexcept;
	virtual const char* file() const noexcept;
};

class WaspHttpError : public WaspException
{
public:
	WaspHttpError(const char* message, int line, const char* function, const char* file);
	WaspHttpError(const std::string& message, int line, const char* function, const char* file);
};

class QueryDictError : public WaspException
{
public:
	QueryDictError(const char* message, int line, const char* function, const char* file);
	QueryDictError(const std::string& message, int line, const char* function, const char* file);
};

class CookieError : public WaspException
{
public:
	CookieError(const char* message, int line, const char* function, const char* file);
	CookieError(const std::string& message, int line, const char* function, const char* file);
};

class ValueError : public WaspException
{
public:
	ValueError(const char* message, int line, const char* function, const char* file);
	ValueError(const std::string& message, int line, const char* function, const char* file);
};

__WASP_END__


#endif // WASP_CORE_EXCEPTIONS_H
