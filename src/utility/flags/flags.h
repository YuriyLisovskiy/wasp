/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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
 * flags.h
 * Purpose: flags' types.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__FLAGS_BEGIN__

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
	virtual std::string label() = 0;
	virtual std::string usage() = 0;
	virtual std::string get_raw();
};


class LongIntFlag final: public Flag
{
private:
	long _default_val;

public:
	LongIntFlag(
		const std::string& label, const std::string& help, long default_val
	);
	std::string label() final;
	std::string usage() final;
	long get();
};

class DoubleFlag final: public Flag
{
private:
	double _default_val;

public:
	DoubleFlag(
		const std::string& label, const std::string& help, double default_val
	);
	std::string label() final;
	std::string usage() final;
	double get();
};

class StringFlag final: public Flag
{
private:
	std::string _default_val;

public:
	StringFlag(
		const std::string& label, const std::string& help, const std::string& default_val
	);
	std::string label() final;
	std::string usage() final;
	std::string get();
};

class BoolFlag final: public Flag
{
public:
	BoolFlag(const std::string& label, const std::string& help);
	std::string label() final;
	std::string usage() final;
	bool get();
};

__FLAGS_END__
