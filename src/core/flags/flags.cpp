/**
 * core/flags/flags.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./flags.h"


__FLAGS_BEGIN__

Flag::Flag(const std::string& label, const std::string& help)
{
	this->_help = help;
	this->_label = label;
}

std::string Flag::get_raw()
{
	return this->_data;
}


LongIntFlag::LongIntFlag(const std::string& label, const std::string& help, long default_val)
	: Flag(label, help)
{
	this->_default_val = default_val;
}

std::string LongIntFlag::label()
{
	return this->_label;
}

std::string LongIntFlag::usage()
{
	return this->_help;
}

long LongIntFlag::get()
{
	if (!this->_data.empty())
	{
		return std::stol(this->_data);
	}

	return this->_default_val;
}


DoubleFlag::DoubleFlag(
	const std::string& label, const std::string& help, double default_val
) : Flag(label, help)
{
	this->_default_val = default_val;
}

std::string DoubleFlag::label()
{
	return this->_label;
}

std::string DoubleFlag::usage()
{
	return this->_help;
}

double DoubleFlag::get()
{
	if (!this->_data.empty())
	{
		return std::stod(this->_data);
	}

	return this->_default_val;
}


StringFlag::StringFlag(
	const std::string& label, const std::string& help, const std::string& default_val
) : Flag(label, help)
{
	this->_default_val = default_val;
}

std::string StringFlag::label()
{
	return this->_label;
}

std::string StringFlag::usage()
{
	return this->_help;
}

std::string StringFlag::get()
{
	if (!this->_data.empty())
	{
		return this->_data;
	}

	return this->_default_val;
}


BoolFlag::BoolFlag(
	const std::string& label, const std::string& help, bool default_val
) : Flag(label, help)
{
	this->_default_val = default_val;
}

std::string BoolFlag::label()
{
	return this->_label;
}

std::string BoolFlag::usage()
{
	return this->_help;
}

bool BoolFlag::get()
{
	if (!this->_data.empty())
	{
		return this->_data == "true";
	}

	return this->_default_val;
}

__FLAGS_END__
