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
//#include <xalwart.base/kwargs.h>
#include <xalwart.base/result.h>
#include <xalwart.base/utility.h>
#include <xalwart.base/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/settings.h"


__CONTROLLERS_BEGIN__

template <typename ...ArgsT>
using Handler = std::function<Result<std::shared_ptr<http::IHttpResponse>>(
	http::HttpRequest*, const std::tuple<ArgsT...>&, conf::Settings*
)>;

// TESTME: Controller<...URLArgsT>
// TODO: docs for 'Controller<...URLArgsT>'
template <typename ...URLArgsT>
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

	// Contains all possible http methods which controller can handle.
	const std::vector<std::string> http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	// List of methods witch will be returned when OPTIONS is request.
	std::vector<std::string> allowed_methods_list;

protected:
	inline explicit Controller(
		const std::vector<std::string>& allowed_methods, conf::Settings* settings
	) : Controller<URLArgsT...>(settings)
	{
		this->allowed_methods_list = allowed_methods;
		if (std::find(allowed_methods.begin(), allowed_methods.end(), "options") == allowed_methods.end())
		{
			this->allowed_methods_list.emplace_back("options");
		}
	}

	template <typename ResponseT, typename ...ArgsT>
	inline Result<std::shared_ptr<http::IHttpResponse>> response(ArgsT&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
				std::make_shared<ResponseT>(std::forward<ArgsT>(args)...)
		);
	}

	template <unsigned short StatusCode, typename ...ArgsT>
	inline Result<std::shared_ptr<http::IHttpResponse>> response(ArgsT&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<http::HttpResponse>(StatusCode, std::forward<ArgsT>(args)...)
		);
	}

	template <typename ...ArgsT>
	inline Result<std::shared_ptr<http::IHttpResponse>> response(ArgsT&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(std::forward<ArgsT>(args)...);
	}

	template<error_type ErrorType, typename ...ArgsT>
	inline Result<std::shared_ptr<http::IHttpResponse>> raise(ArgsT&& ...args)
	{
		return raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(std::forward<ArgsT>(args)...);
	}

public:
	inline explicit Controller(conf::Settings* settings) : allowed_methods_list({"options"}), settings(settings)
	{
		if (!this->settings)
		{
			throw NullPointerException("xw::ctrl::Controller: settings is nullptr", _ERROR_DETAILS_);
		}
	}

	// Processes http GET request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> get(URLArgsT ...args)
	{
		return this->null();
	}

	// Processes http POST request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> post(URLArgsT ...args)
	{
		return this->null();
	}

	// Processes http PUT request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> put(URLArgsT ...args)
	{
		return this->null();
	}

	// Processes http PATCH request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests's url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> patch(URLArgsT ...args)
	{
		return this->null();
	}

	// Processes http DELETE request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests's url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> delete_(URLArgsT ...args)
	{
		return this->null();
	}

	// Processes http HEAD request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests's url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> head(URLArgsT ...args)
	{
		return this->get(args...);
	}

	// Processes http OPTIONS request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests's url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> options(URLArgsT ...args)
	{
		auto response = std::make_shared<http::HttpResponse>(200);
		auto allowed_methods = this->allowed_methods();
		response->set_header(
			"Allow",
			str::join(", ", allowed_methods.begin(), allowed_methods.end())
		);
		response->set_header("Content-Length", "0");
		return this->response(response);
	}

	// Processes http TRACE request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests's url arguments.
	// @return pointer to http response instance.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> trace(URLArgsT ...args)
	{
		return this->null();
	}

	// Setups request before dispatch call.
	//
	// @param request: pointer to http request.
	virtual inline void setup(http::HttpRequest* r)
	{
		this->request = r;
	}

	// Try to dispatch to the right method; if a method doesn't exist,
	// defer to the error handler. Also defer to the error handler if the
	// request method isn't on the approved list.
	//
	// @param request: an actual http request from client.
	// @return pointer to http response returned from handler.
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> dispatch(URLArgsT ...args)
	{
		if (this->request == nullptr)
		{
			throw NullPointerException(
				util::demangle(typeid(*this).name()) +
				" instance has not initialized request."
				" Did you override setup() and forget to call base method?",
				_ERROR_DETAILS_
			);
		}

		std::string method = str::lower(this->request->method());
		auto result = Result<std::shared_ptr<http::IHttpResponse>>::null();
		if (method == "get")
		{
			result = this->get(args...);
		}
		else if (method == "post")
		{
			result = this->post(args...);
		}
		else if (method == "put")
		{
			result = this->put(args...);
		}
		else if (method == "patch")
		{
			result = this->patch(args...);
		}
		else if (method == "delete")
		{
			result = this->delete_(args...);
		}
		else if (method == "head")
		{
			result = this->head(args...);
		}
		else if (method == "options")
		{
			result = this->options(args...);
		}
		else if (method == "trace")
		{
			result = this->trace(args...);
		}

		if (!result)
		{
			result = this->http_method_not_allowed();
		}

		return result;
	}

	// Returns Http 405 (Method Not Allowed) response.
	//
	// @param request: pointer to http request.
	// @return pointer to http response returned from handler.
	inline Result<std::shared_ptr<http::IHttpResponse>> http_method_not_allowed()
	{
		if (!this->settings->LOGGER)
		{
			throw NullPointerException("settings->logger is nullptr", _ERROR_DETAILS_);
		}

		this->settings->LOGGER->warning(
			"Method Not Allowed (" + this->request->method() + "): " + this->request->path(),
			_ERROR_DETAILS_
		);
		return this->response<http::HttpResponseNotAllowed>("", this->allowed_methods());
	}

	// Builds vector of allowed methods.
	// Used for http OPTIONS response.
	// To make this method return correct allowed methods,
	//		pass a vector of allowed methods names to protected
	//		constructor in derived class initialization.
	//
	// @return std::vector of strings.
	inline std::vector<std::string> allowed_methods()
	{
		std::vector<std::string> result;
		for (const auto& method : this->allowed_methods_list)
		{
			bool found = std::find(
				this->http_method_names.begin(),
				this->http_method_names.end(),
				str::lower(method)
			) != this->http_method_names.end();
			if (found)
			{
				result.push_back(str::upper(method));
			}
		}

		return result;
	}
};

__CONTROLLERS_END__
