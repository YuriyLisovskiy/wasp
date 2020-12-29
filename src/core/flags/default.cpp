/**
 * core/flags/flags.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./default.h"

// Core libraries.
#include <xalwart.core/string_utils.h>


__FLAGS_BEGIN__

LongIntFlag::LongIntFlag(const std::string& label, const std::string& help, long default_val)
	: TemplateFlag<long>(label, help)
{
	this->_default_val = default_val;
}

long LongIntFlag::from_string()
{
	return std::stol(this->_data);
}

bool LongIntFlag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::INT_TYPE);
}


DoubleFlag::DoubleFlag(
	const std::string& label, const std::string& help, double default_val
) : TemplateFlag<double>(label, help)
{
	this->_default_val = default_val;
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
) : TemplateFlag<std::string>(label, help)
{
	this->_default_val = default_val;
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
) : TemplateFlag<bool>(label, help)
{
	this->_default_val = default_val;
}

bool BoolFlag::from_string()
{
	auto lower = core::str::lower(this->_data);
	return lower == "true" || lower == "yes" || lower == "1";
}

bool BoolFlag::valid()
{
	return this->_data.empty() || std::regex_match(this->_data, internal::BOOL_TYPE);
}

__FLAGS_END__
