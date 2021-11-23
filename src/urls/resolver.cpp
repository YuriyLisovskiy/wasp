/**
 * urls/resolver.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./resolver.h"


__URLS_BEGIN__

std::function<std::unique_ptr<http::IResponse>(http::IRequest*, conf::Settings*)> resolve(
	const std::string& path, const std::vector<std::shared_ptr<IPattern>>& urlpatterns
)
{
	std::function<std::unique_ptr<http::IResponse>(http::IRequest*, conf::Settings*)> fn = nullptr;
	for (const auto& url_pattern : urlpatterns)
	{
		if (url_pattern->match(path))
		{
			fn = [url_pattern](
				http::IRequest* request, conf::Settings* settings
			) -> std::unique_ptr<http::IResponse> {
				return url_pattern->apply(request, settings);
			};
			break;
		}
	}

	return fn;
}

__URLS_END__
