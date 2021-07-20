/**
 * core/flags/flags.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./default.h"

// Base libraries.
#include <xalwart.base/string_utils.h>


__CORE_FLAGS_BEGIN__

Uint16Flag::Uint16Flag(const std::string& label, const std::string& help, uint16_t default_val)
	: TemplateFlag<uint16_t>(label, help, default_val)
{
}

uint16_t Uint16Flag::from_string()
{
	return std::stol(this->_data);
}

bool Uint16Flag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::UINT16_TYPE);
}


UnsignedLongFlag::UnsignedLongFlag(
	const std::string& label, const std::string& help, unsigned long default_val
) : TemplateFlag<unsigned long>(label, help, default_val)
{
}

unsigned long UnsignedLongFlag::from_string()
{
	return std::stol(this->_data);
}

bool UnsignedLongFlag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::UNSIGNED_LONG_INT_TYPE);
}


LongIntFlag::LongIntFlag(const std::string& label, const std::string& help, long default_val)
	: TemplateFlag<long>(label, help, default_val)
{
}

long int LongIntFlag::from_string()
{
	return std::stol(this->_data);
}

bool LongIntFlag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::LONG_INT_TYPE);
}


DoubleFlag::DoubleFlag(
	const std::string& label, const std::string& help, double default_val
) : TemplateFlag<double>(label, help, default_val)
{
}

double DoubleFlag::from_string()
{
	return std::stod(this->_data);
}

bool DoubleFlag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::DOUBLE_TYPE);
}


StringFlag::StringFlag(
	const std::string& label, const std::string& help, const std::string& default_val
) : TemplateFlag<std::basic_string<char>>(label, help, default_val)
{
}

std::string StringFlag::from_string()
{
	return this->_data;
}

bool StringFlag::valid()
{
	return true;
}


BoolFlag::BoolFlag(
	const std::string& label, const std::string& help, bool default_val
) : TemplateFlag<bool>(label, help, default_val)
{
}

bool BoolFlag::from_string()
{
	auto lower = str::lower(this->_data);
	return lower == "true" || lower == "yes" || lower == "1";
}

bool BoolFlag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::BOOL_TYPE);
}

__CORE_FLAGS_END__
