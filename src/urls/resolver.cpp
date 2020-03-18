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
 * An implementation of urls/resolver.h
 */

#include "./resolver.h"


__URLS_BEGIN__

std::function<std::unique_ptr<http::IHttpResponse>(
	http::HttpRequest* request,
	conf::Settings* settings
)> resolve(
	const std::string& path, std::vector<UrlPattern>& urlpatterns
)
{
	std::function<std::unique_ptr<http::IHttpResponse>(
		http::HttpRequest* request,
		conf::Settings* settings
	)> fn = nullptr;
	for (auto& url_pattern : urlpatterns)
	{
		std::map<std::string, std::string> args_map;
		if (url_pattern.match(path, args_map))
		{
			fn = [url_pattern, args_map](
				http::HttpRequest* request,
				conf::Settings* settings
			) mutable -> std::unique_ptr<http::IHttpResponse>
			{
				auto args = std::make_unique<views::Args>(args_map);
				return url_pattern.apply(request, settings, args.get());
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
