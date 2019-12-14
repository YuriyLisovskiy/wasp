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
 * Purpose: Intentionally simple parent class for all views. Only
 * 			implements dispatch-by-method and simple sanity checking.
 */

#ifndef WASP_VIEWS_VIEW_H
#define WASP_VIEWS_VIEW_H

#include <vector>
#include <string>
#include <cxxabi.h>
#include <algorithm>
#include <functional>

#include "../globals.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../utility/str.h"
#include "../utility/logger.h"
#include "../core/exceptions.h"
#include "../collections/dict.h"
#include "args.h"


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

	/// Processes http GET request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* get(HttpRequest* request, Args* args);

	/// Processes http POST request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* post(HttpRequest* request, Args* args);

	/// Processes http PUT request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* put(HttpRequest* request, Args* args);

	/// Processes http PATCH request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* patch(HttpRequest* request, Args* args);

	/// Processes http DELETE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* delete_(HttpRequest* request, Args* args);

	/// Processes http HEAD request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* head(HttpRequest* request, Args* args);

	/// Processes http OPTIONS request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* options(HttpRequest* request, Args* args);

	/// Processes http TRACE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual HttpResponse* trace(HttpRequest* request, Args* args);

	/// Setups request before dispatch call.
	/// Can be overridden in derived class, but requires
	///		request initialization.
	///
	/// @param request: pointer to http request.
	virtual void setup(HttpRequest* request);

	/// Try to dispatch to the right method; if a method doesn't exist,
	/// defer to the error handler. Also defer to the error handler if the
	/// request method isn't on the approved list.
	///
	/// @param request: an actual http request from client.
	/// @return pointer to http response returned from handler.
	virtual HttpResponse* dispatch(Args* args);

	/// Returns Http 405 (Method Not Allowed) response.
	///
	/// @param request: pointer to http request.
	/// @return pointer to http response returned from handler.
	HttpResponse* http_method_not_allowed(HttpRequest* request);

	/// Builds vector of allowed methods.
	/// Used for http OPTIONS response.
	/// To make this method return correct allowed methods,
	///		pass a vector of allowed methods names to protected
	///		constructor in derived class initialization.
	///
	/// @return std::vector of strings.
	std::vector<std::string> allowed_methods();

	/// An entry point for a request-response process.
	///
	/// @tparam _ViewT: View or it's derived class. Class definition
	/// 	must contains constructor with the next parameter:
	/// 	wasp::ILogger* logger = nullptr
	/// @return std::function which accepts request and url arguments,
	/// 	and returns pointer to HttpResponse instance.
	template <typename _ViewT>
	static std::function<HttpResponse*(HttpRequest*, Args*, ILogger*)> make_view()
	{
		auto func = [](HttpRequest* request, Args* args, ILogger* logger) -> HttpResponse*
		{
			_ViewT view(logger);
			view.setup(request);
			return view.dispatch(args);
		};

		return func;
	}

protected:
	explicit View(const std::vector<std::string>& allowed_methods, ILogger* logger = nullptr);
};

__WASP_END__


#endif // WASP_VIEWS_VIEW_H
