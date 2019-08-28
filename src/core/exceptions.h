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
 * Exceptions list:
 * - Base:
 *  - BaseException.
 *
 * - Server errors:
 *  - HttpError;
 *  - SocketError;
 *  - TcpError.
 *
 * - Wasp errors:
 *  - AttributeError;
 *  - DictError;
 *  - DisallowedRedirect;
 *  - EncodingError;
 *  - FileDoesNotExistError;
 *  - MultiValueDictError;
 *  - ParseError;
 *  - SuspiciousOperation;
 *  - ValueError.
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

	[[nodiscard]] const char* what() const noexcept override;
	[[nodiscard]] virtual int line() const noexcept;
	[[nodiscard]] virtual const char* function() const noexcept;
	[[nodiscard]] virtual const char* file() const noexcept;
};


class SocketError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	SocketError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	SocketError(const char* message, int line, const char* function, const char* file);
	SocketError(const std::string& message, int line, const char* function, const char* file);
};


class HttpError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	HttpError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	HttpError(const char* message, int line, const char* function, const char* file);
	HttpError(const std::string& message, int line, const char* function, const char* file);
};

class ParseError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	ParseError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	ParseError(const char* message, int line, const char* function, const char* file);
	ParseError(const std::string& message, int line, const char* function, const char* file);
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


class ValueError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	ValueError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	ValueError(const char* message, int line, const char* function, const char* file);
	ValueError(const std::string& message, int line, const char* function, const char* file);
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


class FileDoesNotExistError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	FileDoesNotExistError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	FileDoesNotExistError(const char* message, int line, const char* function, const char* file);
	FileDoesNotExistError(const std::string& message, int line, const char* function, const char* file);
};


class EncodingError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	EncodingError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	EncodingError(const char* message, int line, const char* function, const char* file);
	EncodingError(const std::string& message, int line, const char* function, const char* file);
};


// The user did something suspicious
class SuspiciousOperation : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	SuspiciousOperation(const char* message, int line, const char* function, const char* file, const char* type);
public:
	SuspiciousOperation(const char* message, int line, const char* function, const char* file);
	SuspiciousOperation(const std::string& message, int line, const char* function, const char* file);
};


// Length of request's header is too large.
class EntityTooLargeError : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	EntityTooLargeError(const char* message, int line, const char* function, const char* file, const char* type);
public:
	EntityTooLargeError(const char* message, int line, const char* function, const char* file);
	EntityTooLargeError(const std::string& message, int line, const char* function, const char* file);
};


// Redirect to scheme not in allowed list
class DisallowedRedirect : public SuspiciousOperation
{
protected:
	// Use only when initializing of a derived exception!
	DisallowedRedirect(const char* message, int line, const char* function, const char* file, const char* type);
public:
	DisallowedRedirect(const char* message, int line, const char* function, const char* file);
	DisallowedRedirect(const std::string& message, int line, const char* function, const char* file);
};

__WASP_END__


#endif // WASP_CORE_EXCEPTIONS_H
