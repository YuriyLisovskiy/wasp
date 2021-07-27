/**
 * controllers/controller.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./controller.h"

// Base libraries.
#include <xalwart.base/utility.h>
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "../conf/settings.h"


__CONTROLLERS_BEGIN__

Controller::Controller(
	const std::vector<std::string>& allowed_methods,
	conf::Settings* settings
) : Controller(settings)
{
	this->allowed_methods_list = allowed_methods;
	if (std::find(allowed_methods.begin(), allowed_methods.end(), "options") == allowed_methods.end())
	{
		this->allowed_methods_list.emplace_back("options");
	}
}

Result<std::shared_ptr<http::IHttpResponse>> Controller::dispatch(Kwargs* kwargs)
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
		result = this->get(kwargs);
	}
	else if (method == "post")
	{
		result = this->post(kwargs);
	}
	else if (method == "put")
	{
		result = this->put(kwargs);
	}
	else if (method == "patch")
	{
		result = this->patch(kwargs);
	}
	else if (method == "delete")
	{
		result = this->delete_(kwargs);
	}
	else if (method == "head")
	{
		result = this->head(kwargs);
	}
	else if (method == "options")
	{
		result = this->options(kwargs);
	}
	else if (method == "trace")
	{
		result = this->trace(kwargs);
	}

	if (!result)
	{
		result = this->http_method_not_allowed();
	}

	return result;
}

Result<std::shared_ptr<http::IHttpResponse>> Controller::http_method_not_allowed()
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

std::vector<std::string> Controller::allowed_methods()
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

Result<std::shared_ptr<http::IHttpResponse>> Controller::options(Kwargs* kwargs)
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

__CONTROLLERS_END__
