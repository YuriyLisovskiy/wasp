/**
 * core/flags/flag.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: base flags types.
 */

#pragma once

// C++ libraries.
#include <string>
#include <type_traits>

// Module definitions.
#include "./_def_.h"


__CORE_FLAGS_BEGIN__

class Flag
{
private:
	friend class FlagSet;

protected:
	std::string _label;
	std::string _help;
	std::string _data;

	Flag(const std::string& label, const std::string& help);

public:
	virtual ~Flag() = default;
	bool empty();
	virtual std::string label();
	virtual std::string usage();
	virtual std::string get_raw();
	virtual bool valid() = 0;
	virtual std::pair<std::string, std::string> kwarg() = 0;
};

template<class T>
concept Stringifiable = requires(T x) {
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
	virtual FlagT from_string() = 0;

public:
	TemplateFlag(
		const std::string& label, const std::string& help, FlagT default_val
	) : Flag(label, help)
	{
		this->_default_val = default_val;
	}

	FlagT get()
	{
		if (!this->_data.empty())
		{
			return this->from_string();
		}

		return this->_default_val;
	}

	std::pair<std::string, std::string> kwarg() override
	{
		if (this->valid() && !this->_data.empty())
		{
			return {this->_label, this->_data};
		}

		if constexpr (Stringifiable<FlagT>)
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
