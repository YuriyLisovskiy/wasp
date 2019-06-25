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
 * http request definition.
 * TODO: write docs
 */

#ifndef LEAF_HTTP_REQUEST_H
#define LEAF_HTTP_REQUEST_H

#include <string>
#include <map>

#include "../globals.h"
#include "../utils/query_set.h"


__LEAF_BEGIN__

class HttpRequest
{
private:
	std::string _uri;
	std::string _path;
	std::string _proto;
	std::string _method;
	QuerySet<std::string, std::string> _headers;

	enum Method
	{
		Get, Head, Post, Put, Delete, Connect, Options, Trace
	};

public:
	QuerySet<std::string, std::string> GET;
	QuerySet<std::string, std::string> POST;

	explicit HttpRequest(const std::string& data);
};

__LEAF_END__


#endif // LEAF_HTTP_REQUEST_H
