/**
 * http/result.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <ostream>

// Module definitions.
#include "./_def_.h"

// Core libraries.
#include "./error.h"


__HTTP_BEGIN__

template <typename ValueT>
class Result
{
private:
	[[nodiscard]] bool _check_base(error_type base, error_type expected) const
	{
		return expected == base || this->err.type == expected;
	}

	[[nodiscard]] bool _check_nested(error_type super_base, error_type base, error_type expected) const
	{
		return expected == super_base || this->_check_base(base, expected);
	}

public:
	error err;
	ValueT value;

	Result() = default;

	explicit Result(ValueT data) : value(data)
	{
	}

	explicit Result(const error& err) : err(err)
	{
	}

	[[nodiscard]] bool catch_(error_type expected = HttpError) const
	{
		// Process HttpError-based errors
		if (this->err.type >= internal::ERROR_T_SO_LOWER && this->err.type <= internal::ERROR_T_SO_UPPER)
		{
			return this->err.type != None && this->_check_nested(
				HttpError, SuspiciousOperation, expected
			);
		}
		// Process HttpError-based errors
		else if (this->err.type >= internal::ERROR_T_HTTP_LOWER && this->err.type <= internal::ERROR_T_HTTP_UPPER)
		{
			return this->err.type != None && this->_check_base(HttpError, expected);
		}

		return this->err.type != None && expected == HttpError;
	}
};

template <error_type err_type, typename ValueT>
Result<ValueT> raise(const char* msg)
{
	return Result<ValueT>(error(err_type, msg, 0, "", ""));
}

template <error_type err_type, typename ValueT>
Result<ValueT> raise(const std::string& msg)
{
	return Result<ValueT>(error(err_type, msg.c_str(), 0, "", ""));
}

template <error_type err_type, typename ValueT>
Result<ValueT> raise(const char* msg, int line, const char* func, const char* file)
{
	return Result<ValueT>(error(err_type, msg, line, func, file));
}

template <error_type err_type, typename ValueT>
Result<ValueT> raise(const std::string& msg, int line, const char* func, const char* file)
{
	return Result<ValueT>(error(err_type, msg.c_str(), line, func, file));
}

__HTTP_END__
