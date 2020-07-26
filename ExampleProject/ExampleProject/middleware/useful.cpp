/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./useful.h"


UsefulMiddleware::UsefulMiddleware(xw::conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

std::unique_ptr<xw::http::IHttpResponse> UsefulMiddleware::process_request(
	xw::http::HttpRequest* request
)
{
	if (!request->headers.contains("Useful-Header"))
	{
		request->headers.set("Useful-Header", "The most useful value");
	}

	return nullptr;
}
