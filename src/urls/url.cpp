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
 * An implementation of urls/url.h
 */

#include "./url.h"

// Framework modules.
#include "../core/string/str.h"


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
