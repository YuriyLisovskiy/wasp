/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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
 * An implementation of md5.h.
 */

#include "./md5.h"


__CRYPTO_BEGIN__

MD5::MD5()
{
	this->_init();
}

MD5::MD5(const std::string& input)
{
	this->_init();
	this->update(input.c_str(), input.length());
}

MD5* MD5::clone()
{
	auto* copy = new MD5();
	copy->_finalized = this->_finalized;
	for (size_t i = 0; i < MD5::BLOCK_SIZE; i++)
	{
		copy->_buffer[i] = this->_buffer[i];
	}

	copy->_count[0] = this->_count[0];
	copy->_count[1] = this->_count[1];

	copy->_state[0] = this->_state[0];
	copy->_state[1] = this->_state[1];
	copy->_state[2] = this->_state[2];
	copy->_state[3] = this->_state[3];

	for (size_t i = 0; i < MD5::SIZE; i++)
	{
		copy->_digest[i] = this->_digest[i];
	}

	return copy;
}

void MD5::update(const unsigned char input[], unsigned int n)
{
	// compute number of bytes mod 64
	unsigned int index = this->_count[0] / 8 % MD5::BLOCK_SIZE;

	// Update number of bits
	if ((this->_count[0] += (n << 3)) < (n << 3))
	{
		this->_count[1]++;
	}
	this->_count[1] += (n >> 29);

	// number of bytes we need to fill in buffer
	unsigned int first_part = 64 - index;
	unsigned int i;

	// transform as many times as possible.
	if (n >= first_part)
	{
		// fill buffer first, transform
		std::memcpy(&this->_buffer[index], input, first_part);
		this->_transform(this->_buffer);

		// transform chunks of block_size (64 bytes)
		for (i = first_part; i + MD5::BLOCK_SIZE <= n; i += MD5::BLOCK_SIZE)
		{
			this->_transform(&input[i]);
		}

		index = 0;
	}
	else
	{
		i = 0;
	}

	// buffer remaining input
	std::memcpy(&this->_buffer[index], &input[i], n - i);
}

void MD5::update(const char input[], unsigned int n)
{
	this->update((const unsigned char*) input, n);
}

void MD5::update(const std::string& input)
{
	this->update(input.c_str(), input.length());
}

std::string MD5::hex_digest()
{
	this->_finalize();
	char buf[33];
	for (int i = 0; i < 16; i++)
	{
		sprintf(buf + i * 2, "%02x", this->_digest[i]);
	}

	buf[32] = 0;

	return std::string(buf);
}

unsigned char* MD5::digest()
{
	this->_finalize();
	return this->_digest;
}

void MD5::reset()
{
	if (!this->_finalized)
	{
		std::memset(this->_buffer, 0, sizeof(this->_buffer));
		std::memset(this->_digest, 0, sizeof(this->_digest));
	}

	this->_init();
}

size_t MD5::size()
{
	return MD5::SIZE;
}

size_t MD5::block_size()
{
	return MD5::BLOCK_SIZE;
}

void MD5::_init()
{
	this->_finalized = false;

	this->_count[0] = 0;
	this->_count[1] = 0;

	// Magic initialization constants.
	this->_state[0] = 0x67452301;
	this->_state[1] = 0xefcdab89;
	this->_state[2] = 0x98badcfe;
	this->_state[3] = 0x10325476;
}

void MD5::_finalize()
{
	if (!this->_finalized)
	{
		static unsigned char padding[64] = {
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		// Save number of bits
		unsigned char bits[8];
		MD5::_encode(bits, this->_count, 8);

		// pad out to 56 mod 64.
		unsigned int index = this->_count[0] / 8 % 64;
		unsigned int padLen = (index < 56) ? (56 - index) : (120 - index);
		this->update(padding, padLen);

		// Append length (before padding)
		this->update(bits, 8);

		// Store state in digest
		MD5::_encode(this->_digest, this->_state, 16);

		// Zeroize sensitive information.
		std::memset(this->_buffer, 0, sizeof(this->_buffer));
		std::memset(this->_count, 0, sizeof(this->_count));

		this->_finalized = true;
	}
}

void MD5::_transform(const uint8 block[MD5::BLOCK_SIZE])
{
	uint32 a = this->_state[0];
	uint32 b = this->_state[1];
	uint32 c = this->_state[2];
	uint32 d = this->_state[3];
	uint32 x[16];
	MD5::_decode(x, block, MD5::BLOCK_SIZE);

	// Round 1.
	MD5::FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
	MD5::FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
	MD5::FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
	MD5::FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
	MD5::FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
	MD5::FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
	MD5::FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
	MD5::FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
	MD5::FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
	MD5::FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
	MD5::FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	MD5::FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	MD5::FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	MD5::FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	MD5::FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	MD5::FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	// Round 2.
	MD5::GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
	MD5::GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
	MD5::GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	MD5::GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
	MD5::GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
	MD5::GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
	MD5::GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	MD5::GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
	MD5::GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
	MD5::GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	MD5::GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
	MD5::GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
	MD5::GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	MD5::GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
	MD5::GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
	MD5::GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	// Round 3.
	MD5::HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
	MD5::HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
	MD5::HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	MD5::HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	MD5::HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
	MD5::HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
	MD5::HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
	MD5::HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	MD5::HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	MD5::HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
	MD5::HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
	MD5::HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
	MD5::HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
	MD5::HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	MD5::HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	MD5::HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

	// Round 4.
	MD5::II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
	MD5::II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
	MD5::II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	MD5::II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
	MD5::II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	MD5::II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
	MD5::II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	MD5::II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
	MD5::II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
	MD5::II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	MD5::II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
	MD5::II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	MD5::II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
	MD5::II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	MD5::II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
	MD5::II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

	this->_state[0] += a;
	this->_state[1] += b;
	this->_state[2] += c;
	this->_state[3] += d;
	std::memset(x, 0, sizeof x);
}

void MD5::_encode(uint8 output[], const uint32 input[], unsigned int n)
{
	for (unsigned int i = 0, j = 0; j < n; i++, j += 4)
	{
		output[j] = input[i] & 0xff;
		output[j + 1] = (input[i] >> 8) & 0xff;
		output[j + 2] = (input[i] >> 16) & 0xff;
		output[j + 3] = (input[i] >> 24) & 0xff;
	}
}

void MD5::_decode(uint32 output[], const uint8 input[], unsigned int n)
{
	for (unsigned int i = 0, j = 0; j < n; i++, j += 4)
	{
		output[i] = ((uint32) input[j]) | (((uint32) input[j + 1]) << 8) |
		            (((uint32) input[j + 2]) << 16) | (((uint32) input[j + 3]) << 24);
	}
}

inline MD5::uint32 MD5::F(uint32 x, uint32 y, uint32 z)
{
	return x & y | ~x & z;
}

inline MD5::uint32 MD5::G(uint32 x, uint32 y, uint32 z)
{
	return x & z | y & ~z;
}

inline MD5::uint32 MD5::H(uint32 x, uint32 y, uint32 z)
{
	return x ^ y ^ z;
}

inline MD5::uint32 MD5::I(uint32 x, uint32 y, uint32 z)
{
	return y ^ (x | ~z);
}

inline MD5::uint32 MD5::rotate_left(uint32 x, int n)
{
	return (x << n) | (x >> (32 - n));
}

inline void MD5::FF(uint32& a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a = MD5::rotate_left(a + MD5::F(b, c, d) + x + ac, s) + b;
}

inline void MD5::GG(uint32& a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a = MD5::rotate_left(a + MD5::G(b, c, d) + x + ac, s) + b;
}

inline void MD5::HH(uint32& a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a = MD5::rotate_left(a + MD5::H(b, c, d) + x + ac, s) + b;
}

inline void MD5::II(uint32& a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a = MD5::rotate_left(a + MD5::I(b, c, d) + x + ac, s) + b;
}

__CRYPTO_END__
