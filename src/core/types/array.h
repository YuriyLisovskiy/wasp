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
 * core/types/array.h
 *
 * Purpose:
 *  TODO: implement docs for array.h
 */

#pragma once

// C++ libraries.
#include <memory>
#include <vector>
#include <exception>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../object/object.h"
#include "../types/value.h"
#include "../types/utility.h"


__TYPES_BEGIN__

class Array : public object::Object
{
private:
	typedef std::shared_ptr<object::Object> val_t;

	std::shared_ptr<std::vector<val_t>> _arr;

public:
	typedef val_t value_type;

	typedef std::allocator<value_type> allocator_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef typename std::allocator_traits<allocator_type>::pointer pointer;
	typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;

	typedef typename std::vector<value_type>::iterator iterator;
	typedef typename std::vector<value_type>::const_iterator const_iterator;

	typedef typename std::reverse_iterator<iterator> reverse_iterator;
	typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef typename std::iterator_traits<iterator>::difference_type difference_type;
	typedef typename std::vector<value_type>::size_type size_type;

private:
	void _check_index(size_type i) const;

	static signed char _cmp(const value_type& left, const value_type& right);

public:
	explicit Array(size_type size = 0);
	explicit Array(const std::vector<value_type>& other);

	template <class T, typename = std::enable_if<std::is_fundamental<T>::value>>
	explicit Array(const std::vector<T>& other)
	{
		if (this->_arr)
		{
			this->_arr->clear();
		}
		else
		{
			this->_arr = std::make_shared<std::vector<value_type>>();
		}

		for (const auto& o : other)
		{
			this->_arr->push_back(std::make_shared<Value<T>>(o));
		}
	}

	Array& operator= (const std::vector<value_type>& other);
	Array& operator= (const Array& other);

	template <class T, typename = std::enable_if<std::is_fundamental<T>::value>>
	Array& operator= (const std::vector<T>& other)
	{
		this->_arr->clear();
		for (const auto& o : other)
		{
			this->_arr->push_back(std::make_shared<Value<T>>(o));
		}

		return *this;
	}

	iterator begin();
	[[nodiscard]] const_iterator begin() const;
	[[nodiscard]] const_iterator cbegin() const noexcept;
	reverse_iterator rbegin();
	[[nodiscard]] const_reverse_iterator rbegin() const;
	[[nodiscard]] const_reverse_iterator crbegin() const noexcept;

	iterator end();
	[[nodiscard]] const_iterator end() const;
	[[nodiscard]] const_iterator cend() const noexcept;
	reverse_iterator rend();
	[[nodiscard]] const_reverse_iterator rend() const;
	[[nodiscard]] const_reverse_iterator crend() const noexcept;

	[[nodiscard]] size_type size() const noexcept;

	[[nodiscard]] size_type max_size() const noexcept;

	void resize(size_type new_size);
	void resize(size_type new_size, const value_type& val);

	[[nodiscard]] size_type capacity() const noexcept;

	[[nodiscard]] bool empty() const noexcept;

	void reserve(size_type n);

	void shrink_to_fit();

	reference operator[] (size_type i);
	const_reference operator[] (size_type i) const;

	reference at(size_type i);
	[[nodiscard]] const_reference at(size_type i) const;

	reference front();
	[[nodiscard]] const_reference front() const;

	reference back();
	[[nodiscard]] const_reference back() const;

	value_type* data() noexcept;
	[[nodiscard]] const value_type* data() const noexcept;

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		this->_arr->assign(first, last);
	}

	void assign(size_type n, const value_type& val);
	void assign(std::initializer_list<value_type> il);

	template <class T>
	void push_back(const T& val)
	{
		if constexpr (std::is_fundamental<T>::value || std::is_same<std::string, T>::value)
		{
			this->_arr->push_back(std::make_shared<Value<T>>(val));
		}
		else if constexpr (std::is_base_of<Object, T>::value)
		{
			this->_arr->push_back(std::make_shared<T>(val));
		}
		else
		{
			throw std::invalid_argument("type mismatch");
		}
	}

	void push_back(const value_type& val);
	void push_back(const char* val);
	void push_back(value_type&& val);

	void pop_back() noexcept;

	// single element (1)
	iterator insert(const_iterator position, const value_type& val);

	// single element (fundamentals or Object-inherited) (2)
	template <class T>
	iterator insert(const_iterator position, const T& val)
	{
		if constexpr (std::is_fundamental<T>::value || std::is_same<std::string, T>::value)
		{
			return this->_arr->insert(position, std::make_shared<Value<T>>(val));
		}
		else if constexpr (std::is_base_of<Object, T>::value)
		{
			return this->_arr->insert(position, std::make_shared<T>(val));
		}
		else
		{
			throw std::invalid_argument("type mismatch");
		}
	}

	// single element (c-string) (3)
	iterator insert(const_iterator position, const char* val);

	// fill (4)
	iterator insert(const_iterator position, size_type n, const value_type& val);

	// fill (fundamental or Object-inherited) (5)
	template <class T>
	iterator insert(const_iterator position, size_type n, const T& val)
	{
		if constexpr (std::is_fundamental<T>::value)
		{
			return this->_arr->insert(position, n, std::make_shared<Value<T>>(val));
		}
		else if constexpr (std::is_base_of<Object, T>::value)
		{
			return this->_arr->insert(position, n, std::make_shared<T>(val));
		}
		else
		{
			throw std::invalid_argument("type mismatch");
		}
	}

	// fill (c-string) (6)
	iterator insert(const_iterator position, size_type n, const char* val);

	// range (7)
	template <class InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last)
	{
		return this->_arr->insert(position, first, last);
	}

	// move (8)
	iterator insert(const_iterator position, value_type&& val);

	// initializer list (9)
	iterator insert(const_iterator position, std::initializer_list<value_type> il);

	iterator erase(const_iterator position);
	iterator erase(const_iterator first, const_iterator last);

	void swap(Array& x) noexcept;
	void swap(std::vector<value_type>& x) noexcept;

	void clear() noexcept;

	template <class... Args>
	iterator emplace(const_iterator position, Args&&... args)
	{
		return this->_arr->emplace(position, std::forward<Args>(args)...);
	}

	template <class... Args>
	void emplace_back(Args&&... args)
	{
		this->_arr->emplace_back(std::forward<Args>(args)...);
	}

	[[nodiscard]] allocator_type get_allocator() const noexcept;

	friend bool operator== (const Array& lhs, const Array& rhs);
	friend bool operator!= (const Array& lhs, const Array& rhs);
	friend bool operator< (const Array& lhs, const Array& rhs);
	friend bool operator<= (const Array& lhs, const Array& rhs);
	friend bool operator> (const Array& lhs, const Array& rhs);
	friend bool operator>= (const Array& lhs, const Array& rhs);

	[[nodiscard]] std::string __str__() const;

	explicit operator bool () const override;

	void reverse();
};

__TYPES_END__
