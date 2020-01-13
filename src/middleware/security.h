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
 * security.h
 */

#pragma once

// C++ libraries.
#include <vector>
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./middleware_mixin.h"
#include "../core/regex.h"
#include "../core/string/str.h"
#include "../http/response.h"
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

class SecurityMiddleware : public MiddlewareMixin
{
protected:
	size_t sts_seconds;
	bool sts_include_subdomains;
	bool sts_preload;
	bool content_type_no_sniff;
	bool xss_filter;
	bool redirect;
	std::string redirect_host;
	std::string referrer_policy;
	std::vector<core::rgx::Regex> redirect_exempt;

public:
	explicit SecurityMiddleware(conf::Settings* settings);

	http::HttpResponseBase* process_request(http::HttpRequest* request) override;
	http::HttpResponseBase* process_response(http::HttpRequest* request, http::HttpResponseBase* response) override;
};

__MIDDLEWARE_END__
