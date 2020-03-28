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
 * views/view.h
 *
 * Purpose:
 * Intentionally simple parent class for all views. Only
 * implements dispatch-by-method and simple sanity checking.
 */

#pragma once

// C++ libraries.
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./args.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../core/logger.h"
#include "../core/string/str.h"
#include "../core/exceptions.h"
#include "../collections/dict.h"
#include "../conf/settings.h"


__CONF_BEGIN__

class Settings;

__CONF_END__


__VIEWS_BEGIN__

typedef std::function<std::unique_ptr<http::IHttpResponse>(
	http::HttpRequest*, views::Args*, conf::Settings*
)> ViewHandler;


class View
{
protected:
	core::ILogger* _logger;

	conf::Settings* _settings;

	/// Holds pointer to client's request.
	/// Caution: must be deleted outside!
	http::HttpRequest* _request;

	/// Contains all possible http methods which view can handle.
	const std::vector<std::string> _http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	/// List of methods witch will be returned when OPTIONS is request.
	std::vector<std::string> _allowed_methods_list;

public:
	explicit View(conf::Settings* settings);

	/// Processes http GET request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> get(http::HttpRequest* request, Args* args);

	/// Processes http POST request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> post(http::HttpRequest* request, Args* args);

	/// Processes http PUT request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> put(http::HttpRequest* request, Args* args);

	/// Processes http PATCH request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> patch(http::HttpRequest* request, Args* args);

	/// Processes http DELETE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> delete_(http::HttpRequest* request, Args* args);

	/// Processes http HEAD request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> head(http::HttpRequest* request, Args* args);

	/// Processes http OPTIONS request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> options(http::HttpRequest* request, Args* args);

	/// Processes http TRACE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual std::unique_ptr<http::IHttpResponse> trace(http::HttpRequest* request, Args* args);

	/// Setups request before dispatch call.
	/// Can be overridden in derived class, but requires
	///		request initialization.
	///
	/// @param request: pointer to http request.
	virtual void setup(http::HttpRequest* request);

	/// Try to dispatch to the right method; if a method doesn't exist,
	/// defer to the error handler. Also defer to the error handler if the
	/// request method isn't on the approved list.
	///
	/// @param request: an actual http request from client.
	/// @return pointer to http response returned from handler.
	virtual std::unique_ptr<http::IHttpResponse> dispatch(Args* args);

	/// Returns Http 405 (Method Not Allowed) response.
	///
	/// @param request: pointer to http request.
	/// @return pointer to http response returned from handler.
	std::unique_ptr<http::IHttpResponse> http_method_not_allowed(http::HttpRequest* request);

	/// Builds vector of allowed methods.
	/// Used for http OPTIONS response.
	/// To make this method return correct allowed methods,
	///		pass a vector of allowed methods names to protected
	///		constructor in derived class initialization.
	///
	/// @return std::vector of strings.
	std::vector<std::string> allowed_methods();

protected:
	explicit View(
		const std::vector<std::string>& allowed_methods,
		conf::Settings* settings
	);
};

__VIEWS_END__
