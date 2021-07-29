/**
 * core/flags/flags.h
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


__CORE_FLAGS_INTERNAL_BEGIN__

inline const std::regex BOOL_TYPE("true|TRUE|false|FALSE|yes|YES|no|NO|1|0|on|ON|off|OFF");
inline const std::regex LONG_INT_TYPE("[+-]?[0-9]{1,10}");
inline const std::regex UNSIGNED_LONG_INT_TYPE("[+]?[0-9]{1,10}");
inline const std::regex UINT16_TYPE("[+-]?[0-9]{1,5}");
inline const std::regex DOUBLE_TYPE("[+-]?[0-9]+[.]?[0-9]+");

__CORE_FLAGS_INTERNAL_END__


__CORE_FLAGS_BEGIN__

// TESTME: UnsignedLongFlag
// TODO: docs for 'UnsignedLongFlag'
class UnsignedLongFlag final : public TemplateFlag<unsigned long>
{
protected:
	[[nodiscard]]
	inline unsigned long from_string() const override
	{
		return std::stol(this->_data);
	}

public:
	inline UnsignedLongFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, unsigned long default_val
	) : TemplateFlag<unsigned long>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, internal::UNSIGNED_LONG_INT_TYPE);
	}
};

// TESTME: Uint16Flag
// TODO: docs for 'Uint16Flag'
class Uint16Flag final : public TemplateFlag<uint16_t>
{
protected:
	[[nodiscard]]
	uint16_t inline from_string() const override
	{
		return std::stol(this->_data);
	}

public:
	inline Uint16Flag(
		const std::string& shorthand, const std::string& name, const std::string& usage, uint16_t default_val
	) : TemplateFlag<uint16_t>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, internal::UINT16_TYPE);
	}
};

// TESTME: LongIntFlag
// TODO: docs for 'LongIntFlag'
class LongIntFlag final : public TemplateFlag<long int>
{
protected:
	[[nodiscard]]
	inline long int from_string() const override
	{
		return std::stol(this->_data);
	}

public:
	inline LongIntFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, long default_val
	) : TemplateFlag<long>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, internal::LONG_INT_TYPE);
	}
};

// TESTME: DoubleFlag
// TODO: docs for 'DoubleFlag'
class DoubleFlag final : public TemplateFlag<double>
{
protected:
	[[nodiscard]]
	inline double from_string() const override
	{
		return std::stod(this->_data);
	}

public:
	inline DoubleFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, double default_val
	) : TemplateFlag<double>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, internal::DOUBLE_TYPE);
	}
};

// TESTME: StringFlag
// TODO: docs for 'StringFlag'
class StringFlag final : public TemplateFlag<std::basic_string<char>>
{
protected:
	[[nodiscard]]
	inline std::string from_string() const override
	{
		return this->_data;
	}

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
};

// TESTME: BoolFlag
// TODO: docs for 'BoolFlag'
class BoolFlag final : public TemplateFlag<bool>
{
protected:
	[[nodiscard]]
	inline bool from_string() const override
	{
		auto lower = str::lower(this->_data);
		return lower == "true" || lower == "yes" || lower == "1" || lower == "on";
	}

public:
	inline BoolFlag(
		const std::string& shorthand, const std::string& name, const std::string& usage, bool default_val
	) : TemplateFlag<bool>(shorthand, name, usage, default_val)
	{
	}

	[[nodiscard]]
	inline bool valid() const override
	{
		return this->_data.empty() || std::regex_match(this->_data, internal::BOOL_TYPE);
	}
};

__CORE_FLAGS_END__
