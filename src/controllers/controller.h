/**
 * controllers/controller.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Intentionally simple parent class for all controllers.
 * Only implements dispatch-by-method and simple sanity checking.
 */

#pragma once

// C++ libraries.
#include <memory>
#include <functional>

// Base libraries.
#include <xalwart.base/kwargs.h>
#include <xalwart.base/result.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/_def_.h"


__CONF_BEGIN__

struct Settings;

__CONF_END__


__CONTROLLERS_BEGIN__

typedef std::function<Result<std::shared_ptr<http::IHttpResponse>>(
	http::HttpRequest*, Kwargs*, conf::Settings*
)> Handler;

class Controller
{
private:
	inline Result<std::shared_ptr<http::IHttpResponse>> null()
	{
		return Result<std::shared_ptr<http::IHttpResponse>>::null();
	}

protected:
	http::HttpRequest* request = nullptr;
	conf::Settings* settings = nullptr;

	/// Contains all possible http methods which controller can handle.
	const std::vector<std::string> http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	/// List of methods witch will be returned when OPTIONS is request.
	std::vector<std::string> allowed_methods_list;

protected:
	explicit Controller(
		const std::vector<std::string>& allowed_methods,
		conf::Settings* settings
	);

	template <typename ResponseT, typename ...Args>
	inline Result<std::shared_ptr<http::IHttpResponse>> response(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<ResponseT>(std::forward<Args>(args)...)
		);
	}

	template <unsigned short StatusCode, typename ...Args>
	inline Result<std::shared_ptr<http::IHttpResponse>> response(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<http::HttpResponse>(StatusCode, std::forward<Args>(args)...)
		);
	}

	template <typename ...Args>
	inline Result<std::shared_ptr<http::IHttpResponse>> response(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(std::forward<Args>(args)...);
	}

	template<error_type ErrorType, typename ...Args>
	inline Result<std::shared_ptr<http::IHttpResponse>> raise(Args&& ...args)
	{
		return raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(
			std::forward<Args>(args)...
		);
	}

public:
	inline explicit Controller(conf::Settings* settings) : allowed_methods_list({"options"}), settings(settings)
	{
		if (!this->settings)
		{
			throw NullPointerException("ctrl::Controller: settings is nullptr", _ERROR_DETAILS_);
		}
	}

	/// Processes http GET request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests' url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> get(Kwargs* args)
	{
		return this->null();
	}

	/// Processes http POST request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests' url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> post(Kwargs* args)
	{
		return this->null();
	}

	/// Processes http PUT request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests' url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> put(Kwargs* args)
	{
		return this->null();
	}

	/// Processes http PATCH request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> patch(Kwargs* args)
	{
		return this->null();
	}

	/// Processes http DELETE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> delete_(Kwargs* args)
	{
		return this->null();
	}

	/// Processes http HEAD request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> head(Kwargs* kwargs)
	{
		return this->get(kwargs);
	}

	/// Processes http OPTIONS request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual Result<std::shared_ptr<http::IHttpResponse>> options(Kwargs* args);

	/// Processes http TRACE request.
	/// Can be overridden in derived class, otherwise returns nullptr.
	///
	/// @param request: pointer to http request.
	/// @param args: pointer to requests's url arguments.
	/// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> trace(Kwargs* args)
	{
		return this->null();
	}

	/// Setups request before dispatch call.
	///
	/// @param request: pointer to http request.
	virtual inline void setup(http::HttpRequest* request)
	{
		this->request = request;
	}

	/// Try to dispatch to the right method; if a method doesn't exist,
	/// defer to the error handler. Also defer to the error handler if the
	/// request method isn't on the approved list.
	///
	/// @param request: an actual http request from client.
	/// @return pointer to http response returned from handler.
	virtual Result<std::shared_ptr<http::IHttpResponse>> dispatch(Kwargs* args);

	/// Returns Http 405 (Method Not Allowed) response.
	///
	/// @param request: pointer to http request.
	/// @return pointer to http response returned from handler.
	Result<std::shared_ptr<http::IHttpResponse>> http_method_not_allowed();

	/// Builds vector of allowed methods.
	/// Used for http OPTIONS response.
	/// To make this method return correct allowed methods,
	///		pass a vector of allowed methods names to protected
	///		constructor in derived class initialization.
	///
	/// @return std::vector of strings.
	std::vector<std::string> allowed_methods();
};

__CONTROLLERS_END__
