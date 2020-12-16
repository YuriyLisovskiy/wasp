/**
 * http/error.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./error.h"


__HTTP_BEGIN__

error::error(
	error_type type, const char* msg, int line, const char* func, const char* file
) : type(type), msg(msg), line(line), func(func), file(file)
{
}

error::error(
	error_type type, const std::string& msg, int line, const char* func, const char* file
) : type(type), msg(msg.c_str()), line(line), func(func), file(file)
{
}

error::error(error_type type, const char* msg) : error(type, msg, 0, "", "")
{
}

error::error(error_type type, const std::string& msg) : error(type, msg, 0, "", "")
{
}

error::error() : error(None, "", 0, "", "")
{
}

error error::none()
{
	return error();
}

error::operator bool() const
{
	return this->type != None;
}

__HTTP_END__

std::ostream& operator<<(std::ostream& os, const xw::http::error_type& type)
{
	const char *repr;
	switch (type)
	{
		case xw::http::error_type::None:
			repr = "None";
			break;
		case xw::http::error_type::HttpError:
			repr = "HttpError";
			break;
		case xw::http::error_type::DisallowedHost:
			repr = "DisallowedHost";
			break;
		case xw::http::error_type::DisallowedRedirect:
			repr = "DisallowedRedirect";
			break;
		case xw::http::error_type::EntityTooLargeError:
			repr = "EntityTooLargeError";
			break;
		case xw::http::error_type::FileDoesNotExistError:
			repr = "FileDoesNotExistError";
			break;
		case xw::http::error_type::PermissionDenied:
			repr = "PermissionDenied";
			break;
		case xw::http::error_type::SuspiciousOperation:
			repr = "SuspiciousOperation";
			break;
		default:
			repr = "Unknown";
			break;
	}

	return os << repr;
}

std::ostream& operator<<(std::ostream& os, const xw::http::error& err)
{
	return os << "\nFile \"" << err.file << "\", line " << err.line << ", in " << err.func << '\n' << err.msg << '\n';
}
