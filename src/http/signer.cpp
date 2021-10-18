/**
 * utility/crypto/signer.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./signer.h"

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>


__HTTP_BEGIN__

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

std::string Signer::signature(const std::string& value, crypto::abc::ISignatureAlgorithm* algorithm) const
{
	std::unique_ptr<crypto::abc::ISignatureAlgorithm> default_algorithm = nullptr;
	if (!algorithm)
	{
		default_algorithm = _get_default_algorithm("");
	}

	return salted_hmac_hex_digest(
		this->_salt + "signer", value, this->_key, algorithm ? algorithm : default_algorithm.get()
	);
}

std::string Signer::unsign(const std::string& signed_value, crypto::abc::ISignatureAlgorithm* algorithm)
{
	if (signed_value.find(this->_sep) == std::string::npos)
	{
		throw BadSignature("no '" + this->_str_sep + "' found in value", _ERROR_DETAILS_);
	}

	auto value_sig = str::rsplit(signed_value, this->_sep, 1);
	if (value_sig[1] == this->signature(value_sig[0], algorithm))
	{
		return value_sig[0];
	}

	throw BadSignature("signature '" + this->_str_sep + "' does not match", _ERROR_DETAILS_);
}

std::string salted_hmac_hex_digest(
	const std::string& salt,
	const std::string& value,
	const std::string& secret_key,
	crypto::abc::ISignatureAlgorithm* algorithm
)
{
	if (secret_key.empty())
	{
		throw ValueError("'secret_key' can not be empty", _ERROR_DETAILS_);
	}

	std::unique_ptr<crypto::abc::ISignatureAlgorithm> default_algorithm = nullptr;
	if (!algorithm)
	{
		default_algorithm = _get_default_algorithm("");
		algorithm = default_algorithm.get();
	}

	// We need to generate a derived key from our base key.
	auto hash_function = algorithm->get_digest_function();
	if (!hash_function)
	{
		throw NullPointerException(
			"Digest function of '" + algorithm->get_name() + "' signature algorithm is nullptr",
			_ERROR_DETAILS_
		);
	}

	auto salted_key = hash_function(salt + secret_key);
	algorithm->set_secret_key(salted_key);
	auto digest = algorithm->sign_to_hex(value);
	algorithm->reset_secret_key();
	return digest;
}

__HTTP_END__
