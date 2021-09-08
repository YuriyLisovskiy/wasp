/**
 * utility/crypto/signer.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./signer.h"

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "./hmac.h"
#include "./utility.h"


__CRYPTO_BEGIN__

Signer::Signer(const std::string& key, char sep, const std::string& salt)
{
	if (key.empty())
	{
		throw ValueError("Signer: key must not be empty", _ERROR_DETAILS_);
	}

	this->_key = key;
	this->_sep = sep;
	this->_str_sep = std::string(1, this->_sep);
	auto sep_unsafe_regex = SEP_UNSAFE_REGEX;
	if (sep_unsafe_regex.match(this->_str_sep))
	{
		throw ValueError(
			"Unsafe Signer separator: " + this->_str_sep + " (cannot be empty or consist of only A-z0-9-_=)",
			_ERROR_DETAILS_
		);
	}

	this->_salt = salt.empty() ? "xw::util::crypto::Signer" : salt;
}

std::string Signer::signature(const std::string& value, abc::IHash* hash_function) const
{
	std::unique_ptr<abc::IHash> default_hash_function = nullptr;
	if (!hash_function)
	{
		default_hash_function = util::crypto::_get_default_hash_function();
	}

	return util::crypto::salted_hmac_hex_digest(
		this->_salt + "signer", value, this->_key, hash_function ? hash_function : default_hash_function.get()
	);
}

std::string Signer::unsign(const std::string& signed_value, abc::IHash* hash_function)
{
	if (signed_value.find(this->_sep) == std::string::npos)
	{
		throw BadSignature("no '" + this->_str_sep + "' found in value", _ERROR_DETAILS_);
	}

	auto value_sig = str::rsplit(signed_value, this->_sep, 1);
	if (value_sig[1] == this->signature(value_sig[0], hash_function))
	{
		return value_sig[0];
	}

	throw BadSignature("signature '" + this->_str_sep + "' does not match", _ERROR_DETAILS_);
}

__CRYPTO_END__
