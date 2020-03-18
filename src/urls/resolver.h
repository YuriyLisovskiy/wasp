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
 * urls/resolver.h
 *
 * Purpose: resolves given url as string.
 */

#pragma once

// C++ libraries.
#include <vector>
#include <functional>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./pattern.h"
#include "../http/response.h"
#include "../conf/settings.h"


__URLS_BEGIN__

/// Searches path in urlpatterns and returns an expression
/// to process request if path is found, otherwise returns nullptr.
extern std::function<std::unique_ptr<http::IHttpResponse>(
	http::HttpRequest* request,
	conf::Settings* settings
)> resolve(
	const std::string& path, std::vector<UrlPattern>& urlpatterns
);

/// Return true if the given path can be found in urlpatterns,
/// false otherwise.
extern bool is_valid_path(
	const std::string& path, std::vector<UrlPattern>& urlpatterns
);

__URLS_END__
