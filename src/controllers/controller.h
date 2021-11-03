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
#include <xalwart.base/interfaces/base.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/settings.h"


__CONTROLLERS_BEGIN__

template <typename ...ArgsT>
using Handler = std::function<std::unique_ptr<http::HttpResponse>(
	http::Request*, const std::tuple<ArgsT...>&, const conf::Settings*
)>;

// TESTME: Controller<...URLArgsT>
// TODO: docs for 'Controller<...URLArgsT>'
template <typename ...URLArgsT>
class Controller
{
public:
	inline explicit Controller(const ILogger* logger) : Controller<URLArgsT...>({"options"}, logger)
	{
	}

	// Processes http GET request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> get(http::Request* request, URLArgsT ...args) const
	{
		return nullptr;
	}

	// Processes http POST request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> post(http::Request* request, URLArgsT ...args) const
	{
		return nullptr;
	}

	// Processes http PUT request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> put(http::Request* request, URLArgsT ...args) const
	{
		return nullptr;
	}

	// Processes http PATCH request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> patch(http::Request* request, URLArgsT ...args) const
	{
		return nullptr;
	}

	// Processes http DELETE request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> delete_(http::Request* request, URLArgsT ...args) const
	{
		return nullptr;
	}

	// Processes http HEAD request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> head(http::Request* request, URLArgsT ...args) const
	{
		return this->get(request, args...);
	}

	// Processes http OPTIONS request.
	// Can be overridden in derived class, otherwise returns nullptr.
	//
	// @param request: pointer to http request.
	// @param args: pointer to requests' url arguments.
	// @return pointer to http response instance.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> options(http::Request* request, URLArgsT ...args) const
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
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> trace(http::Request* request, URLArgsT ...args) const
	{
		return nullptr;
	}

	// Try to dispatch to the right method; if a method doesn't exist,
	// defer to the error handler. Also defer to the error handler if the
	// request method isn't on the approved list.
	//
	// @param request: an actual http request from client.
	// @return pointer to http response returned from handler.
	[[nodiscard]]
	virtual inline std::unique_ptr<http::HttpResponse> dispatch(http::Request* request, URLArgsT ...args) const
	{
		if (!request)
		{
			throw NullPointerException(
				demangle(typeid(*this).name()) + " instance has not initialized request.", _ERROR_DETAILS_
			);
		}

		std::string method = str::to_lower(request->method());
		std::unique_ptr<http::HttpResponse> result = nullptr;
		if (this->_has_method(method))
		{
			result = this->method_not_allowed_response(request);
		}
		else
		{
			auto method_function = this->_get_method(method);
			if (method_function)
			{
				result = method_function(request, args...);
			}
			else
			{
				result = this->method_not_allowed_response(request);
			}
		}

		return result;
	}

	// Builds vector of allowed methods and used for http OPTIONS response.
	// To make this method return correct allowed methods, pass a vector of
	// allowed methods names to protected constructor in derived class
	// initialization.
	[[nodiscard]]
	inline std::vector<std::string> allowed_methods() const
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

protected:
	const ILogger* logger = nullptr;

	// Contains all possible http methods which controller can handle.
	const std::vector<std::string> http_method_names = {
		"get", "post", "put", "patch", "delete", "head", "options", "trace"
	};

	// List of methods witch will be returned when 'OPTIONS' is in request.
	std::vector<std::string> allowed_methods_list{};

	inline explicit Controller(
		const std::vector<std::string>& allowed_methods, const ILogger* logger
	) : logger(logger)
	{
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

	// Returns Http 405 (Method Not Allowed) response.
	//
	// @param request: pointer to http request.
	// @return pointer to http response returned from handler.
	[[nodiscard]]
	inline std::unique_ptr<http::HttpResponse> method_not_allowed_response(http::Request* request) const
	{
		if (this->logger)
		{
			this->logger->warning(
				"Method Not Allowed (" + request->method() + "): " + request->url().path, _ERROR_DETAILS_
			);
		}

		return std::make_unique<http::NotAllowed>("", this->allowed_methods());
	}

private:
	[[nodiscard]]
	inline bool _has_method(const std::string& method) const
	{
		return std::find(
			this->allowed_methods_list.begin(), this->allowed_methods_list.end(), str::to_lower(method)
		) == this->allowed_methods_list.end();
	}

	[[nodiscard]]
	inline std::function<std::unique_ptr<http::HttpResponse>(http::Request*, URLArgsT...)> _get_method(
		const std::string& method
	) const
	{
		if (method == "get")
		{
			return [this](auto* request, auto ...args) -> auto {return this->get(request, args...); };
		}
		else if (method == "post")
		{
			return [this](auto* request, auto ...args) -> auto {return this->post(request, args...); };
		}
		else if (method == "put")
		{
			return [this](auto* request, auto ...args) -> auto {return this->put(request, args...); };
		}
		else if (method == "patch")
		{
			return [this](auto* request, auto ...args) -> auto {return this->patch(request, args...); };
		}
		else if (method == "delete")
		{
			return [this](auto* request, auto ...args) -> auto {return this->delete_(request, args...); };
		}
		else if (method == "head")
		{
			return [this](auto* request, auto ...args) -> auto {return this->head(request, args...); };
		}
		else if (method == "options")
		{
			return [this](auto* request, auto ...args) -> auto {return this->options(request, args...); };
		}
		else if (method == "trace")
		{
			return [this](auto* request, auto ...args) -> auto {return this->trace(request, args...); };
		}
		else
		{
			return nullptr;
		}
	}
};

__CONTROLLERS_END__
