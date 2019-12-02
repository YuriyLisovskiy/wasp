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
 * view.h
 * Purpose: TODO:
 */

#ifndef WASP_VIEWS_VIEW_H
#define WASP_VIEWS_VIEW_H

#include <vector>
#include <string>
#include <algorithm>

#include "../globals.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../utility/str.h"
#include "../utility/logger.h"
#include "../core/exceptions.h"
#include "../collections/dict.h"


__WASP_BEGIN__

class View
{
protected:
	/// View's logger.
	ILogger* _logger;

	/// Holds pointer to client's request.
	/// Caution: must be deleted outside!
	HttpRequest* _request;

	/// Contains all possible http methods which view can handle.
	const std::vector<std::string> _http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	/// List of methods witch will be returned when OPTIONS is request.
	std::vector<std::string> _allowed_methods_list;

public:
	explicit View(ILogger* logger = nullptr);

	virtual HttpResponse* get(HttpRequest& request);
	virtual HttpResponse* post(HttpRequest& request);
	virtual HttpResponse* put(HttpRequest& request);
	virtual HttpResponse* patch(HttpRequest& request);
	virtual HttpResponse* delete_(HttpRequest& request);
	virtual HttpResponse* head(HttpRequest& request);
	virtual HttpResponse* options(HttpRequest& request);
	virtual HttpResponse* trace(HttpRequest& request);

	virtual void setup(HttpRequest& request);

	/// Try to dispatch to the right method; if a method doesn't exist,
	/// defer to the error handler. Also defer to the error handler if the
	/// request method isn't on the approved list.
	///
	/// @param request: an actual http request from client.
	/// @return pointer to http response returned from handler.
	virtual HttpResponse* dispatch(HttpRequest& request);
	HttpResponse* http_method_not_allowed(HttpRequest& request);
	std::vector<std::string> _allowed_methods();

protected:
	explicit View(const std::vector<std::string>& allowed_methods = {}, ILogger* logger = nullptr);
};

__WASP_END__


#endif // WASP_VIEWS_VIEW_H
