/**
 * controllers/controller.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Intentionally simple parent class for all controllers.
 */

#pragma once

// C++ libraries.
#include <memory>
#include <functional>

// Base libraries.
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
using Handler = std::function<std::unique_ptr<http::abc::IHttpResponse>(
	http::Request*, const std::tuple<ArgsT...>&, conf::Settings*
)>;

// TESTME: Controller<...URLArgsT>
// TODO: docs for 'Controller<...URLArgsT>'
template <typename ...URLArgsT>
class Controller
{
protected:
	http::Request* request = nullptr;
	conf::Settings* settings = nullptr;

	// Contains all possible http methods which controller can handle.
	const std::vector<std::string> http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	// List of methods witch will be returned when 'OPTIONS' is in request.
	std::vector<std::string> allowed_methods_list{};

protected:
	inline explicit Controller(
		const std::vector<std::string>& allowed_methods, conf::Settings* settings
	) : settings(settings)
	{
		util::require_non_null(this->settings, "'settings' is nullptr", _ERROR_DETAILS_);
		for (const auto& method : allowed_methods)
		{
			this->allowed_methods_list.push_back(str::to_lower(method));
		}

		auto options = std::find(this->allowed_methods_list.begin(), this->allowed_methods_list.end(), "options");
		if (options == this->allowed_methods_list.end())
		{
			this->allowed_methods_list.emplace_back("options");
		}
	}

public:
	inline explicit Controller(conf::Settings* settings) : Controller<URLArgsT...>({"options"}, settings)
	{
	}

	// Processes http GET request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> get(URLArgsT ...args)
	{
		return nullptr;
	}

	// Processes http POST request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> post(URLArgsT ...args)
	{
		return nullptr;
	}

	// Processes http PUT request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> put(URLArgsT ...args)
	{
		return nullptr;
	}

	// Processes http PATCH request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> patch(URLArgsT ...args)
	{
		return nullptr;
	}

	// Processes http DELETE request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> delete_(URLArgsT ...args)
	{
		return nullptr;
	}

	// Processes http HEAD request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> head(URLArgsT ...args)
	{
		return this->get(args...);
	}

	// Processes http OPTIONS request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> options(URLArgsT ...args)
	{
		auto response = std::make_unique<http::Response>(200);
		auto allowed_methods = this->allowed_methods();
		response->set_header(http::ALLOW, str::join(", ", allowed_methods.begin(), allowed_methods.end()));
		response->set_header(http::CONTENT_LENGTH, "0");
		return response;
	}

	// Processes http TRACE request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> trace(URLArgsT ...args)
	{
		return nullptr;
	}

	// Setups request before dispatch call.
	//
	// @param request: pointer to http request.
	virtual inline void setup(http::Request* req)
	{
		this->request = req;
	}

	// Try to dispatch to the right method; if a method doesn't exist,
	// defer to the error handler. Also defer to the error handler if the
	// request method isn't on the approved list.
	//
	// @param request: an actual http request from client.
	// @return pointer to http response returned from handler.
	virtual inline std::unique_ptr<http::abc::IHttpResponse> dispatch(URLArgsT ...args)
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

		std::string method = str::to_lower(this->request->method);
		std::unique_ptr<http::abc::IHttpResponse> result = nullptr;
		if (std::find(
			this->allowed_methods_list.begin(), this->allowed_methods_list.end(), str::to_lower(method)
		) == this->allowed_methods_list.end())
		{
			result = this->http_method_not_allowed();
		}
		else
		{
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
			else
			{
				result = this->http_method_not_allowed();
			}
		}

		return result;
	}

	// Returns Http 405 (Method Not Allowed) response.
	//
	// @param request: pointer to http request.
	// @return pointer to http response returned from handler.
	inline std::unique_ptr<http::abc::IHttpResponse> http_method_not_allowed()
	{
		if (!this->settings->LOGGER)
		{
			throw NullPointerException("settings->LOGGER is nullptr", _ERROR_DETAILS_);
		}

		this->settings->LOGGER->warning(
			"Method Not Allowed (" + this->request->method + "): " + this->request->url.path,
			_ERROR_DETAILS_
		);

		return std::make_unique<http::resp::NotAllowed>("", this->allowed_methods());
	}

	// Builds vector of allowed methods and used for http OPTIONS response.
	// To make this method return correct allowed methods, pass a vector of
	// allowed methods names to protected constructor in derived class
	// initialization.
	inline std::vector<std::string> allowed_methods()
	{
		std::vector<std::string> result;
		for (const auto& method : this->allowed_methods_list)
		{
			bool found = std::find(
				this->http_method_names.begin(), this->http_method_names.end(), str::to_lower(method)
			) != this->http_method_names.end();
			if (found)
			{
				result.push_back(str::to_upper(method));
			}
		}

		return result;
	}
};

__CONTROLLERS_END__
