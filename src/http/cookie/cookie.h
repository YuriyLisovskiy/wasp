/**
 * http/cookie/cookie.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * An http cookies as object.
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/datetime.h>

// Module definitions.
#include "../_def_.h"


__HTTP_BEGIN__

static inline constexpr const char* COOKIE_DATE_TIME_FORMAT = "%a, %d %b %Y %T %Z";
static inline constexpr const char* COOKIE_DELETE_DATETIME = "Thu, 01 Jan 1970 00:00:00 GMT";

extern dt::Datetime _get_expires(long max_age);

extern long _get_max_age(const dt::Datetime& expires);

// TESTME: Cookie
// TODO: docs for 'Cookie'
class Cookie final
{
public:
	inline Cookie() : Cookie("", "")
	{
	}

	Cookie(std::string name, std::string value);

	Cookie(
		std::string name,
		std::string value,
		long max_age,
		std::string domain="",
		std::string path="/",
		bool is_secure=true,
		bool is_http_only=false,
		std::string same_site=""
	);

	Cookie(
		std::string name,
		std::string value,
		const dt::Datetime& expires,
		std::string domain="",
		std::string path="/",
		bool is_secure=true,
		bool is_http_only=false,
		std::string same_site=""
	);

	inline Cookie(const Cookie& other)
	{
		if (this != &other)
		{
			this->_copy(other);
		}
	}

	inline Cookie(Cookie&& other) noexcept
	{
		if (this != &other)
		{
			this->_move(std::forward<Cookie>(other));
		}
	}

	inline Cookie& operator= (const Cookie& other)
	{
		this->_copy(other);
		return *this;
	}

	inline Cookie& operator= (Cookie&& other) noexcept
	{
		if (this != &other)
		{
			this->_move(std::forward<Cookie>(other));
		}

		return *this;
	}

	bool operator== (const Cookie& right);

	[[nodiscard]]
	std::string to_string() const;

	[[nodiscard]]
	inline std::string name() const
	{
		return this->_name;
	}

	[[nodiscard]]
	inline std::string value() const
	{
		return this->_value;
	}

	[[nodiscard]]
	inline long max_age() const
	{
		return this->_max_age;
	}

	[[nodiscard]]
	dt::Datetime expires() const
	{
		return this->_expires;
	}

	[[nodiscard]]
	std::string domain() const
	{
		return this->_domain;
	}

	[[nodiscard]]
	std::string path() const
	{
		return this->_path;
	}

	[[nodiscard]]
	bool is_secure() const
	{
		return this->_is_secure;
	}

	[[nodiscard]]
	bool is_http_only() const
	{
		return this->_is_http_only;
	}

	[[nodiscard]]
	std::string same_site() const
	{
		return this->_same_site;
	}

private:
	std::string _name;
	std::string _value;

	// _max_age = 0 means no 'Max-Age' attribute specified.
	// _max_age < 0 means delete cookie now, equivalently 'Max-Age: 0'
	// _max_age > 0 means Max-Age attribute present and given in seconds
	long _max_age;

	dt::Datetime _expires;
	std::string _domain;
	std::string _path;
	bool _is_secure;
	bool _is_http_only;
	std::string _same_site;

	void _copy(const Cookie& other);

	void _move(Cookie&& other);
};

__HTTP_END__
