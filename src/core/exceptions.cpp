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
 * An implementation of core/exceptions.h
 */

#include "./exceptions.h"

// C++ libraries.
#include <csignal>


__CORE_BEGIN__

// BaseException
BaseException::BaseException(const char *message, int line, const char *function, const char *file, const char* exceptionType)
	: _message(message), _line(line), _function(function), _file(file), _exception_type(exceptionType)
{
	this->init();
}

BaseException::BaseException(const char* message, int line, const char* function, const char* file)
	: BaseException(message, line, function, file, "BaseException")
{
}

void BaseException::init()
{
	this->_full_message = this->_exception_type + ": " + std::string(this->_message);
}

const char* BaseException::what() const noexcept
{
	return this->_full_message.c_str();
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


// ErrorResponseException
ErrorResponseException::ErrorResponseException(
	short int status_code,
	const char* message,
	int line,
	const char* function,
	const char* file,
	const char* type
)
	: HttpError(message, line, function, file, type)
{
	this->_status_code = status_code;
}

ErrorResponseException::ErrorResponseException(
	short int status_code,
	const char* message,
	int line,
	const char* function,
	const char* file
)
	: ErrorResponseException(status_code, message, line, function, file, "ErrorResponseException")
{
}

ErrorResponseException::ErrorResponseException(
	short int status_code,
	const std::string& message,
	int line,
	const char *function,
	const char *file
)
	: ErrorResponseException(status_code, message.c_str(), line, function, file)
{
}

short int ErrorResponseException::status_code() const
{
	return this->_status_code;
}


// InterruptException
InterruptException::InterruptException(
	const char* message, int line, const char* function, const char* file, const char* type
) : BaseException(message, line, function, file, type)
{
}

InterruptException::InterruptException(const char* message, int line, const char* function, const char* file)
	: InterruptException(message, line, function, file, "InterruptException")
{
}

InterruptException::InterruptException(
	const std::string& message, int line, const char *function, const char *file
) : InterruptException(message.c_str(), line, function, file)
{
}

void InterruptException::handle_signal(int sig)
{
	throw InterruptException("execution is interrupted with signal " + std::to_string(sig));
}

void InterruptException::initialize()
{
	struct sigaction sig_int_handler{};
	sig_int_handler.sa_handler = InterruptException::handle_signal;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_int_handler, nullptr);
	sigaction(SIGTERM, &sig_int_handler, nullptr);
//	sigaction(SIGKILL, &sig_int_handler, nullptr);
}

__CORE_END__
