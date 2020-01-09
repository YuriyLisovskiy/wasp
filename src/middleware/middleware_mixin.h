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
 * middleware_mixin.h
 * Purpose: base middleware class.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./interface.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

class MiddlewareMixin : public IMiddleware
{
protected:
	conf::Settings* settings;

public:
	explicit MiddlewareMixin(conf::Settings* settings);
	~MiddlewareMixin() override = default;

	http::HttpResponseBase* process_request(http::HttpRequest* request) override;
	http::HttpResponseBase* process_response(
		http::HttpRequest* request, http::HttpResponseBase* response
	) override;
};

__MIDDLEWARE_END__
