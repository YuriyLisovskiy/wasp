/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/exceptions.h
 *
 * - BaseException - main exception class

 * - AttributeError
 * - BadSignature
 * - CommandError
 * - DictError
 * - DisallowedHost
 * - DisallowedRedirect
 * - EntityTooLargeError
 * - EncodingError
 * - ErrorResponseException
 * - FileDoesNotExistError
 * - FileError
 * - HttpError
 * - ImproperlyConfigured
 * - InterruptException
 * - MultiPartParserError
 * - MultiValueDictError
 * - NotImplementedException
 * - NullPointerException
 * - ParseError
 * - PermissionDenied
 * - RuntimeError
 * - SocketError
 * - SuspiciousOperation
 * - ValueError
 */

#pragma once

// C++ libraries.
#include <exception>
#include <string>

// Module definitions.
#include "./_def_.h"


__CORE_BEGIN__

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

	/// Initializes '_fullMessage' member.
	///
	/// Calls only in constructor.
	void init();

public:
	BaseException(const char* message, int line, const char* function, const char* file);
	~BaseException() noexcept override = default;

	[[nodiscard]] const char* what() const noexcept override;
	[[nodiscard]] virtual int line() const noexcept;
	[[nodiscard]] virtual const char* function() const noexcept;
	[[nodiscard]] virtual const char* file() const noexcept;
};


DEF_EXCEPTION_WITH_BASE(AttributeError, BaseException, "Attribute Error");
DEF_EXCEPTION_WITH_BASE(BadSignature, BaseException, "Bad Signature Error");
DEF_EXCEPTION_WITH_BASE(CommandError, BaseException, "Command Error");
DEF_EXCEPTION_WITH_BASE(DictError, BaseException, "Dict Error");
DEF_EXCEPTION_WITH_BASE(HttpError, BaseException, "Http Error");

class ErrorResponseException : public HttpError
{
protected:
	short int _status_code;

	// Use only when initializing of a derived exception!
	ErrorResponseException(
		short int status_code,
		const char* message,
		int line,
		const char* function,
		const char* file,
		const char* type
	);

public:
	explicit ErrorResponseException(
		short int status_code = -1,
		const char* message = "Error Response Exception",
		int line = 0,
		const char* function = "",
		const char* file = ""
	);
	explicit ErrorResponseException(
		short int status_code = -1,
		const std::string& message = "Error Response Exception",
		int line = 0,
		const char* function = "",
		const char* file = ""
	);
	[[nodiscard]] short int status_code() const;
};

// The user did something suspicious.
DEF_HTTP_EXCEPTION(SuspiciousOperation, 400, "Suspicious Operation");

// Length of request's header is too large.
DEF_HTTP_EXCEPTION(EntityTooLargeError, 413, "Entity Too Large");
DEF_HTTP_EXCEPTION(FileDoesNotExistError, 404, "File Does Not Exist");
DEF_HTTP_EXCEPTION(PermissionDenied, 403, "Permission Denied");

DEF_EXCEPTION_WITH_BASE(DisallowedHost, SuspiciousOperation, "Disallowed Host");

// Redirect to scheme not in allowed list.
DEF_EXCEPTION_WITH_BASE(DisallowedRedirect, SuspiciousOperation, "Disallowed Redirect");
DEF_EXCEPTION_WITH_BASE(EncodingError, BaseException, "Encoding Error");
DEF_EXCEPTION_WITH_BASE(FileError, BaseException, "File Error");
DEF_EXCEPTION_WITH_BASE(ImproperlyConfigured, BaseException, "Improperly Configured");

class InterruptException : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	InterruptException(
		const char* message, int line, const char* function, const char* file, const char* type
	);

	static void handle_signal(int sig);
public:
	explicit InterruptException(
		const char* message, int line = 0, const char* function = "", const char* file = ""
	);
	explicit InterruptException(
		const std::string& message, int line = 0, const char* function = "", const char* file = ""
	);
	static void initialize();
};

DEF_EXCEPTION_WITH_BASE(ParseError, BaseException, "Parse Error");
DEF_EXCEPTION_WITH_BASE(RuntimeError, BaseException, "Runtime Error");
DEF_EXCEPTION_WITH_BASE(MultiPartParserError, ParseError, "Multipart Parser Error");
DEF_EXCEPTION_WITH_BASE(MultiValueDictError, DictError, "Multi-Value Dict Error");
DEF_EXCEPTION_WITH_BASE(NotImplementedException, BaseException, "Not Implemented");
DEF_EXCEPTION_WITH_BASE(NullPointerException, BaseException, "Null Pointer Exception");
DEF_EXCEPTION_WITH_BASE(SocketError, BaseException, "Socket Error");
DEF_EXCEPTION_WITH_BASE(ValueError, BaseException, "Value Error");

__CORE_END__
