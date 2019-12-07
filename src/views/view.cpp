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


__WASP_BEGIN__

View::View(ILogger* logger) : _request(nullptr)
{
	this->_logger = logger;
	if (this->_logger == nullptr)
	{
		this->_logger = Logger::get_instance();
	}
}

void View::setup(HttpRequest& request)
{
	this->_request = &request;
}

HttpResponse* View::dispatch(HttpRequest& request)
{
	if (this->_request == nullptr)
	{
		// TODO: add class name
		throw NullPointerException(
			" instance has no 'request' attribute. Did you override setup() and forget to call super()?",
			_ERROR_DETAILS_
		);
	}

	std::string method = str::lower(request.method());
	HttpResponse* result = nullptr;
	if (method == "get")
	{
		result = this->get(request);
	}
	else if (method == "post")
	{
		result = this->post(request);
	}
	else if (method == "put")
	{
		result = this->put(request);
	}
	else if (method == "patch")
	{
		result = this->patch(request);
	}
	else if (method == "delete")
	{
		result = this->delete_(request);
	}
	else if (method == "head")
	{
		result = this->head(request);
	}
	else if (method == "options")
	{
		result = this->options(request);
	}
	else if (method == "trace")
	{
		result = this->trace(request);
	}

	if (!result)
	{
		result = this->http_method_not_allowed(request);
	}

	return result;
}

HttpResponse* View::http_method_not_allowed(HttpRequest& request)
{
	this->_logger->warning(
		"Method Not Allowed (" + request.method() + "): " + request.path(),
		_ERROR_DETAILS_
	);
	return new HttpResponseNotAllowed("", this->_allowed_methods());
}

std::vector<std::string> View::_allowed_methods()
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

HttpResponse* View::get(HttpRequest& request)
{
	return nullptr;
}

HttpResponse* View::post(HttpRequest& request)
{
	return nullptr;
}

HttpResponse* View::put(HttpRequest& request)
{
	return nullptr;
}

HttpResponse* View::patch(HttpRequest& request)
{
	return nullptr;
}

HttpResponse* View::delete_(HttpRequest& request)
{
	return nullptr;
}

HttpResponse* View::head(HttpRequest& request)
{
	return this->get(request);
}

HttpResponse* View::options(HttpRequest& request)
{
	auto* response = new HttpResponse("");
	response->set_header("Allow", str::join(", ", this->_allowed_methods()));
	response->set_header("Content-Length", "0");
	return response;
}

HttpResponse* View::trace(HttpRequest& request)
{
	return nullptr;
}

View::View(const std::vector<std::string>& allowed_methods, ILogger* logger) : View(logger)
{
	this->_allowed_methods_list = allowed_methods;
}

__WASP_END__
