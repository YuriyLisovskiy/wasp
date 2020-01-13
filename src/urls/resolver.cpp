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
 * An implementation of resolver.h.
 */

#include "./resolver.h"


__URLS_BEGIN__

std::function<http::HttpResponseBase*(
	http::HttpRequest* request,
	core::ILogger* logger
)> resolve(
	const std::string& path, std::vector<UrlPattern>& urlpatterns
)
{
	std::function<http::HttpResponseBase*(
		http::HttpRequest* request,
		core::ILogger* logger
	)> fn = nullptr;
	for (auto& url_pattern : urlpatterns)
	{
		std::map<std::string, std::string> args_map;
		if (url_pattern.match(path, args_map))
		{
			fn = [url_pattern, args_map](
				http::HttpRequest* request,
				core::ILogger* logger
			) mutable -> http::HttpResponseBase*
			{
				return url_pattern.apply(request, new views::Args(args_map), logger);
			};
			break;
		}
	}

	return fn;
}

bool is_valid_path(
	const std::string& path, std::vector<UrlPattern>& urlpatterns
)
{
	return resolve(path, urlpatterns) != nullptr;
}

__URLS_END__
