/**
 * http/cookie.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: represents an http cookies as object.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

class Cookie final
{
private:
	const char* DATE_TIME_FORMAT = "%a, %e %b %Y %T %Z";

	std::string _name;
	std::string _value;
	long _max_age;
	std::string _expires;
	std::string _domain;
	std::string _path;
	bool _is_secure;
	bool _is_http_only;
	std::string _same_site;

	void _copy(const Cookie& other);
	[[nodiscard]] std::string _get_expires(long max_age) const;
	[[nodiscard]] long _get_max_age(const std::string& expires) const;

public:
	Cookie();
	Cookie(
		std::string name,
		std::string value,
		long max_age,
		std::string expires = "",
		std::string domain = "",
		std::string path = "/",
		bool is_secure = true,
		bool is_http_only = false,
		std::string same_site = ""
	);

	// Copy constructor.
	Cookie(const Cookie& other);

	// Move constructor.
	Cookie(Cookie&& other) noexcept;

	// Copy assignment operator.
	Cookie& operator=(const Cookie& other);

	// Move assignment operator.
	Cookie& operator=(Cookie&& other) noexcept;

	[[nodiscard]] std::string to_string() const;

	bool operator==(const Cookie& right);
};

__HTTP_END__
