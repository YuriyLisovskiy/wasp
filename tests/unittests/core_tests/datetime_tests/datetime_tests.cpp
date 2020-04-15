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

#include <gtest/gtest.h>

#include "../../../../src/core/datetime.h"

using namespace wasp;


TEST(DateTimeTestCase, StrftimeTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 GMT";
	auto dt = core::dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	auto strf_time = dt.strftime("%a, %e %b %Y %T %Z");
	ASSERT_EQ(str_dt, strf_time);
}

TEST(DateTimeTestCase, TimestampTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 GMT";
	auto dt = core::dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	long expected = 1573821926;
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 EET";
	dt = core::dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 Y";
	dt = core::dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 AFT";
	dt = core::dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 IRST";
	dt = core::dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());
}
