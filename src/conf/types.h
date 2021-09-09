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

// TESTME: Secure
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

__CONF_END__
