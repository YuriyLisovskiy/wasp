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

#include "./exceptions.h"


__CORE_BEGIN__


// BaseException
BaseException::BaseException(const char *message, int line, const char *function, const char *file, const char* exceptionType)
	: _message(message), _line(line), _function(function), _file(file), _exceptionType(exceptionType)
{
	this->init();
}

BaseException::BaseException(const char* message, int line, const char* function, const char* file)
	: BaseException(message, line, function, file, "BaseException")
{
}

void BaseException::init()
{
	this->_fullMessage = this->_exceptionType + ": " + std::string(this->_message);
}

const char* BaseException::what() const noexcept
{
	return this->_fullMessage.c_str();
}

int BaseException::line() const noexcept
{
	return this->_line;
}

const char* BaseException::function() const noexcept
{
	return this->_function;
}

const char* BaseException::file() const noexcept
{
	return this->_file;
}


// SocketError
SocketError::SocketError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

SocketError::SocketError(const char* message, int line, const char* function, const char* file)
	: SocketError(message, line, function, file, "SocketError")
{
}

SocketError::SocketError(const std::string& message, int line, const char *function, const char *file)
	: SocketError(message.c_str(), line, function, file)
{
}


// HttpError
HttpError::HttpError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
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


// ParseError
ParseError::ParseError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

ParseError::ParseError(const char* message, int line, const char* function, const char* file)
	: ParseError(message, line, function, file, "ParseError")
{
}

ParseError::ParseError(const std::string& message, int line, const char *function, const char *file)
	: ParseError(message.c_str(), line, function, file)
{
}


// MultiPartParserError
MultiPartParserError::MultiPartParserError(const char* message, int line, const char* function, const char* file, const char* type)
	: ParseError(message, line, function, file, type)
{
}

MultiPartParserError::MultiPartParserError(const char* message, int line, const char* function, const char* file)
	: MultiPartParserError(message, line, function, file, "MultiPartParserError")
{
}

MultiPartParserError::MultiPartParserError(const std::string& message, int line, const char *function, const char *file)
	: MultiPartParserError(message.c_str(), line, function, file)
{
}


// DictError
DictError::DictError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
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


// NullPointerException
NullPointerException::NullPointerException(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

NullPointerException::NullPointerException(const char* message, int line, const char* function, const char* file)
	: NullPointerException(message, line, function, file, "NullPointerException")
{
}

NullPointerException::NullPointerException(const std::string& message, int line, const char *function, const char *file)
	: NullPointerException(message.c_str(), line, function, file)
{
}


// ValueError
ValueError::ValueError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
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


// AttributeError
AttributeError::AttributeError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

AttributeError::AttributeError(const char* message, int line, const char* function, const char* file)
	: AttributeError(message, line, function, file, "AttributeError")
{
}

AttributeError::AttributeError(const std::string& message, int line, const char *function, const char *file)
	: AttributeError(message.c_str(), line, function, file)
{
}


// FileDoesNotExistError
FileDoesNotExistError::FileDoesNotExistError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

FileDoesNotExistError::FileDoesNotExistError(const char* message, int line, const char* function, const char* file)
	: FileDoesNotExistError(message, line, function, file, "FileDoesNotExistError")
{
}

FileDoesNotExistError::FileDoesNotExistError(const std::string& message, int line, const char *function, const char *file)
	: FileDoesNotExistError(message.c_str(), line, function, file)
{
}


// ImproperlyConfigured
ImproperlyConfigured::ImproperlyConfigured(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

ImproperlyConfigured::ImproperlyConfigured(const char* message, int line, const char* function, const char* file)
	: ImproperlyConfigured(message, line, function, file, "ImproperlyConfigured")
{
}

ImproperlyConfigured::ImproperlyConfigured(const std::string& message, int line, const char *function, const char *file)
	: ImproperlyConfigured(message.c_str(), line, function, file)
{
}


// FileError
FileError::FileError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

FileError::FileError(const char* message, int line, const char* function, const char* file)
	: FileError(message, line, function, file, "FileError")
{
}

FileError::FileError(const std::string& message, int line, const char *function, const char *file)
	: FileError(message.c_str(), line, function, file)
{
}


// EncodingError
EncodingError::EncodingError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

EncodingError::EncodingError(const char* message, int line, const char* function, const char* file)
	: EncodingError(message, line, function, file, "EncodingError")
{
}

EncodingError::EncodingError(const std::string& message, int line, const char *function, const char *file)
	: EncodingError(message.c_str(), line, function, file)
{
}


// SuspiciousOperation
SuspiciousOperation::SuspiciousOperation(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

SuspiciousOperation::SuspiciousOperation(const char* message, int line, const char* function, const char* file)
	: SuspiciousOperation(message, line, function, file, "SuspiciousOperation")
{
}

SuspiciousOperation::SuspiciousOperation(const std::string& message, int line, const char *function, const char *file)
	: SuspiciousOperation(message.c_str(), line, function, file)
{
}


// DisallowedRedirect
DisallowedRedirect::DisallowedRedirect(const char* message, int line, const char* function, const char* file, const char* type)
	: SuspiciousOperation(message, line, function, file, type)
{
}

DisallowedRedirect::DisallowedRedirect(const char* message, int line, const char* function, const char* file)
	: DisallowedRedirect(message, line, function, file, "DisallowedRedirect")
{
}

DisallowedRedirect::DisallowedRedirect(const std::string& message, int line, const char *function, const char *file)
	: DisallowedRedirect(message.c_str(), line, function, file)
{
}


// EntityTooLargeError
EntityTooLargeError::EntityTooLargeError(const char* message, int line, const char* function, const char* file, const char* type)
		: BaseException(message, line, function, file, type)
{
}

EntityTooLargeError::EntityTooLargeError(const char* message, int line, const char* function, const char* file)
		: EntityTooLargeError(message, line, function, file, "EntityTooLargeError")
{
}

EntityTooLargeError::EntityTooLargeError(const std::string& message, int line, const char *function, const char *file)
		: EntityTooLargeError(message.c_str(), line, function, file)
{
}


// InterruptException
InterruptException::InterruptException(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

InterruptException::InterruptException(const char* message, int line, const char* function, const char* file)
	: InterruptException(message, line, function, file, "InterruptException")
{
}

InterruptException::InterruptException(const std::string& message, int line, const char *function, const char *file)
	: InterruptException(message.c_str(), line, function, file)
{
}

void InterruptException::handle_signal(int sig)
{
	throw InterruptException("execution is interrupted with signal " + std::to_string(sig), _DETAILS_NONE_);
}

void InterruptException::initialize()
{
	struct sigaction sig_int_handler{};
	sig_int_handler.sa_handler = InterruptException::handle_signal;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_int_handler, nullptr);
	sigaction(SIGTERM, &sig_int_handler, nullptr);
}

__CORE_END__
