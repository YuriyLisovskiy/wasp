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
 * cache.h
 * Purpose:
 * 	Provides utils for developing middleware.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./http.h"
#include "./crypto/md5.h"
#include "../http/response.h"
#include "../http/response.h"


__CACHE_BEGIN__

extern void set_response_etag(http::HttpResponseBase* response);

extern http::HttpResponseBase* get_conditional_response(
	http::HttpRequest* request,
	const std::string& etag,
	const std::string& last_modified,
	http::HttpResponseBase* response
);

__CACHE_END__
