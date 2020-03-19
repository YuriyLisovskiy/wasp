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
 * An implementation of views/view.h
 */

#include "./view.h"


__VIEWS_BEGIN__

View::View(conf::Settings* settings)
	: _request(nullptr), _allowed_methods_list({"options"})
{
	this->_settings = settings;
	if (!this->_settings->LOGGER)
	{
		throw core::ImproperlyConfigured("View: LOGGER instance must be configured");
	}

	this->_logger = this->_settings->LOGGER.get();
}

View::View(
	const std::vector<std::string>& allowed_methods,
	conf::Settings* settings
) : View(settings)
{
	this->_allowed_methods_list = allowed_methods;
	if (std::find(allowed_methods.begin(), allowed_methods.end(), "options") == allowed_methods.end())
	{
		this->_allowed_methods_list.emplace_back("options");
	}
}

void View::setup(http::HttpRequest* request)
{
	this->_request = request;
}

std::unique_ptr<http::IHttpResponse> View::dispatch(Args* args)
{
	if (this->_request == nullptr)
	{
		int status;
		std::string name = typeid(*this).name();
		char* fullName = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
		if (status == 0)
		{
			name = fullName;
		}

		throw core::NullPointerException(
			name + " instance has not initialized request."
			" Did you override setup() and forget to call base method?",
			_ERROR_DETAILS_
		);
	}

	std::string method = core::str::lower(this->_request->method());
	std::unique_ptr<http::IHttpResponse> result = nullptr;
	if (method == "get")
	{
		result = this->get(this->_request, args);
	}
	else if (method == "post")
	{
		result = this->post(this->_request, args);
	}
	else if (method == "put")
	{
		result = this->put(this->_request, args);
	}
	else if (method == "patch")
	{
		result = this->patch(this->_request, args);
	}
	else if (method == "delete")
	{
		result = this->delete_(this->_request, args);
	}
	else if (method == "head")
	{
		result = this->head(this->_request, args);
	}
	else if (method == "options")
	{
		result = this->options(this->_request, args);
	}
	else if (method == "trace")
	{
		result = this->trace(this->_request, args);
	}

	if (!result)
	{
		result = this->http_method_not_allowed(this->_request);
	}

	return result;
}

std::unique_ptr<http::IHttpResponse> View::http_method_not_allowed(http::HttpRequest* request)
{
	if (this->_logger != nullptr)
	{
		this->_logger->warning(
			"Method Not Allowed (" + request->method() + "): " + request->path(),
			_ERROR_DETAILS_
		);
	}

	return std::make_unique<http::HttpResponseNotAllowed>("", this->allowed_methods());
}

std::vector<std::string> View::allowed_methods()
{
	std::vector<std::string> result;
	for (const auto& method : this->_allowed_methods_list)
	{
		bool found = std::find(
			this->_http_method_names.begin(),
			this->_http_method_names.end(),
			core::str::lower(method)
		) != this->_http_method_names.end();
		if (found)
		{
			result.push_back(core::str::upper(method));
		}
	}

	return result;
}

std::unique_ptr<http::IHttpResponse> View::get(http::HttpRequest* request, Args* args)
{
	return nullptr;
}

std::unique_ptr<http::IHttpResponse> View::post(http::HttpRequest* request, Args* args)
{
	return nullptr;
}

std::unique_ptr<http::IHttpResponse> View::put(http::HttpRequest* request, Args* args)
{
	return nullptr;
}

std::unique_ptr<http::IHttpResponse> View::patch(http::HttpRequest* request, Args* args)
{
	return nullptr;
}

std::unique_ptr<http::IHttpResponse> View::delete_(http::HttpRequest* request, Args* args)
{
	return nullptr;
}

std::unique_ptr<http::IHttpResponse> View::head(http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

std::unique_ptr<http::IHttpResponse> View::options(http::HttpRequest* request, Args* args)
{
	auto response = std::make_unique<http::HttpResponse>("");
	auto allowed_methods = this->allowed_methods();
	response->set_header(
		"Allow",
		core::str::join(allowed_methods.begin(), allowed_methods.end(), ", ")
	);
	response->set_header("Content-Length", "0");
	return response;
}

std::unique_ptr<http::IHttpResponse> View::trace(http::HttpRequest* request, Args* args)
{
	return nullptr;
}

__VIEWS_END__
