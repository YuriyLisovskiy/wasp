/**
 * core/flags/flags.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: flags' types.
 */

#pragma once

// C++ libraries.
#include <string>
#include <regex>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./flag.h"


__FLAGS_INTERNAL_BEGIN__

const std::regex BOOL_TYPE("true|TRUE|false|FALSE|yes|YES|no|NO|1|0");
const std::regex INT_TYPE("[+-]?[0-9]+");
const std::regex UNSIGNED_INT_TYPE("[+]?[0-9]+");
const std::regex DOUBLE_TYPE("[+-]?[0-9]+[.]?[0-9]+");
const std::regex UNSIGNED_DOUBLE_TYPE("[+]?[0-9]+[.]?[0-9]+");

__FLAGS_INTERNAL_END__


__FLAGS_BEGIN__

class LongIntFlag final : public TemplateFlag<long>
{
protected:
	long from_string() override;

public:
	LongIntFlag(
		const std::string& label, const std::string& help, long default_val
	);
	bool valid() final;
};

class DoubleFlag final : public TemplateFlag<double>
{
protected:
	double from_string() override;

public:
	DoubleFlag(
		const std::string& label, const std::string& help, double default_val
	);
	bool valid() final;
};

class StringFlag final : public TemplateFlag<std::string>
{
protected:
	std::string from_string() override;

public:
	StringFlag(
		const std::string& label, const std::string& help, const std::string& default_val
	);
	bool valid() final;
};

class BoolFlag final : public TemplateFlag<bool>
{
protected:
	bool from_string() override;

public:
	BoolFlag(const std::string& label, const std::string& help, bool default_val);
	bool valid() final;
};

__FLAGS_END__
