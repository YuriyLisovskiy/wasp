/**
 * commands/flags/flags.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Default flags.
 */

#pragma once

// C++ libraries.
#include <string>
#include <regex>

// Base libraries.
#include <xalwart.base/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./flag.h"


__COMMANDS_FLAGS_BEGIN__

// TESTME: UnsignedLongFlag
// TODO: docs for 'UnsignedLongFlag'
class UnsignedLongFlag final : public TemplateFlag<unsigned long>
{
public:
	inline UnsignedLongFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, unsigned long default_val
	) : TemplateFlag<unsigned long>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, UNSIGNED_LONG_INT_TYPE);
	}

protected:
	inline static const auto UNSIGNED_LONG_INT_TYPE = std::regex("[+]?[0-9]{1,10}");

	[[nodiscard]]
	inline unsigned long from_string() const override
	{
		return std::stol(this->_data);
	}
};

// TESTME: Uint16Flag
// TODO: docs for 'Uint16Flag'
class Uint16Flag final : public TemplateFlag<uint16_t>
{
public:
	inline Uint16Flag(
		const std::string& shorthand, const std::string& name, const std::string& usage, uint16_t default_val
	) : TemplateFlag<uint16_t>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, UINT16_TYPE);
	}

protected:
	inline static const auto UINT16_TYPE = std::regex("[+-]?[0-9]{1,5}");

	[[nodiscard]]
	uint16_t inline from_string() const override
	{
		return std::stol(this->_data);
	}
};

// TESTME: LongIntFlag
// TODO: docs for 'LongIntFlag'
class LongIntFlag final : public TemplateFlag<long int>
{
public:
	inline LongIntFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, long default_val
	) : TemplateFlag<long>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, LONG_INT_TYPE);
	}

protected:
	inline static const auto LONG_INT_TYPE = std::regex("[+-]?[0-9]{1,10}");

	[[nodiscard]]
	inline long int from_string() const override
	{
		return std::stol(this->_data);
	}
};

// TESTME: DoubleFlag
// TODO: docs for 'DoubleFlag'
class DoubleFlag final : public TemplateFlag<double>
{
public:
	inline DoubleFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, double default_val
	) : TemplateFlag<double>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, DOUBLE_TYPE);
	}

protected:
	inline static const auto DOUBLE_TYPE = std::regex("[+-]?[0-9]+[.]?[0-9]+");

	[[nodiscard]]
	inline double from_string() const override
	{
		return std::stod(this->_data);
	}
};

// TESTME: StringFlag
// TODO: docs for 'StringFlag'
class StringFlag final : public TemplateFlag<std::basic_string<char>>
{
public:
	inline StringFlag(
		const std::string& shorthand, const std::string& name,
		const std::string& usage, const std::basic_string<char>& default_val
	) : TemplateFlag<std::basic_string<char>>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return true;
	}

protected:
	[[nodiscard]]
	inline std::string from_string() const override
	{
		return this->_data;
	}
};

// TESTME: BoolFlag
// TODO: docs for 'BoolFlag'
class BoolFlag final : public TemplateFlag<bool>
{
public:
	inline BoolFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, bool default_val
	) : TemplateFlag<bool>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, BOOL_TYPE);
	}

protected:
	inline static const auto BOOL_TYPE = std::regex("true|TRUE|false|FALSE|yes|YES|no|NO|1|0|on|ON|off|OFF");

	[[nodiscard]]
	inline bool from_string() const override
	{
		auto lower = str::to_lower(this->_data);
		return lower == "true" || lower == "yes" || lower == "1" || lower == "on";
	}
};

__COMMANDS_FLAGS_END__
