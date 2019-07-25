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

class BaseException : public std::exception
{
protected:
	std::string _fullMessage;
	std::string _exceptionType;
	const char* _message;
	int _line;
	const char* _function;
	const char* _file;

	// Use only when initializing of a derived exception!
	BaseException(const char* message, int line, const char* function, const char* file, const char* type);

	// Initializes '_fullMessage' member.
	//
	// Calls only in constructor.
	void init();

public:
	BaseException(const char* message, int line, const char* function, const char* file);
	~BaseException() noexcept override = default;

	const char* what() const noexcept override;
	virtual const int line() const noexcept;
	virtual const char* function() const noexcept;
	virtual const char* file() const noexcept;
};


class WaspError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	WaspError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	WaspError(const char* message, int line, const char* function, const char* file);
	WaspError(const std::string& message, int line, const char* function, const char* file);
};


class HttpError : public WaspError
{
protected:
	// Use only when initializing of a derived exception!
	HttpError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	HttpError(const char* message, int line, const char* function, const char* file);
	HttpError(const std::string& message, int line, const char* function, const char* file);
};


class DictError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	DictError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	DictError(const char* message, int line, const char* function, const char* file);
	DictError(const std::string& message, int line, const char* function, const char* file);
};


class MultiValueDictError : public DictError
{
protected:
	// Use only when initializing of a derived exception!
	MultiValueDictError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	MultiValueDictError(const char* message, int line, const char* function, const char* file);
	MultiValueDictError(const std::string& message, int line, const char* function, const char* file);
};


class CookieError : public WaspError
{
protected:
	// Use only when initializing of a derived exception!
	CookieError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	CookieError(const char* message, int line, const char* function, const char* file);
	CookieError(const std::string& message, int line, const char* function, const char* file);
};


class ValueError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	ValueError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	ValueError(const char* message, int line, const char* function, const char* file);
	ValueError(const std::string& message, int line, const char* function, const char* file);
};


class TcpError : public WaspError
{
protected:
	// Use only when initializing of a derived exception!
	TcpError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	TcpError(const char* message, int line, const char* function, const char* file);
	TcpError(const std::string& message, int line, const char* function, const char* file);
};


class AttributeError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	AttributeError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	AttributeError(const char* message, int line, const char* function, const char* file);
	AttributeError(const std::string& message, int line, const char* function, const char* file);
};

__WASP_END__


#endif // WASP_CORE_EXCEPTIONS_H
