/**
 * urls/resolver.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Url resolver.
 */

#pragma once

// C++ libraries.
#include <memory>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"


__URLS_BEGIN__

// TESTME: resolve
// Searches path in urlpatterns and returns an expression
// to process request if path is found, otherwise returns nullptr.
extern std::function<std::unique_ptr<http::abc::IHttpResponse>(
	http::Request* request, conf::Settings* settings
)> resolve(
	const std::string& path, std::vector<std::shared_ptr<IPattern>>& urlpatterns
);

// TESTME: is_valid_path
// Return true if the given path can be found in urlpatterns,
// false otherwise.
inline bool is_valid_path(const std::string& path, std::vector<std::shared_ptr<IPattern>>& urlpatterns)
{
	return resolve(path, urlpatterns) != nullptr;
}

__URLS_END__
