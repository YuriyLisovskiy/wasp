/**
 * utility/crypto/signer.h
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
#include "./abc.h"


__CRYPTO_BEGIN__

inline static const re::Regex SEP_UNSAFE_REGEX = re::Regex(R"([A-z0-9-_=]*)");

// TESTME: Signer
// TODO: docs for 'Signer'
class Signer final
{
public:
	explicit Signer(const std::string& key, char sep=':', const std::string& salt="");

	[[nodiscard]]
	std::string signature(const std::string& value, abc::IHash* hash_function=nullptr) const;

	[[nodiscard]]
	inline std::string sign(const std::string& value, abc::IHash* hash_function=nullptr) const
	{
		return value + this->_sep + this->signature(value, hash_function);
	}

	std::string unsign(const std::string& signed_value, abc::IHash* hash_function=nullptr);

private:
	std::string _key;
	char _sep;
	std::string _str_sep;
	std::string _salt;
};

__CRYPTO_END__
