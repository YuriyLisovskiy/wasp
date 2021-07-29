/**
 * core/flags/flag.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Base flags types.
 */

#pragma once

// C++ libraries.
#include <string>
#include <type_traits>

// Base libraries.
#include <xalwart.base/utility.h>

// Module definitions.
#include "./_def_.h"


__CORE_FLAGS_BEGIN__

class Flag
{
private:
	friend class FlagSet;

protected:
	std::string _short_label;
	std::string _label;
	std::string _usage;
	std::string _data;

	Flag(std::string short_label, std::string label, std::string usage) :
		_short_label(std::move(short_label)), _label(std::move(label)), _usage(std::move(usage))
	{
		if (this->_short_label.empty())
		{
			throw ArgumentError("core::flags::Flag: short label can not be empty", _ERROR_DETAILS_);
		}

		if (this->_label.empty())
		{
			throw ArgumentError("core::flags::Flag: label can not be empty", _ERROR_DETAILS_);
		}
	}

public:
	virtual ~Flag() = default;

	[[nodiscard]]
	inline bool empty() const
	{
		return this->_data.empty();
	}

	[[nodiscard]]
	virtual inline std::string short_label() const
	{
		return this->_short_label;
	}

	[[nodiscard]]
	virtual inline std::string label() const
	{
		return this->_label;
	}

	[[nodiscard]]
	virtual inline std::string usage() const
	{
		return this->_usage;
	}

	[[nodiscard]]
	virtual std::string get_raw() const
	{
		return this->_data;
	}

	[[nodiscard]]
	virtual bool valid() const = 0;

	[[nodiscard]]
	virtual std::pair<std::string, std::string> kwarg() const = 0;
};

template<class T>
concept is_stringifiable_c = requires(T x) {
	std::to_string(x);
};

template <class FlagT, class = std::enable_if<
	std::is_fundamental_v<FlagT> || std::is_same_v<FlagT, std::basic_string<char>>
>>
class TemplateFlag : public Flag
{
protected:
	FlagT _default_val;

protected:
	virtual FlagT from_string() const = 0;

public:
	TemplateFlag(
		const std::string& short_label, const std::string& label, const std::string& help, FlagT default_val
	) : Flag(short_label, label, help), _default_val(default_val)
	{
	}

	FlagT get() const
	{
		if (!this->_data.empty())
		{
			return this->from_string();
		}

		return this->_default_val;
	}

	[[nodiscard]]
	std::pair<std::string, std::string> kwarg() const override
	{
		if (this->valid() && !this->_data.empty())
		{
			return {this->_label, this->_data};
		}

		if constexpr (is_stringifiable_c<FlagT>)
		{
			return {this->_label, std::to_string(this->_default_val)};
		}
		else
		{
			return {this->_label, this->_default_val};
		}
	}
};

__CORE_FLAGS_END__
