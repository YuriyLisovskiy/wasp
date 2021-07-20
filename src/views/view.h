/**
 * views/view.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Intentionally simple parent class for all views. Only
 * 	implements dispatch-by-method and simple sanity checking.
 */

#pragma once

// C++ libraries.
#include <memory>
#include <functional>

// Base libraries.
#include <xalwart.base/logger.h>
#include <xalwart.base/result.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./args.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/_def_.h"


__CONF_BEGIN__

struct Settings;

__CONF_END__


__VIEWS_BEGIN__

typedef std::function<Result<std::shared_ptr<http::IHttpResponse>>(
	http::HttpRequest*, views::Args*, conf::Settings*
)> ViewHandler;

class View
{
private:
	Result<std::shared_ptr<http::IHttpResponse>> null();

protected:
	log::ILogger* logger;

	conf::Settings* settings;

	/// Holds pointer to client's request.
	/// Caution: must be deleted outside!
	http::HttpRequest* request;

	/// Contains all possible http methods which view can handle.
	const std::vector<std::string> http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	/// List of methods witch will be returned when OPTIONS is request.
	std::vector<std::string> allowed_methods_list;

protected:
	explicit View(
		const std::vector<std::string>& allowed_methods,
		conf::Settings* settings
	);

	template <typename ResponseT, typename ...Args>
	Result<std::shared_ptr<http::IHttpResponse>> response(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<ResponseT>(std::forward<Args>(args)...)
		);
	}

	template <unsigned short StatusCode, typename ...Args>
	Result<std::shared_ptr<http::IHttpResponse>> response(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<http::HttpResponse>(StatusCode, std::forward<Args>(args)...)
		);
	}

	template <typename ...Args>
	Result<std::shared_ptr<http::IHttpResponse>> response(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(std::forward<Args>(args)...);
	}

	template<error_type ErrorType, typename ...Args>
	Result<std::shared_ptr<http::IHttpResponse>> raise(Args&& ...args)
	{
		return raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(
			std::forward<Args>(args)...
		);
	}

public:
	explicit View(conf::Settings* settings);

	/// Processes http GET request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> get(http::HttpRequest* request, Args* args);

	/// Processes http POST request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> post(http::HttpRequest* request, Args* args);

	/// Processes http PUT request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> put(http::HttpRequest* request, Args* args);

	/// Processes http PATCH request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> patch(http::HttpRequest* request, Args* args);

	/// Processes http DELETE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> delete_(http::HttpRequest* request, Args* args);

	/// Processes http HEAD request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> head(http::HttpRequest* request, Args* args);

	/// Processes http OPTIONS request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> options(http::HttpRequest* request, Args* args);

	/// Processes http TRACE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> trace(http::HttpRequest* request, Args* args);

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
	virtual Result<std::shared_ptr<http::IHttpResponse>> dispatch(Args* args);

	/// Returns Http 405 (Method Not Allowed) response.
	///
	/// @param request: pointer to http request.
	/// @return pointer to http response returned from handler.
	Result<std::shared_ptr<http::IHttpResponse>> http_method_not_allowed(http::HttpRequest* request);

	/// Builds vector of allowed methods.
	/// Used for http OPTIONS response.
	/// To make this method return correct allowed methods,
	///		pass a vector of allowed methods names to protected
	///		constructor in derived class initialization.
	///
	/// @return std::vector of strings.
	std::vector<std::string> allowed_methods();
};

__VIEWS_END__
