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


__CORE_FLAGS_INTERNAL_BEGIN__

const std::regex BOOL_TYPE("true|TRUE|false|FALSE|yes|YES|no|NO|1|0");
const std::regex LONG_INT_TYPE("[+-]?[0-9]{1,10}");
const std::regex UNSIGNED_LONG_INT_TYPE("[+]?[0-9]{1,10}");
const std::regex UINT16_TYPE("[+-]?[0-9]{1,5}");
const std::regex DOUBLE_TYPE("[+-]?[0-9]+[.]?[0-9]+");

__CORE_FLAGS_INTERNAL_END__


__CORE_FLAGS_BEGIN__

class UnsignedLongFlag final : public TemplateFlag<unsigned long>
{
protected:
	unsigned long from_string() override;

public:
	UnsignedLongFlag(
		const std::string& label, const std::string& help, unsigned long default_val
	);
	bool valid() final;
};

class Uint16Flag final : public TemplateFlag<uint16_t>
{
protected:
	uint16_t from_string() override;

public:
	Uint16Flag(
		const std::string& label, const std::string& help, uint16_t default_val
	);
	bool valid() final;
};

class LongIntFlag final : public TemplateFlag<long int>
{
protected:
	long int from_string() override;

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

class StringFlag final : public TemplateFlag<std::basic_string<char>>
{
protected:
	std::string from_string() override;

public:
	StringFlag(
		const std::string& label, const std::string& help, const std::basic_string<char>& default_val
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

__CORE_FLAGS_END__
