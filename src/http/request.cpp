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

#include "request.h"
#include "../core/files/uploaded_file.h"


__WASP_BEGIN__

HttpRequest::HttpRequest(
	std::string method, std::string path, size_t major_v, size_t minor_v,
	std::string query, bool keep_alive, std::string content,
	const std::map<std::string, std::string>& headers,
	const HttpRequest::Parameters<std::string, std::string>& get_params,
	const HttpRequest::Parameters<std::string, std::string>& post_params,
	const HttpRequest::Parameters<std::string, UploadedFile>& files_params
)
:   _method(std::move(method)),
	_path(std::move(path)),
	_major_version(major_v),
	_minor_version(minor_v),
	_query(std::move(query)),
	_keep_alive(keep_alive),
	_body(std::move(content))
{
	this->headers = Dict<std::string, std::string>(headers);
	this->GET = get_params;
	this->POST = post_params;
	this->FILES = files_params;
}

std::string HttpRequest::version()
{
	return std::to_string(this->_major_version) + "." + std::to_string(this->_minor_version);
}

std::string HttpRequest::path()
{
	return this->_path;
}

std::string HttpRequest::query()
{
	return this->_query;
}

std::string HttpRequest::method()
{
	return this->_method;
}

bool HttpRequest::keep_alive()
{
	return this->_keep_alive;
}

std::string HttpRequest::body()
{
	return this->_body;
}

__WASP_END__
