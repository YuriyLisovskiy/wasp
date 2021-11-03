/**
 * middleware/clickjacking.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./clickjacking.h"

// C++ libraries.
#include <memory>


__MIDDLEWARE_BEGIN__

Function XFrameOptions::operator() (const Function& next) const
{
	return [*this, next](http::IRequest* request) -> std::unique_ptr<http::IResponse>
	{
		auto response = next(request);

		// Set it if it's not already in the response.
		if (!response->has_header(http::X_FRAME_OPTIONS))
		{
			require_non_null(
				response.get(),
				"Got nullptr response in '" + std::string(NAME) +
				"' middleware while setting " + std::string(http::X_FRAME_OPTIONS)
			);
			response->set_header(http::X_FRAME_OPTIONS, this->get_x_frame_options_value(request, response.get()));
		}

		return response;
	};
}

__MIDDLEWARE_END__
