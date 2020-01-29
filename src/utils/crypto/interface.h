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
 * interface.h
 * Purpose:
 * 	A collection of interfaces for cryptography module.
 */

#pragma once

// Module definitions.
#include "./_def_.h"


__CRYPTO_BEGIN__

class IHash
{
public:
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
