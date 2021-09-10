/**
 * conf/types.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Settings' types which have limited set of values.
 */

#pragma once

// C++ libraries.
#include <string>
#include <optional>
#include <vector>

// Base libraries.
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"


__CONF_BEGIN__

// TESTME: XFrameOptions
// TODO: docs for 'XFrameOptions'
class XFrameOptions final
{
public:
	enum Value
	{
		Deny = 0, SameOrigin
	};

	XFrameOptions() = default;

	constexpr inline XFrameOptions(Value value) : _value(value)
	{
	}

	inline operator Value() const
	{
		return this->_value;
	}

	explicit operator bool() = delete;

	constexpr inline bool operator== (XFrameOptions a) const
	{
		return this->_value == a._value;
	}

	constexpr inline bool operator!= (XFrameOptions a) const
	{
		return this->_value != a._value;
	}

	[[nodiscard]]
	constexpr inline const char* to_cstring() const
	{
		switch (this->_value) {
			case XFrameOptions::SameOrigin:
				return "SAMEORIGIN";
			case XFrameOptions::Deny:
				return "DENY";
			default:
				throw ValueError("invalid 'XFrameOptions' option", _ERROR_DETAILS_);
		}
	}

	[[nodiscard]]
	inline std::string to_string() const
	{
		return this->to_cstring();
	}

	inline static XFrameOptions from_string(const std::string& value)
	{
		auto lower_value = str::to_lower(value);
		if (lower_value == "sameorigin")
		{
			return Value::SameOrigin;
		}

		if (lower_value == "deny")
		{
			return Value::Deny;
		}

		throw ValueError("invalid X-Frame-Options value: '" + value + "'", _ERROR_DETAILS_);
	}

private:
	Value _value;
};

/** Security middleware settings.
 */
struct Secure final
{
	bool BROWSER_XSS_FILTER;
	bool CONTENT_TYPE_NO_SNIFF;
	bool HSTS_INCLUDE_SUBDOMAINS;
	bool HSTS_PRELOAD;
	size_t HSTS_SECONDS;
	std::vector<std::string> REDIRECT_EXEMPT;
	std::string REFERRER_POLICY;
	std::string SSL_HOST;
	bool SSL_REDIRECT;
	std::optional<std::pair<std::string, std::string>> PROXY_SSL_HEADER;
};

// TODO: docs for 'Static'
struct Static
{
	// Absolute filesystem path to the directory that will hold static files.
	std::string ROOT;

	// URL that handles files from `root` or some another url.
	std::string URL;
};

// TODO: docs for 'Limits'
struct Limits
{
	// Maximum size, in bytes, of a request before it will be streamed to the
	// file system instead of into memory.
	size_t FILE_UPLOAD_MAX_MEMORY_SIZE;

	// Maximum size in bytes of request data (excluding file uploads) that will be
	// read before a SuspiciousOperation (RequestDataTooBig) is raised.
	size_t DATA_UPLOAD_MAX_MEMORY_SIZE;

	// Maximum number of GET/POST parameters that will be read before a
	// SuspiciousOperation (TooManyFieldsSent) is thrown.
	size_t DATA_UPLOAD_MAX_NUMBER_FIELDS;

	// Maximum header length that will be read before returning HTTP 431 error code.
	size_t MAX_HEADER_LENGTH;

	// Maximum number of headers per request.
	size_t MAX_HEADERS_COUNT;
};

// TODO: docs for 'Formats'
struct Formats
{
	// Default formatting for date objects.
	std::string DATE_FORMAT;

	// Default formatting for datetime objects.
	std::string DATETIME_FORMAT;

	// Default formatting for time objects.
	std::string TIME_FORMAT;

	// Default formatting for date objects when only the year and month are relevant.
	std::string YEAR_MONTH_FORMAT;

	// Default formatting for date objects when only the month and day are relevant.
	std::string MONTH_DAY_FORMAT;

	// Default short formatting for date objects.
	std::string SHORT_DATE_FORMAT;

	// Default short formatting for datetime objects.
	std::string SHORT_DATETIME_FORMAT;
};

// TODO: docs for 'CSRFConfiguration'
struct CSRFConfiguration
{
	// Settings for CSRF cookie.
	struct Cookie
	{
		std::string NAME;
		size_t AGE;
		std::string DOMAIN;
		std::string PATH;
		bool SECURE;
		bool HTTP_ONLY;
		std::string SAME_SITE;
	};

	Cookie COOKIE;
	std::string HEADER_NAME;
	std::vector<std::string> TRUSTED_ORIGINS;
	bool USE_SESSIONS;
};

__CONF_END__
