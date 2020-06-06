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
 * middleware/middleware_mixin.h
 *
 * Purpose:
 * 	Base middleware class.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

class MiddlewareMixin : public IMiddleware
{
protected:
	conf::Settings* settings;

public:
	explicit MiddlewareMixin(conf::Settings* settings);
	MiddlewareMixin(nullptr_t) = delete;
	~MiddlewareMixin() override = default;

	std::unique_ptr<http::IHttpResponse> process_request(http::HttpRequest* request) override;
	std::unique_ptr<http::IHttpResponse> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
