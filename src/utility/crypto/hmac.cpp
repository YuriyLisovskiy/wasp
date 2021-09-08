/**
 * utils/crypto/hmac.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./hmac.h"

// Base libraries.
#include <xalwart.base/exceptions.h>

// Framework libraries.
#include "./md5.h"
#include "./utility.h"


__CRYPTO_BEGIN__

HMAC::HMAC(std::string key, abc::IHash* hash_func)
{
	if (hash_func)
	{
		this->_outer = hash_func->clone();
		this->_inner = hash_func->clone();
	}
	else
	{
		MD5 md5;
		this->_outer = md5.clone();
		this->_inner = md5.clone();
	}

	this->_size = this->_inner->size();
	this->_block_size = this->_inner->block_size();
	this->_i_pad = new unsigned char[this->_block_size]();
	this->_o_pad = new unsigned char[this->_block_size]();
	if (key.size() > this->_block_size)
	{
		this->_outer->update(key);
		key = this->_outer->hex_digest();
	}

	for (size_t i = 0; i < key.size(); i++)
	{
		this->_i_pad[i] = key[i];
		this->_o_pad[i] = key[i];
	}

	for (size_t i = 0; i < this->_block_size; i++)
	{
		this->_i_pad[i] ^= 0x36;
		this->_o_pad[i] ^= 0x5c;
	}

	this->_inner->update(this->_i_pad, this->_block_size);
}

HMAC::~HMAC()
{
	delete[] this->_o_pad;
	delete[] this->_i_pad;
	delete this->_outer;
	delete this->_inner;
}

void HMAC::_prepare_outer()
{
	auto inner_sum = this->_inner->digest();
	this->_outer->reset();
	this->_outer->update(this->_o_pad, this->_block_size);
	this->_outer->update(inner_sum, this->_inner->size());
}

std::string salted_hmac_hex_digest(
	const std::string& salt,
	const std::string& value,
	const std::string& secret_key,
	abc::IHash* hash_func
)
{
	if (secret_key.empty())
	{
		throw ValueError("'secret_key' can not be empty", _ERROR_DETAILS_);
	}

	std::unique_ptr<abc::IHash> default_hash_function = nullptr;
	if (!hash_func)
	{
		default_hash_function = _get_default_hash_function();
		hash_func = default_hash_function.get();
	}

	// We need to generate a derived key from our base key.  We can do this by
	// passing the key_salt and our base key through a pseudo-random function.
	hash_func->update(salt + secret_key);
	auto salted_key = hash_func->hex_digest();

	// If len(key_salt + secret) > block size of the hash algorithm, the above
	// line is redundant and could be replaced by key = key_salt + secret, since
	// the hmac module does the same thing for keys longer than the block size.
	// However, we need to ensure that we *always* do this.
	hash_func->reset();
	auto hmac = std::make_shared<HMAC>(salted_key, hash_func);

	hmac->update(value);
	return hmac->hex_digest();
}

__CRYPTO_END__
