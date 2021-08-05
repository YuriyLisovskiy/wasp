/**
 * http/exceptions.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * List of http exceptions:
 *  -
 */

#pragma once

// Base libraries.
#include <xalwart.base/exceptions.h>

// C++ libraries.
// TODO

// Module definitions.
#include "./_def_.h"

// Other libraries.
// TODO


__HTTP_BEGIN__

// TESTME: HttpException
// TODO: docs for 'HttpException'
class HttpException : public BaseException
{
private:
	unsigned short int _status_code;

protected:
	// Use only when initializing of a derived exception!
	inline HttpException(
		unsigned short int status_code, const std::string& message,
		int line, const char* function, const char* file, const char* type
	) : BaseException(message.c_str(), line, function, file, type), _status_code(status_code)
	{
	}

public:
	inline HttpException(
		unsigned short int status_code, const std::string& message, int line, const char* function, const char* file
	) : HttpException(status_code, message, line, function, file, "HttpException")
	{
	}

	[[nodiscard]]
	inline unsigned short int status_code() const
	{
		return this->_status_code;
	}

	// Returns error message with exception type as `std::string`.
	[[nodiscard]]
	inline std::string get_message() const noexcept override
	{
		return this->_exception_type + " [" + std::to_string(this->status_code()) + "]: " + this->_message;
	}
};

// TESTME: EntityTooLargeErrorException
// TODO: docs for 'EntityTooLargeErrorException'
class EntityTooLargeErrorException : public HttpException
{
public:
	inline EntityTooLargeErrorException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(413, message, line, function, file, "EntityTooLargeErrorException")
	{
	}
};

// TESTME: DisallowedHostException
// TODO: docs for 'DisallowedHostException'
class DisallowedHostException : public HttpException
{
public:
	inline DisallowedHostException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(400, message, line, function, file, "DisallowedHostException")
	{
	}
};

// TESTME: DisallowedRedirectException
// TODO: docs for 'DisallowedRedirectException'
class DisallowedRedirectException : public HttpException
{
public:
	inline DisallowedRedirectException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(400, message, line, function, file, "DisallowedRedirectException")
	{
	}
};

// TESTME: FileDoesNotExistException
// TODO: docs for 'FileDoesNotExistException'
class FileDoesNotExistException : public HttpException
{
public:
	inline FileDoesNotExistException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(404, message, line, function, file, "FileDoesNotExistException")
	{
	}
};

// TESTME: InternalServerErrorException
// TODO: docs for 'InternalServerErrorException'
class InternalServerErrorException : public HttpException
{
public:
	inline InternalServerErrorException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(500, message, line, function, file, "InternalServerErrorException")
	{
	}
};

// TESTME: NotFoundException
// TODO: docs for 'NotFoundException'
class NotFoundException : public HttpException
{
public:
	inline NotFoundException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(404, message, line, function, file, "NotFoundException")
	{
	}
};

// TESTME: PermissionDeniedException
// TODO: docs for 'PermissionDeniedException'
class PermissionDeniedException : public HttpException
{
public:
	inline PermissionDeniedException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(403, message, line, function, file, "PermissionDeniedException")
	{
	}
};

// TESTME: RequestTimeoutException
// TODO: docs for 'RequestTimeoutException'
class RequestTimeoutException : public HttpException
{
public:
	inline RequestTimeoutException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(408, message, line, function, file, "RequestTimeoutException")
	{
	}
};

// TESTME: SuspiciousOperationException
// TODO: docs for 'SuspiciousOperationException'
class SuspiciousOperationException : public HttpException
{
public:
	inline SuspiciousOperationException(
		const std::string& message, int line, const char* function, const char* file
	) : HttpException(400, message, line, function, file, "SuspiciousOperationException")
	{
	}
};

__HTTP_END__
