/**
 * core/signing/signer.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * String signer.
 */

#pragma once

// Base libraries.
#include <xalwart.base/re/regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../utils/crypto/abc.h"


__CORE_SIGNING_BEGIN__

inline re::Regex _SEP_UNSAFE_REGEX = re::Regex(R"([A-z0-9-_=]*)");

// TODO: docs
// TESTME: Signer
class Signer final
{
private:
	std::string _key;
	char _sep;
	std::string _str_sep;
	std::string _salt;
	crypto::IHash* _hash_func;

public:
	explicit Signer(
		const std::string& key, char sep=':', const std::string& salt="",
		crypto::IHash* hash_func=nullptr
	);

	std::string signature(const std::string& value);

	inline std::string sign(const std::string& value)
	{
		return value + this->_sep + this->signature(value);
	}

	std::string unsign(const std::string& signed_value);
};

__CORE_SIGNING_END__
