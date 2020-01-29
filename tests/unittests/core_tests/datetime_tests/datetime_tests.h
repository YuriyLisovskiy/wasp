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

#ifndef WASP_UNIT_TESTS_CORE_TESTS_DATETIME_TESTS_DATETIME_TESTS_INCLUDE_H
#define WASP_UNIT_TESTS_CORE_TESTS_DATETIME_TESTS_DATETIME_TESTS_INCLUDE_H

#include <iostream>
#include <gtest/gtest.h>

#include "../../_def_.h"
#include "../../../../src/core/datetime/datetime.h"


__UNIT_TESTS_BEGIN__

// FIXME: TEST(DateTimeTestCase, StrptimeTest)
TEST(DateTimeTestCase, StrptimeTest)
{
	const char* str_dt = "Tue, 15 Nov 2019 12:45:26 EET";
	auto dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
//	auto dt = core::dt::DateTime();
	auto strf_time = dt.strftime("%a, %e %b %Y %T %Z");

	std::cout << "Original: " << str_dt << '\n';
	std::cout << "Strftime: " << strf_time << '\n';
//	auto e = dt.utc_epoch();
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_CORE_TESTS_DATETIME_TESTS_DATETIME_TESTS_INCLUDE_H
