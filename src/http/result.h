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

// Core libraries.
#include <type_traits>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./error.h"


__HTTP_BEGIN__

template <typename T>
concept ResultValue = std::is_default_constructible_v<T> && !std::is_pointer_v<T>;

template <ResultValue ValueT>
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
	bool is_nullptr = false;

	Result() = default;

	explicit Result(ValueT data) : value(data)
	{
	}

	explicit Result(const error& err) : err(err)
	{
	}

	explicit Result(nullptr_t) : is_nullptr(true)
	{
	}

	explicit operator bool () const
	{
		return !this->is_nullptr;
	}

	[[nodiscard]] bool catch_(error_type expected = HttpError) const
	{
		// Process SuspiciousOperation-based errors
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

	template<typename NewType>
	Result<NewType> forward()
	{
		return Result<NewType>(this->err);
	}

	static Result<ValueT> null()
	{
		return Result<ValueT>(nullptr);
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
