/**
 * core/signing/signer.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: used for signing string.
 */

#pragma once

// Core libraries.
#include <xalwart.core/regex.h>

// Module definitions.
#include "./_def_.h"


__SIGNING_INTERNAL_BEGIN__

extern rgx::Regex _SEP_UNSAFE_REGEX;

__SIGNING_INTERNAL_END__


__SIGNING_BEGIN__

class Signer
{
private:
	std::string _key;
	char _sep;
	std::string _str_sep;
	std::string _salt;

public:
	explicit Signer(
		const std::string& key,
		char sep = ':',
		const std::string& salt = ""
	);
	std::string signature(const std::string& value);
	std::string sign(const std::string& value);
	std::string unsign(const std::string& signed_value);
};

__SIGNING_END__
