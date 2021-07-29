/**
 * utils/crypto/hmac.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * HMAC hashing implementation.
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
class HMAC
{
private:
	size_t _size;
	size_t _block_size;
	unsigned char* _o_pad;
	unsigned char* _i_pad;
	abc::IHash* _outer;
	abc::IHash* _inner;

	void _prepare_outer();

public:
	HMAC(const HMAC& other) = delete;

	explicit HMAC(std::string key, abc::IHash* hash_func=nullptr);

	virtual ~HMAC();

	inline void update(const unsigned char input[], unsigned int n)
	{
		this->_inner->update(input, n);
	}

	inline void update(const char input[], unsigned int n)
	{
		this->_inner->update(input, n);
	}

	inline void update(const std::string& input)
	{
		this->_inner->update(input);
	}

	[[nodiscard]]
	inline size_t size() const
	{
		return this->_size;
	}

	[[nodiscard]]
	inline size_t block_size() const
	{
		return this->_block_size;
	}

	inline unsigned char* digest()
	{
		this->_prepare_outer();
		return this->_outer->digest();
	}

	inline std::string hex_digest()
	{
		this->_prepare_outer();
		return this->_outer->hex_digest();
	}
};

// TESTME: salted_hmac
// Returns the 'HMAC' of 'value', using a key generated from 'salt' and a
// 'secret_key'. Default algorithm is 'MD5', but any algorithm witch inherits
// 'abc::IHash' is supported and can be passed. A different 'salt' should be
// passed in for every application of 'HMAC'.
extern std::shared_ptr<HMAC> salted_hmac(
	const std::string& salt,
	const std::string& value,
	const std::string& secret_key,
	abc::IHash* hash_func = nullptr
);

__CRYPTO_END__
