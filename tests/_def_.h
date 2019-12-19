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

#ifndef WASP_TESTS_DEF_H
#define WASP_TESTS_DEF_H


// Memory leak detection.
#define DETECT_MEMORY_LEAK
#include "./mem_leak_check.h"

#define __TESTS_BEGIN__ namespace wasp { namespace tests {
#define __TESTS_END__ } }


#endif // WASP_TESTS_DEF_H
