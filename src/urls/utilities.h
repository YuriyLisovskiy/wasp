/**
 * utility.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * URL utilities.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Other libraries.
#include "./pattern.h"
#include "../controllers/static.h"


__URLS_BEGIN__

// TESTME: make_static
// TODO: docs for 'make_static'
inline std::shared_ptr<urls::Pattern<std::string>> make_static(
	const std::string& static_url, const std::string& static_root, const std::string& name=""
)
{
	if (static_url.empty())
	{
		throw ImproperlyConfigured("empty static url not permitted", _ERROR_DETAILS_);
	}

	auto controller_func = [static_root](
		http::Request* request, const std::tuple<std::string>& args, conf::Settings* settings
	) -> http::Response::Result
	{
		ctrl::StaticController controller(settings);
		controller.set_kwargs(Kwargs({
			{"document_root", static_root}
		}));
		controller.setup(request);
		return std::apply([controller](const std::string& p) mutable -> auto {return controller.dispatch(p); }, args);
	};

	return std::make_shared<Pattern<std::string>>(
		str::rtrim(static_url, "/") + "/" + "<path>(.*)", controller_func, name.empty() ? "static" : name
	);
}

__URLS_END__
