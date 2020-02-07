/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)

   based on:

   md5.h and md5.c
   reference implementation of RFC 1321

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All rights reserved.
   Copyright (C) 2020 Yuriy Lisovskiy

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

/**
 * utils/crypto/md5.h
 *
 * Purpose:
 * 	Provides md5 hash function algorithm.
 */

#pragma once

// C++ libraries.
#include <string>
#include <cstring>

// Module definitions.
#include "./_def_.h"

// Waps libraries.
#include "./interfaces.h"


__CRYPTO_BEGIN__

// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

class MD5 final : public IHash
{
private:
	typedef unsigned char uint8;
	typedef unsigned int uint32;

	const static size_t BLOCK_SIZE = 64;

	const static size_t SIZE = 16;

	// For excluding multiple finalization.
	bool _finalized;

	// Bytes that didn't fit in last 64 byte chunk.
	uint8 _buffer[BLOCK_SIZE];

	// 64-bit counter for number of bits (lo, hi).
	uint32 _count[2];

	// Digest so far.
	uint32 _state[4];

	// The result.
	uint8 _digest[SIZE];

public:
	MD5();
	explicit MD5(const std::string& input);
	MD5* clone() final;

	// Continues an MD5 message-digest operation,
	// processing another message block.
	void update(const unsigned char input[], unsigned int n) final;

	// For convenience provide a version with signed char.
	void update(const char input[], unsigned int n) final;

	// For convenience provide a version with std::string.
	void update(const std::string& input) final;

	// Hex representation of digest as string.
	std::string hex_digest() final;

	// Hex representation of digest as unsigned char array.
	unsigned char* digest() final;

	void reset() final;
	size_t size() final;
	size_t block_size() final;

private:
	void _init();

	// Ends an MD5 message-digest operation, writing the
	// the message digest and zeroizing the context.
	void _finalize();

	// Applies MD5 algorithm on a block.
	void _transform(const uint8 block[BLOCK_SIZE]);

	// Encodes input (uint4) into output (unsigned char).
	// Assumes len is a multiple of 4.
	static void _encode(uint8 output[], const uint32 input[], unsigned int n);

	// Decodes input (unsigned char) into output (uint4).
	// Assumes len is a multiple of 4.
	static void _decode(uint32 output[], const uint8 input[], unsigned int n);

	// Basic MD5 functions.
	static inline uint32 F(uint32 x, uint32 y, uint32 z);
	static inline uint32 G(uint32 x, uint32 y, uint32 z);
	static inline uint32 H(uint32 x, uint32 y, uint32 z);
	static inline uint32 I(uint32 x, uint32 y, uint32 z);

	// Rotates x left n bits.
	static inline uint32 rotate_left(uint32 x, int n);

	// Transformations for 1-st round.
	static inline void FF(uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);

	// Transformations for 2-nd round.
	static inline void GG(uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);

	// Transformations for 4-rd round.
	static inline void HH(uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);

	// Transformations for 4-th round.
	static inline void II(uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
};

__CRYPTO_END__
