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
)> resolve(const std::string& path, std::vector<std::shared_ptr<IPattern>>& urlpatterns)
{
	std::function<Result<std::shared_ptr<http::IHttpResponse>>(
		http::HttpRequest*, conf::Settings*
	)> fn = nullptr;
	for (auto& url_pattern : urlpatterns)
	{
		if (url_pattern->match(path))
		{
			fn = [url_pattern](
				http::HttpRequest* request, conf::Settings* settings
			) mutable -> Result<std::shared_ptr<http::IHttpResponse>>
			{
				return url_pattern->apply(request, settings);
			};
			break;
		}
	}

	return fn;
}

__URLS_END__
