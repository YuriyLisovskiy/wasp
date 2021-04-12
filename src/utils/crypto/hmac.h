/**
 * utils/crypto/hmac.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: a collection of hmac functions.
 */

#pragma once

// C++ libraries.
#include <functional>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./abc.h"


__CRYPTO_BEGIN__

// TESTME: HMAC
// TODO: docs
// TODO: consider adding hash function as template argument
class HMAC
{
private:
	size_t _size;
	size_t _block_size;
	unsigned char* _o_pad;
	unsigned char* _i_pad;
	IHash* _outer;
	IHash* _inner;

	void _prepare_outer();

public:
	HMAC(const HMAC& other) = delete;

	explicit HMAC(std::string key, IHash* hash_func=nullptr);

	virtual ~HMAC();

	void update(const unsigned char input[], unsigned int n);

	void update(const char input[], unsigned int n);

	void update(const std::string& input);

	[[nodiscard]] size_t size() const;

	[[nodiscard]] size_t block_size() const;

	unsigned char* digest();

	std::string hex_digest();
};


// Return the HMAC of 'value', using a key generated from key_salt and a
// secret. Default algorithm is MD5, but any algorithm witch inherits
// IHash is supported and can be passed. A different key_salt should be
// passed in for every application of HMAC.
extern std::shared_ptr<HMAC> salted_hmac(
	const std::string& salt,
	const std::string& value,
	const std::string& secret_key,
	IHash* hash_func = nullptr
);

__CRYPTO_END__
