/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * core/signing/signer.h
 *
 * Purpose: used for signing string.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../regex.h"
#include "../string/str.h"
#include "../exceptions.h"
#include "../../utils/crypto/hmac.h"


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
