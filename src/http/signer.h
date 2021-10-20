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

// Crypto libraries.
#include <xalwart.crypto/abc.h>
#include <xalwart.crypto/hmac.h>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

inline static const re::Regex SEP_UNSAFE_REGEX = re::Regex(R"([A-z0-9-_=]*)");

// TESTME: Signer
// TODO: docs for 'Signer'
class Signer final
{
public:
	explicit Signer(const std::string& key, char sep=':', const std::string& salt="");

	[[nodiscard]]
	std::string signature(const std::string& value, crypto::abc::ISignatureAlgorithm* algorithm=nullptr) const;

	[[nodiscard]]
	inline std::string sign(const std::string& value, crypto::abc::ISignatureAlgorithm* algorithm=nullptr) const
	{
		return value + this->_sep + this->signature(value, algorithm);
	}

	std::string unsign(const std::string& signed_value, crypto::abc::ISignatureAlgorithm* algorithm=nullptr);

private:
	std::string _key;
	char _sep;
	std::string _str_sep;
	std::string _salt;
};

// TESTME: _get_default_algorithm
// TODO: docs for '_get_default_algorithm'
inline std::unique_ptr<crypto::abc::ISignatureAlgorithm> _get_default_algorithm(const std::string& secret_key)
{
	return std::make_unique<crypto::HMAC>(secret_key, crypto::md5, "HMAC-MD5");
}

// TESTME: salted_hmac
// Returns the 'HMAC' of 'value', using a key generated from 'salt' and a
// 'secret_key'. Default algorithm is 'MD5', but any algorithm witch inherits
// 'abc::IHash' is supported and can be passed. A different 'salt' should be
// passed in for every application of 'HMAC'.
extern std::string salted_hmac_hex_digest(
	const std::string& salt,
	const std::string& value,
	const std::string& secret_key,
	crypto::abc::ISignatureAlgorithm* algorithm=nullptr
);

__HTTP_END__
