/**
 * core/error.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <ostream>
#include <string>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./interfaces.h"


__HTTP_INTERNAL_BEGIN__

const short ERROR_T_HTTP_LOWER = 2;
const short ERROR_T_HTTP_UPPER = 7;

const short ERROR_T_SO_LOWER = 8;
const short ERROR_T_SO_UPPER = 9;

__HTTP_INTERNAL_END__

__HTTP_BEGIN__

enum error_type
{
	None = 0,
	HttpError = 1,

	// HttpError-based exceptions.
	EntityTooLargeError = 2,
	FileDoesNotExistError = 3,
	PermissionDenied = 4,
	NotFound = 5,
	InternalServerError = 6,
	SuspiciousOperation = 7,

	// SuspiciousOperation-based exceptions.
	DisallowedHost = 8,
	DisallowedRedirect = 9
};

struct error
{
	short type{};
	int line = 0;
	const char* func = "";
	const char* file = "";
	const char* msg = "";

	error(
		error_type type, const char* msg, int line, const char* func, const char* file
	);

	error(
		error_type type, const std::string& msg, int line, const char* func, const char* file
	);

	explicit error(error_type type, const char* msg);

	explicit error(error_type type, const std::string& msg);

	error();

	static error none();

	explicit operator bool() const;

	std::shared_ptr<IHttpResponse> get_response();
};

__HTTP_END__

std::ostream& operator<<(std::ostream& os, const xw::http::error_type& type);

std::ostream& operator<<(std::ostream& os, const xw::http::error& err);
