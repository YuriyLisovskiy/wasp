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
 * hmac.h
 * Purpose: a collection of hmac functions.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./interface.h"
#include "./md5.h"


__CRYPTO_BEGIN__

class Hmac
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
	Hmac(
		std::string key,
		std::function<IHash*()> make_hash_func = nullptr
	);
	virtual ~Hmac();
	void update(const unsigned char input[], unsigned int n);
	void update(const char input[], unsigned int n);
	void update(const std::string& input);
	size_t size();
	size_t block_size();
	unsigned char* digest();
	std::string hex_digest();
};


/// Wraps Hmac using salt.
extern std::string salted_hmac(
	const std::string& salt,
	const std::string& value,
	const std::string& key,
	IHash* hash_func = nullptr
);

__CRYPTO_END__
