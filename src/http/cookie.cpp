/**
 * http/cookie.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./cookie.h"

// Base libraries.
#include <xalwart.base/utility.h>
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>


__HTTP_BEGIN__

void Cookie::_copy(const Cookie& other)
{
	if (this != &other)
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
}

std::string Cookie::_get_expires(long max_age) const
{
	if (max_age == 0)
	{
		return "Thu, 01 Jan 1970 00:00:00 GMT";
	}

	auto now = dt::Datetime::utc_now();
	return dt::Datetime::utc_from_timestamp(
		now.timestamp() + (double)max_age
	).strftime(this->DATE_TIME_FORMAT);
}

long Cookie::_get_max_age(const std::string& expires) const
{
	auto expires_gmt = dt::Datetime::strptime(
		expires, this->DATE_TIME_FORMAT
	).timestamp();
	long max_age = (long)expires_gmt - (long)dt::Datetime::utc_now().timestamp();
	return max_age > 0 ? max_age : 0;
}

Cookie::Cookie() : Cookie("", "", 0)
{
}

Cookie::Cookie(
	std::string name,
	std::string value,
	long max_age,
	std::string expires,
	std::string domain,
	std::string path,
	bool is_secure,
	bool is_http_only,
	std::string same_site
) : _name(std::move(name)), _value(std::move(value)), _max_age(max_age),
	_expires(std::move(expires)), _domain(std::move(domain)), _path(std::move(path)),
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

	if (!this->_expires.empty())
	{
		this->_max_age = this->_get_max_age(this->_expires);
		if (this->_max_age == 0)
		{
			this->_expires = "Thu, 01 Jan 1970 00:00:00 GMT";
		}
	}
	else
	{
		if (this->_max_age < 0)
		{
			this->_max_age = 0;
		}

		this->_expires = this->_get_expires(this->_max_age);
	}
}

Cookie::Cookie(const Cookie& other)
{
	this->_copy(other);
}

Cookie::Cookie(Cookie&& other) noexcept
{
	this->_copy(other);
}

Cookie& Cookie::operator=(const Cookie& other)
{
	this->_copy(other);
	return *this;
}

Cookie& Cookie::operator=(Cookie&& other) noexcept
{
	this->_copy(other);
	return *this;
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
	result += "; Expires=" + this->_expires;

	if (!this->_same_site.empty())
	{
		std::vector<std::string> allowed_same_site_values = {"lax", "none", "strict"};
		if (!util::contains(
			str::lower(this->_same_site),
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

bool Cookie::operator==(const Cookie& right)
{
	return this->_name == right._name &&
		this->_value == right._value &&
		this->_domain == right._domain &&
		this->_path == right._path &&
		this->_max_age == right._max_age &&
		this->_expires == right._expires &&
		this->_is_secure == right._is_secure &&
		this->_is_http_only == right._is_http_only &&
		this->_same_site == right._same_site;
}

__HTTP_END__
