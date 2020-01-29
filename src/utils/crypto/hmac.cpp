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
 * An implementation of hmac.h.
 */

#include "./hmac.h"


__CRYPTO_BEGIN__

Hmac::Hmac(
	std::string key,
	std::function<IHash*()> make_hash_func
)
{
	if (!make_hash_func)
	{
		make_hash_func = []() -> IHash* { return new MD5(); };
	}

	this->_outer = make_hash_func();
	if (!this->_outer)
	{
		make_hash_func = []() -> IHash* { return new MD5(); };
		this->_outer = make_hash_func();
	}

	this->_inner = make_hash_func();
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

Hmac::~Hmac()
{
	delete[] this->_o_pad;
	delete[] this->_i_pad;
	delete this->_outer;
	delete this->_inner;
}

void Hmac::update(const unsigned char input[], unsigned int n)
{
	this->_inner->update(input, n);
}

void Hmac::update(const char input[], unsigned int n)
{
	this->_inner->update(input, n);
}

void Hmac::update(const std::string& input)
{
	this->_inner->update(input);
}

size_t Hmac::size()
{
	return this->_size;
}

size_t Hmac::block_size()
{
	return this->_block_size;
}

unsigned char* Hmac::digest()
{
	this->_prepare_outer();
	return this->_outer->digest();
}

std::string Hmac::hex_digest()
{
	this->_prepare_outer();
	return this->_outer->hex_digest();
}

void Hmac::_prepare_outer()
{
	auto inner_sum = this->_inner->digest();
	this->_outer->reset();
	this->_outer->update(this->_o_pad, this->_block_size);
	this->_outer->update(inner_sum, this->_inner->size());
}


std::string salted_hmac(
	const std::string& salt,
	const std::string& value,
	const std::string& key,
	IHash* hash_func
)
{
	// TODO: implement salted_hmac
	return value;
}

__CRYPTO_END__
