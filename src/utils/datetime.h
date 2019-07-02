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

/*
 * datetime
 * TODO: write docs.
 */

#ifndef WASP_UTILS_DATETIME_H
#define WASP_UTILS_DATETIME_H

#include <chrono>

#include "../globals.h"


__WASP_BEGIN__

namespace datetime
{

template <typename _TimeT = std::chrono::milliseconds>
class Measure
{
private:
	std::chrono::high_resolution_clock::time_point _begin;
	std::chrono::high_resolution_clock::time_point _end;

public:
	void start()
	{
		this->_begin = std::chrono::high_resolution_clock::now();
	}

	void end()
	{
		this->_end = std::chrono::high_resolution_clock::now();
	}

	double elapsed()
	{
		return std::chrono::duration_cast<_TimeT>(this->_end - this->_begin).count();
	}

	void reset()
	{
		this->_begin = 0;
		this->_end = 0;
	}
};

}

__WASP_END__


#endif // WASP_UTILS_DATETIME_H
