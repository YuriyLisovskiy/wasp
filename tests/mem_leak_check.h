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

#ifndef MEM_LEAK_CHECK_H
#define MEM_LEAK_CHECK_H

#include <map>
#include <memory>
#include <iostream>
#include <exception>


#ifdef DETECT_MEMORY_LEAK

template<typename T>
struct track_alloc : public std::allocator<T>
{
	typedef typename std::allocator<T>::pointer pointer;
	typedef typename std::allocator<T>::size_type size_type;

	template<typename U>
	struct rebind
	{
		typedef track_alloc<U> other;
	};

	track_alloc() = default;

	template<typename U>
	explicit track_alloc(track_alloc<U> const& u) :std::allocator<T>(u)
	{
	}

	pointer allocate(size_type size, std::allocator<void>::const_pointer = 0)
	{
		void* p = std::malloc(size * sizeof(T));
		if (p == nullptr)
		{
			throw std::bad_alloc();
		}

		return static_cast<pointer>(p);
	}

	void deallocate(pointer p, size_type)
	{
		std::free(p);
	}
};

typedef std::map<void*, std::size_t, std::less<>, track_alloc<std::pair<void* const, std::size_t>>> track_type;

struct track_printer
{
	track_type* track;

	explicit track_printer(track_type * track) : track(track)
	{
	}

	~track_printer()
	{
		auto it = track->begin();
		while(it != track->end())
		{
			std::cerr << "Memory leak of " << it->second << " byte(s) detected at " << it->first << ".\n";
			++it;
		}
	}
};

track_type* get_map()
{
	// don't use normal new to avoid infinite recursion.
	static track_type* track = new (std::malloc(sizeof(*track))) track_type;
	static track_printer printer(track);
	return track;
}

void* operator new(std::size_t size) noexcept(false)
{
	// we are required to return non-null
	void* mem = std::malloc(size == 0 ? 1 : size);
	if (mem == nullptr)
	{
		throw std::bad_alloc();
	}

	(*get_map())[mem] = size;
	return mem;
}

void operator delete(void* mem) throw()
{
	if (get_map()->erase(mem) == 0)
	{
		// this indicates a serious bug
		std::cerr << "bug: memory at " << mem << " wasn't allocated by us\n";
	}

	std::free(mem);
}

#endif // DETECT_MEMORY_LEAK


#endif // MEM_LEAK_CHECK_H
