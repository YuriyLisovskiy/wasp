/**
 * middleware/exception.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./exception.h"


__MIDDLEWARE_BEGIN__

Function Exception::operator() (const Function& next) const
{
	return [*this, next](http::Request* request) -> std::unique_ptr<http::HttpResponse>
	{
		net::StatusCode error_status_code = 500;
		std::string message;
		try
		{
			return next(request);
		}
		catch (const http::exc::HttpError& exc)
		{
			error_status_code = exc.status_code();
			message = exc.get_message();
		}
		catch (const BaseException& exc)
		{
			if (this->settings->LOGGER)
			{
				this->settings->LOGGER->error(exc);
			}

			message = exc.get_message();
		}
		catch (const std::exception& exc)
		{
			if (this->settings->LOGGER)
			{
				this->settings->LOGGER->error(exc.what(), _ERROR_DETAILS_);
			}

			message = exc.what();
		}

		require_non_null(request, _ERROR_DETAILS_);
		return this->error_to_response(error_status_code, message, request->is_json());
	};
}

std::unique_ptr<http::HttpResponse> Exception::error_to_response(
	net::StatusCode status_code, const std::string& message, bool is_json
) const
{
	auto [status, status_is_found] = net::get_status_by_code(status_code);
	std::string content = this->get_response_content(
		status_is_found ? status : DEFAULT_ERROR_STATUS, message, is_json
	);
	auto response = std::make_unique<http::Response>(status_code, content);
	if (is_json)
	{
		response->set_header(http::CONTENT_TYPE, http::mime::APPLICATION_JSON);
	}

	return response;
}

__MIDDLEWARE_END__
