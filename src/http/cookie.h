/**
 * http/cookie.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * An http cookies as object.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

// TESTME: Cookie
// TODO: docs for 'Cookie'
class Cookie final
{
private:
	const char* DATE_TIME_FORMAT = "%a, %e %b %Y %T %Z";

	std::string _name;
	std::string _value;

	// _max_age = 0 means no 'Max-Age' attribute specified.
	// _max_age < 0 means delete cookie now, equivalently 'Max-Age: 0'
	// _max_age > 0 means Max-Age attribute present and given in seconds
	long _max_age;

	std::string _expires;
	std::string _domain;
	std::string _path;
	bool _is_secure;
	bool _is_http_only;
	std::string _same_site;

	void _copy(const Cookie& other);

	[[nodiscard]]
	std::string _get_expires(long max_age) const;

	[[nodiscard]]
	long _get_max_age(const std::string& expires) const;

public:
	inline Cookie() : Cookie("", "", 0)
	{
	}

	Cookie(
		std::string name,
		std::string value,
		long max_age=0,
		std::string expires="",
		std::string domain="",
		std::string path="/",
		bool is_secure=true,
		bool is_http_only=false,
		std::string same_site=""
	);

	// Copy constructor.
	inline Cookie(const Cookie& other)
	{
		this->_copy(other);
	}

	// Move constructor.
	inline Cookie(Cookie&& other) noexcept
	{
		this->_copy(other);
	}

	// Copy assignment operator.
	inline Cookie& operator=(const Cookie& other)
	{
		this->_copy(other);
		return *this;
	}

	// Move assignment operator.
	inline Cookie& operator=(Cookie&& other) noexcept
	{
		this->_copy(other);
		return *this;
	}

	[[nodiscard]]
	std::string to_string() const;

	bool operator==(const Cookie& right);
};

__HTTP_END__
