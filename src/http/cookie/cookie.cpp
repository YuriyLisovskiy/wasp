/**
 * http/cookie/cookie.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./cookie.h"

// Base libraries.
#include <xalwart.base/datetime.h>
#include <xalwart.base/utility.h>
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>


__HTTP_BEGIN__

dt::Datetime _get_expires(long max_age)
{
	if (max_age == 0)
	{
		return dt::Datetime::strptime(COOKIE_DELETE_DATETIME, COOKIE_DATE_TIME_FORMAT);
	}

	auto now = dt::Datetime::utc_now();
	return dt::Datetime::utc_from_timestamp(now.timestamp() + (double)max_age);
}

long _get_max_age(const dt::Datetime& expires)
{
	auto expires_gmt = expires.timestamp();
	long max_age = (long)expires_gmt - (long)dt::Datetime::utc_now().timestamp();
	return max_age > 0 ? max_age : 0;
}

Cookie::Cookie(std::string name, std::string value) :
	_name(std::move(name)), _value(std::move(value)), _max_age(0),
	_expires(dt::Datetime::strptime(COOKIE_DELETE_DATETIME, COOKIE_DATE_TIME_FORMAT)),
	_path("/"), _is_secure(true), _is_http_only(false)
{
	if (this->_name.empty())
	{
		throw ValueError("cookie's name can not be empty", _ERROR_DETAILS_);
	}
}

Cookie::Cookie(
	std::string name,
	std::string value,
	long max_age,
	std::string domain,
	std::string path,
	bool is_secure,
	bool is_http_only,
	std::string same_site
) : _name(std::move(name)), _value(std::move(value)), _max_age(max_age),
	_domain(std::move(domain)), _path(std::move(path)),
	_is_secure(is_secure), _is_http_only(is_http_only), _same_site(std::move(same_site))
{
	if (this->_name.empty())
	{
		throw ValueError("cookie's name can not be empty", _ERROR_DETAILS_);
	}

	if (this->_max_age < 0)
	{
		throw ValueError("cookie's Max-age can not be less than zero", _ERROR_DETAILS_);
	}

	if (this->_max_age < 0)
	{
		this->_max_age = 0;
	}

	this->_expires = _get_expires(this->_max_age);
}

Cookie::Cookie(
	std::string name,
	std::string value,
	const dt::Datetime& expires,
	std::string domain,
	std::string path,
	bool is_secure,
	bool is_http_only,
	std::string same_site
) : _name(std::move(name)), _value(std::move(value)),
    _expires(expires), _domain(std::move(domain)), _path(std::move(path)),
    _is_secure(is_secure), _is_http_only(is_http_only), _same_site(std::move(same_site))
{
	if (this->_name.empty())
	{
		throw ValueError("cookie's name can not be empty", _ERROR_DETAILS_);
	}

	if (this->_max_age < 0)
	{
		throw ValueError("cookie's Max-age can not be less than zero", _ERROR_DETAILS_);
	}

	auto tz_name = this->_expires.tz_name();
	if (!tz_name.empty() && tz_name != dt::Timezone::UTC.tz_name(nullptr))
	{
		throw ValueError("'expires' should have UTC timezone or created without it", _ERROR_DETAILS_);
	}

	this->_max_age = _get_max_age(this->_expires);
	if (this->_max_age == 0)
	{
		this->_expires = dt::Datetime::strptime(COOKIE_DELETE_DATETIME, COOKIE_DATE_TIME_FORMAT);
	}
}

std::string Cookie::to_string() const
{
	// Sets browser's directive 'Set-Cookie' and cookie's name and value.
	std::string result = "Set-Cookie: " + this->_name + "=" + this->_value;

	// Sets domain if it is not an empty string.
	result += !this->_domain.empty() ? "; Domain=" + this->_domain : "";

	// Sets path if it is not an empty string.
	result += !this->_path.empty() ? "; Path=" + this->_path : "";

	// Sets expiration time in '%a, %e %b %Y %T %Z' format,
	// for instance, 'Thu, 18 Jul 2019 16:25:19 GMT'.
	result += "; Max-Age=" + std::to_string(this->_max_age);

	auto string_expires = str::rtrim(this->_expires.strftime(COOKIE_DATE_TIME_FORMAT));
	result += "; Expires=" + (string_expires.ends_with("GMT") ? string_expires : string_expires + " GMT");

	if (!this->_same_site.empty())
	{
		std::vector<std::string> allowed_same_site_values = {"lax", "none", "strict"};
		if (!util::contains(
			str::to_lower(this->_same_site),
			allowed_same_site_values.begin(), allowed_same_site_values.end()
		))
		{
			throw ValueError(R"(samesite must be "lax", "none", or "strict".)");
		}

		result += "; SameSite=" + this->_same_site;
	}

	if (this->_is_secure)
	{
		result += "; Secure";
	}

	if (this->_is_http_only)
	{
		result += "; HttpOnly";
	}

	return result;
}

bool Cookie::operator== (const Cookie& other)
{
	if (this == &other)
	{
		return true;
	}

	return this->_name == other._name &&
		this->_value == other._value &&
		this->_domain == other._domain &&
		this->_path == other._path &&
		this->_max_age == other._max_age &&
		this->_expires == other._expires &&
		this->_is_secure == other._is_secure &&
		this->_is_http_only == other._is_http_only &&
		this->_same_site == other._same_site;
}

void Cookie::_copy(const Cookie& other)
{
	this->_name = other._name;
	this->_value = other._value;
	this->_max_age = other._max_age;
	this->_expires = other._expires;
	this->_domain = other._domain;
	this->_path = other._path;
	this->_is_secure = other._is_secure;
	this->_is_http_only = other._is_http_only;
	this->_same_site = other._same_site;
}

void Cookie::_move(Cookie&& other)
{
	this->_name = std::move(other._name);
	this->_value = std::move(other._value);
	this->_max_age = other._max_age;
	this->_expires = other._expires;
	this->_domain = std::move(other._domain);
	this->_path = std::move(other._path);
	this->_is_secure = other._is_secure;
	this->_is_http_only = other._is_http_only;
	this->_same_site = std::move(other._same_site);
}

__HTTP_END__
