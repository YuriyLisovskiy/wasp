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
#include "../../utility/crypto/abc.h"


__CORE_SIGNING_BEGIN__

inline const re::Regex _SEP_UNSAFE_REGEX = re::Regex(R"([A-z0-9-_=]*)");

// TESTME: Signer
// TODO: docs for 'Signer'
class Signer final
{
private:
	std::string _key;
	char _sep;
	std::string _str_sep;
	std::string _salt;
	util::crypto::abc::IHash* _hash_func;

public:
	explicit Signer(
		const std::string& key, char sep=':', const std::string& salt="",
		util::crypto::abc::IHash* hash_func=nullptr
	);

	[[nodiscard]]
	std::string signature(const std::string& value) const;

	[[nodiscard]]
	inline std::string sign(const std::string& value) const
	{
		return value + this->_sep + this->signature(value);
	}

	std::string unsign(const std::string& signed_value);
};

__CORE_SIGNING_END__
