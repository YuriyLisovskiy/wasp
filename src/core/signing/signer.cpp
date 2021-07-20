/**
 * core/signing/signer.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./signer.h"

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "../../utils/crypto/hmac.h"


__CORE_SIGNING_BEGIN__

Signer::Signer(
	const std::string& key, char sep, const std::string& salt, crypto::IHash* hash_func
)
{
	if (key.empty())
	{
		throw ValueError("Signer: key must not be empty", _ERROR_DETAILS_);
	}

	this->_hash_func = hash_func;
	this->_key = key;
	this->_sep = sep;
	this->_str_sep = std::string(1, this->_sep);
	auto sep_unsafe_regex = _SEP_UNSAFE_REGEX;
	if (sep_unsafe_regex.match(this->_str_sep))
	{
		throw ValueError(
			"Unsafe Signer separator: " + this->_str_sep + " (cannot be empty or consist of only A-z0-9-_=)",
			_ERROR_DETAILS_
		);
	}

	if (salt.empty())
	{
		this->_salt = "core::signing::Signer";
	}
	else
	{
		this->_salt = salt;
	}
}

std::string Signer::signature(const std::string& value)
{
	return crypto::salted_hmac(
		this->_salt + "signer", value, this->_key, this->_hash_func
	)->hex_digest();
}

std::string Signer::unsign(const std::string& signed_value)
{
	if (signed_value.find(this->_sep) == std::string::npos)
	{
		throw BadSignature(
			"no \"" + this->_str_sep + "\" found in value", _ERROR_DETAILS_
		);
	}

	auto value_sig = str::rsplit(signed_value, this->_sep, 1);
	if (value_sig[1] == this->signature(value_sig[0]))
	{
		return value_sig[0];
	}

	throw BadSignature(
		"signature \"" + this->_str_sep + "\" does not match", _ERROR_DETAILS_
	);
}

__CORE_SIGNING_END__
