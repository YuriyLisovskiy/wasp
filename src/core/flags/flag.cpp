/**
 * core/flags/flag.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./flag.h"


__FLAGS_BEGIN__

Flag::Flag(const std::string& label, const std::string& help)
{
	this->_help = help;
	this->_label = label;
}

std::string Flag::label()
{
	return this->_label;
}

std::string Flag::usage()
{
	return this->_help;
}

std::string Flag::get_raw()
{
	return this->_data;
}

__FLAGS_END__
