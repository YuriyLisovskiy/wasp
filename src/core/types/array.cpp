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
 * An implementation of core/types/array.h
 */

// C++ libraries.
// TODO:

// Header.
#include "./array.h"

// Framework modules.
// TODO:


__TYPES_BEGIN__

void Array::_check_index(size_type i) const
{
	if (i >= this->_arr->size())
	{
		throw std::out_of_range("index is out of range: " + std::to_string(i));
	}
}

signed char Array::_cmp(const value_type& left, const value_type& right)
{
	if (left == right)
	{
		return 0;
	}

	return left > right ? 1 : -1;
}

Array::Array(size_type size)
{
	this->_arr = std::make_shared<std::vector<value_type>>(size);
}

Array::Array(const std::vector<value_type>& other)
{
	this->_arr = std::make_shared<std::vector<value_type>>(other);
}

Array& Array::operator= (const std::vector<value_type>& other)
{
	this->_arr = std::make_shared<std::vector<value_type>>(other);
	return *this;
}

Array& Array::operator= (const Array& other)
{
	if (this != &other)
	{
		this->_arr = other._arr;
	}

	return *this;
}

Array::iterator Array::begin()
{
	return this->_arr->begin();
}

Array::const_iterator Array::begin() const
{
	return this->_arr->begin();
}

Array::const_iterator Array::cbegin() const noexcept
{
	return this->_arr->cbegin();
}

Array::reverse_iterator Array::rbegin()
{
	return this->_arr->rbegin();
}

Array::const_reverse_iterator Array::rbegin() const
{
	return this->_arr->rbegin();
}

Array::const_reverse_iterator Array::crbegin() const noexcept
{
	return this->_arr->crbegin();
}

Array::iterator Array::end()
{
	return this->_arr->end();
}

Array::const_iterator Array::end() const
{
	return this->_arr->end();
}

Array::const_iterator Array::cend() const noexcept
{
	return this->_arr->cend();
}

Array::reverse_iterator Array::rend()
{
	return this->_arr->rend();
}

Array::const_reverse_iterator Array::rend() const
{
	return this->_arr->rend();
}

Array::const_reverse_iterator Array::crend() const noexcept
{
	return this->_arr->crend();
}

Array::size_type Array::size() const noexcept
{
	return this->_arr->size();
}

Array::size_type Array::max_size() const noexcept
{
	return this->_arr->max_size();
}

void Array::resize(size_type new_size)
{
	this->_arr->resize(new_size);
}

void Array::resize(size_type new_size, const value_type& val)
{
	this->_arr->resize(new_size, val);
}

Array::size_type Array::capacity() const noexcept
{
	return this->_arr->capacity();
}

bool Array::empty() const noexcept
{
	return this->_arr->empty();
}

void Array::reserve(size_type n)
{
	this->_arr->reserve(n);
}

void Array::shrink_to_fit()
{
	this->_arr->shrink_to_fit();
}

Array::reference Array::operator[] (size_type i)
{
	this->_check_index(i);
	return (*this->_arr)[i];
}

Array::const_reference Array::operator[] (size_type i) const
{
	this->_check_index(i);
	return (*this->_arr)[i];
}

Array::reference Array::at(size_type i)
{
	this->_check_index(i);
	return this->_arr->at(i);
}

Array::const_reference Array::at(size_type i) const
{
	this->_check_index(i);
	return this->_arr->at(i);
}

Array::reference Array::front()
{
	return this->_arr->front();
}

Array::const_reference Array::front() const
{
	return this->_arr->front();
}

Array::reference Array::back()
{
	return this->_arr->back();
}

Array::const_reference Array::back() const
{
	return this->_arr->back();
}

Array::value_type* Array::data() noexcept
{
	return this->_arr->data();
}

const Array::value_type* Array::data() const noexcept
{
	return this->_arr->data();
}

void Array::assign(size_type n, const value_type& val)
{
	this->_arr->assign(n, val);
}

void Array::assign(std::initializer_list<value_type> il)
{
	this->_arr->assign(il);
}

void Array::push_back(const value_type& val)
{
	this->_arr->push_back(val);
}

void Array::push_back(const char* val)
{
	this->_arr->push_back(std::make_shared<Value<const char*>>(val));
}

void Array::push_back(value_type&& val)
{
	this->_arr->push_back(std::forward<value_type>(val));
}

void Array::pop_back() noexcept
{
	this->_arr->pop_back();
}

Array::iterator Array::insert(const_iterator position, const value_type& val)
{
	return this->_arr->insert(position, val);
}

Array::iterator Array::insert(const_iterator position, const char* val)
{
	return this->_arr->insert(position, std::make_shared<Value<const char*>>(val));
}

Array::iterator Array::insert(const_iterator position, size_type n, const value_type& val)
{
	return this->_arr->insert(position, n, val);
}

Array::iterator Array::insert(const_iterator position, size_type n, const char* val)
{
	return this->_arr->insert(position, n, std::make_shared<Value<const char*>>(val));
}

Array::iterator Array::insert(const_iterator position, value_type&& val)
{
	return this->_arr->insert(position, std::forward<value_type>(val));
}

Array::iterator Array::insert(const_iterator position, std::initializer_list<value_type> il)
{
	return this->_arr->insert(position, il);
}

Array::iterator Array::erase(const_iterator position)
{
	return this->_arr->erase(position);
}

Array::iterator Array::erase(const_iterator first, const_iterator last)
{
	return this->_arr->erase(first, last);
}

void Array::swap(Array& x) noexcept
{
	this->_arr->swap(*x._arr);
}

void Array::swap(std::vector<value_type>& x) noexcept
{
	this->_arr->swap(x);
}

void Array::clear() noexcept
{
	this->_arr->clear();
}

Array::allocator_type Array::get_allocator() const noexcept
{
	return this->_arr->get_allocator();
}

bool operator== (const Array& lhs, const Array& rhs)
{
	return lhs._arr == rhs._arr;
}

bool operator!= (const Array& lhs, const Array& rhs)
{
	return lhs._arr != rhs._arr;
}

bool operator< (const Array& lhs, const Array& rhs)
{
	return lhs._arr < rhs._arr;
}

bool operator<= (const Array& lhs, const Array& rhs)
{
	return lhs._arr <= rhs._arr;
}

bool operator> (const Array& lhs, const Array& rhs)
{
	return lhs._arr > rhs._arr;
}

bool operator>= (const Array& lhs, const Array& rhs)
{
	return lhs._arr >= rhs._arr;
}

std::string Array::__str__() const
{
	std::string res = "{";
	for (auto it = this->_arr->begin(); it != this->_arr->end(); it++)
	{
		auto item = *it;
		if (item)
		{
			res += item->__repr__();
		}
		else
		{
			res += "nullptr";
		}

		if (it < this->_arr->end() - 1)
		{
			res += ", ";
		}
	}

	return res + "}";
}

std::string Array::__repr__() const
{
	return this->__str__();
}

Array::operator bool () const
{
	return !this->_arr->empty();
}

void Array::reverse()
{
	std::reverse(this->_arr->begin(), this->_arr->end());
}

__TYPES_END__
