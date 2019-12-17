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
 * An implementation of view.h.
 */

#include "view.h"


__VIEWS_BEGIN__

View::View(ILogger* logger) : _request(nullptr), _allowed_methods_list({"options"})
{
	this->_logger = logger;
}

View::View(const std::vector<std::string>& allowed_methods, ILogger* logger) : View(logger)
{
	this->_allowed_methods_list = allowed_methods;
	if (std::find(allowed_methods.begin(), allowed_methods.end(), "options") == allowed_methods.end())
	{
		this->_allowed_methods_list.emplace_back("options");
	}
}

void View::setup(wasp::http::HttpRequest* request)
{
	this->_request = request;
}

wasp::http::HttpResponse* View::dispatch(Args* args)
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

		throw NullPointerException(
			name + " instance has not initialized request."
			" Did you override setup() and forget to call base method?",
			_ERROR_DETAILS_
		);
	}

	std::string method = str::lower(this->_request->method());
	wasp::http::HttpResponse* result = nullptr;
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

wasp::http::HttpResponse* View::http_method_not_allowed(wasp::http::HttpRequest* request)
{
	if (this->_logger != nullptr)
	{
		this->_logger->warning(
			"Method Not Allowed (" + request->method() + "): " + request->path(),
			_ERROR_DETAILS_
		);
	}

	return new wasp::http::HttpResponseNotAllowed("", this->allowed_methods());
}

std::vector<std::string> View::allowed_methods()
{
	std::vector<std::string> result;
	for (const auto& method : this->_allowed_methods_list)
	{
		bool found = std::find(
			this->_http_method_names.begin(),
			this->_http_method_names.end(),
			str::lower(method)
		) != this->_http_method_names.end();
		if (found)
		{
			result.push_back(str::upper(method));
		}
	}

	return result;
}

wasp::http::HttpResponse* View::get(wasp::http::HttpRequest* request, Args* args)
{
	return nullptr;
}

wasp::http::HttpResponse* View::post(wasp::http::HttpRequest* request, Args* args)
{
	return nullptr;
}

wasp::http::HttpResponse* View::put(wasp::http::HttpRequest* request, Args* args)
{
	return nullptr;
}

wasp::http::HttpResponse* View::patch(wasp::http::HttpRequest* request, Args* args)
{
	return nullptr;
}

wasp::http::HttpResponse* View::delete_(wasp::http::HttpRequest* request, Args* args)
{
	return nullptr;
}

wasp::http::HttpResponse* View::head(wasp::http::HttpRequest* request, Args* args)
{
	return this->get(request, args);
}

wasp::http::HttpResponse* View::options(wasp::http::HttpRequest* request, Args* args)
{
	auto* response = new wasp::http::HttpResponse("");
	response->set_header("Allow", str::join(", ", this->allowed_methods()));
	response->set_header("Content-Length", "0");
	return response;
}

wasp::http::HttpResponse* View::trace(wasp::http::HttpRequest* request, Args* args)
{
	return nullptr;
}

__VIEWS_END__
