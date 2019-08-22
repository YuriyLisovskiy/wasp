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
 * http request implementation.
 * TODO: write docs
 */

#include <iostream>

#include "request.h"


__WASP_BEGIN__

HttpRequest::HttpRequest(
	std::string method, std::string path, size_t majorV, size_t minorV,
	std::string query, bool keepAlive, std::string content,
	const std::map<std::string, std::string>& headers,
	const std::map<std::string, std::string>& getParameters,
	const std::map<std::string, std::string>& postParameters
)
:   _method(std::move(method)),
	_path(std::move(path)),
	_majorVersion(majorV),
	_minorVersion(minorV),
	_query(std::move(query)),
	_keepAlive(keepAlive),
	_body(std::move(content))
{
	this->headers = Dict<std::string, std::string>(headers);
	this->GET = Dict<std::string, std::string>(getParameters);
	this->POST = Dict<std::string, std::string>(postParameters);
}

std::string HttpRequest::version()
{
	return std::to_string(this->_majorVersion) + "." + std::to_string(this->_minorVersion);
}

std::string HttpRequest::path()
{
	return this->_path;
}

std::string HttpRequest::method()
{
	return this->_method;
}

std::string HttpRequest::body()
{
	return this->_body;
}

__WASP_END__
