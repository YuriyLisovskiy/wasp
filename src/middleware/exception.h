/**
 * middleware/exception.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Processing of `xw::HttpError`, `xw::BaseException` and `std::exception` exceptions.
 */

#pragma once

// C++ libraries.
#include <memory>
#include <string>

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/net/status.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./types.h"
#include "./base.h"
#include "../conf/abc.h"
#include "../http/mime/content_types/application.h"


__MIDDLEWARE_BEGIN__

// TESTME: Exception
// TODO: docs for 'Exception'
class Exception final : public MiddlewareWithConstantSettings
{
public:
	inline static constexpr const char* NAME = "xw::middleware::Exception";

	explicit Exception(const conf::Settings* settings) : MiddlewareWithConstantSettings(settings)
	{
	}

	Function operator() (const Function& next);

protected:
	inline static const net::Status DEFAULT_ERROR_STATUS = {
		500, "Internal Server Error", "Server got itself in trouble"
	};

	[[nodiscard]]
	static inline std::unique_ptr<http::abc::IHttpResponse> get_response(
		net::StatusCode status_code, const std::string& content, bool is_json
	)
	{
		std::string content_type = is_json ? http::mime::APPLICATION_JSON : "";
		return std::make_unique<http::Response>(status_code, content, content_type);
	}

	[[nodiscard]]
	inline std::string get_response_content(const net::Status& status, const std::string& message, bool is_json) const
	{
		if (is_json)
		{
			return this->settings->render_json_error_template(status, message);
		}

		return this->settings->render_html_error_template(status, message);
	}

	std::unique_ptr<http::abc::IHttpResponse> error_to_response(
		net::StatusCode status_code, const std::string& message, bool is_json
	);
};

__MIDDLEWARE_END__
