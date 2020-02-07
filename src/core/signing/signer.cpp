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
 * An implementation of core/signing/signer.h
 */

#include "./signer.h"


__SIGNING_INTERNAL_BEGIN__

rgx::Regex _SEP_UNSAFE_REGEX = rgx::Regex(R"([A-z0-9-_=]*)");

__SIGNING_INTERNAL_END__


__SIGNING_BEGIN__

Signer::Signer(
	const std::string& key, char sep, const std::string& salt
)
{
	if (key.empty())
	{
		throw ValueError("Signer: key must not be empty", _ERROR_DETAILS_);
	}

	this->_key = key;
	this->_sep = sep;
	this->_str_sep = std::string(1, this->_sep);
	auto sep_unsafe_regex = internal::_SEP_UNSAFE_REGEX;
	if (sep_unsafe_regex.match(this->_str_sep))
	{
		throw ValueError(
			"Unsafe Signer separator: " + this->_str_sep + " (cannot be empty or consist of only A-z0-9-_=)",
			_ERROR_DETAILS_
		);
	}

	if (salt.empty())
	{
		this->_salt = "wasp.core.signing.Signer";
	}
	else
	{
		this->_salt = salt;
	}
}

std::string Signer::signature(const std::string& value)
{
	auto hmac = utils::crypto::salted_hmac(
		this->_salt + "signer", value, this->_key
	);
	auto result = hmac->hex_digest();
	delete hmac;
	return result;
}

std::string Signer::sign(const std::string& value)
{
	return value + this->_sep + this->signature(value);
}

std::string Signer::unsign(const std::string& signed_value)
{
	if (signed_value.find(this->_sep) == std::string::npos)
	{
		throw BadSignature("No \"" + this->_str_sep + "\" found in value");
	}

	auto value_sig = str::rsplit(signed_value, this->_sep, 1);
	if (value_sig[1] == this->signature(value_sig[0]))
	{
		return value_sig[0];
	}

	throw BadSignature("Signature \"" + this->_str_sep + "\" does not match");
}

__SIGNING_END__
