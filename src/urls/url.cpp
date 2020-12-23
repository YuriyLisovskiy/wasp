/**
 * urls/url.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./url.h"

// Core libraries.
#include <xalwart.core/string_utils.h>


__URLS_BEGIN__

std::shared_ptr<urls::UrlPattern> make_url(
	const std::string& rgx,
	const views::ViewHandler& handler,
	const std::string& name
)
{
	return std::make_shared<UrlPattern>(rgx, handler, name);
}

std::shared_ptr<urls::UrlPattern> make_static(
	const std::string& static_url,
	const std::string& static_root,
	const std::string& name
)
{
	if (static_url.empty())
	{
		throw core::ImproperlyConfigured("Empty static url not permitted", _ERROR_DETAILS_);
	}

	auto view_func = [static_root](
		http::HttpRequest* request,
		views::Args* args,
		conf::Settings* settings
	) -> std::unique_ptr<http::IHttpResponse>
	{
		views::StaticView view(settings);
		auto kwargs = std::make_unique<collections::Dict<std::string, std::string>>(
			std::map<std::string, std::string>{
				{"document_root", static_root}
			}
		);
		view.set_kwargs(kwargs.get());
		view.setup(request);
		return view.dispatch(args);
	};

	return make_url(
		core::str::rtrim(static_url, "/") + "/" + "<path>(.*)",
		view_func,
		name.empty() ? "static" : name
	);
}

__URLS_END__
