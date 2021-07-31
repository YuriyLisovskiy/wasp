/**
 * urls/resolver.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./resolver.h"


__URLS_BEGIN__

std::function<Result<std::shared_ptr<http::IHttpResponse>>(
	http::HttpRequest* request,
	conf::Settings* settings
)> resolve(
	const std::string& path, std::vector<std::shared_ptr<UrlPattern>>& urlpatterns
)
{
	std::function<Result<std::shared_ptr<http::IHttpResponse>>(
		http::HttpRequest*, conf::Settings*
	)> fn = nullptr;
	for (auto& url_pattern : urlpatterns)
	{
		std::map<std::string, std::string> args_map;
		if (url_pattern->match(path, args_map))
		{
			fn = [url_pattern, args_map](
				http::HttpRequest* request, conf::Settings* settings
			) mutable -> Result<std::shared_ptr<http::IHttpResponse>>
			{
				auto kwargs = Kwargs(args_map);
				return url_pattern->apply(request, settings, &kwargs);
			};
			break;
		}
	}

	return fn;
}

__URLS_END__
