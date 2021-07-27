/**
 * urls/url.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./url.h"

// Base libraries.
#include <xalwart.base/string_utils.h>


__URLS_BEGIN__

std::shared_ptr<UrlPattern> make_url(
	const std::string& rgx,
	const ctrl::ViewHandler& handler,
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
		throw ImproperlyConfigured("Empty static url not permitted", _ERROR_DETAILS_);
	}

	auto view_func = [static_root](
		http::HttpRequest* request,
		core::Kwargs* args,
		conf::Settings* settings
	) -> Result<std::shared_ptr<http::IHttpResponse>>
	{
		ctrl::StaticController controller(settings);
		controller.set_kwargs(core::Kwargs({
			{"document_root", static_root}
		}));
		controller.setup(request);
		return controller.dispatch(args);
	};

	return make_url(
		str::rtrim(static_url, "/") + "/" + "<path>(.*)",
		view_func,
		name.empty() ? "static" : name
	);
}

__URLS_END__
