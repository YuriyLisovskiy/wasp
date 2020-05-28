/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of core/flags/flags.h
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
