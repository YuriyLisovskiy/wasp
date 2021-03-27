/**
 * utils/crypto/abc.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: abstract base classes for cryptography module.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__CRYPTO_BEGIN__

class IHash
{
private:
	IHash(const IHash&) = default;
	IHash& operator=(const IHash&) = default;

public:
	IHash() = default;

	virtual IHash* clone() = 0;

	// Continues message-digest operation,
	// processing another message block.
	virtual void update(const unsigned char input[], unsigned int n) = 0;

	// For convenience provide a version with signed char.
	virtual void update(const char input[], unsigned int n) = 0;

	// For convenience provide a version with std::string.
	virtual void update(const std::string& input) = 0;

	// Hex representation of digest as string.
	virtual std::string hex_digest() = 0;

	// Hex representation of digest as string.
	virtual unsigned char* digest() = 0;

	virtual void reset() = 0;
	virtual size_t size() = 0;
	virtual size_t block_size() = 0;

	virtual ~IHash() = default;
};

__CRYPTO_END__
