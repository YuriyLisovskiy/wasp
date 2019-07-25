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

const int BaseException::line() const noexcept
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


// TcpError
TcpError::TcpError(const char* message, int line, const char* function, const char* file, const char* type)
	: BaseException(message, line, function, file, type)
{
}

TcpError::TcpError(const char* message, int line, const char* function, const char* file)
	: TcpError(message, line, function, file, "TcpError")
{
}

TcpError::TcpError(const std::string& message, int line, const char *function, const char *file)
	: TcpError(message.c_str(), line, function, file)
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

__WASP_END__
