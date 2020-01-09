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

/**
 * Available exceptions.
 *
 * - AttributeError;
 * - BaseException.
 * - CommandError;
 * - DictError;
 * - DisallowedRedirect;
 * - EncodingError;
 * - EntityTooLargeError;
 * - FileError;
 * - FileDoesNotExistError;
 * - HttpError;
 * - ImproperlyConfigured;
 * - InterruptException;
 * - MultiPartParserError
 * - MultiValueDictError;
 * - NullPointerException;
 * - ParseError;
 * - SocketError.
 * - SuspiciousOperation;
 * - ValueError.
 */

#pragma once

// C++ libraries.
#include <exception>
#include <string>
#include <csignal>
#include <cstdlib>

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


DEF_WASP_EXCEPTION_WITH_BASE(AttributeError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(CommandError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(DictError, BaseException);

// The user did something suspicious.
DEF_WASP_EXCEPTION_WITH_BASE(SuspiciousOperation, BaseException);

// Redirect to scheme not in allowed list.
DEF_WASP_EXCEPTION_WITH_BASE(DisallowedRedirect, SuspiciousOperation);

DEF_WASP_EXCEPTION_WITH_BASE(EncodingError, BaseException);

// Length of request's header is too large.
DEF_WASP_EXCEPTION_WITH_BASE(EntityTooLargeError, BaseException);

DEF_WASP_EXCEPTION_WITH_BASE(FileError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(FileDoesNotExistError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(HttpError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(ImproperlyConfigured, BaseException);

class InterruptException : public BaseException
{
protected:
	// Use only when initializing of a derived exception!
	InterruptException(const char* message, int line, const char* function, const char* file, const char* type);

	static void handle_signal(int sig);
public:
	explicit InterruptException(const char* message, int line = 0, const char* function = "", const char* file = "");
	explicit InterruptException(const std::string& message, int line = 0, const char* function = "", const char* file = "");
	static void initialize();
};

DEF_WASP_EXCEPTION_WITH_BASE(ParseError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(MultiPartParserError, ParseError);
DEF_WASP_EXCEPTION_WITH_BASE(MultiValueDictError, DictError);
DEF_WASP_EXCEPTION_WITH_BASE(NullPointerException, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(SocketError, BaseException);
DEF_WASP_EXCEPTION_WITH_BASE(ValueError, BaseException);

__CORE_END__
