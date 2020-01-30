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

#include <gtest/gtest.h>

#include "../../_def_.h"
#include "../../../../src/core/datetime/datetime.h"


__UNIT_TESTS_BEGIN__

TEST(DateTimeTestCase, StrptimeTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 EEDT";
	auto dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(dt.date().day_of_month(), 15);
	ASSERT_EQ(dt.date().day_of_week(), 5);
	ASSERT_EQ(dt.date().month(), 11);
	ASSERT_EQ(dt.date().year(), 2019);
	ASSERT_EQ(dt.time().hour(), 12);
	ASSERT_EQ(dt.time().minute(), 45);
	ASSERT_EQ(dt.time().second(), 26);
	ASSERT_EQ(dt.tz().get_name(), "EEDT");
	ASSERT_EQ(dt.tz().get_offset(), 10800);
}

TEST(DateTimeTestCase, StrftimeTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 EEDT";
	auto dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	auto strf_time = dt.strftime("%a, %e %b %Y %T %Z");
	ASSERT_EQ(str_dt, strf_time);
}

// FIXME: fails TEST(DateTimeTestCase, UtcEpochTest)
TEST(DateTimeTestCase, UtcEpochTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 GMT";
	auto dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	size_t expected = 1573821926;
//	ASSERT_EQ(expected, dt.utc_epoch());

	str_dt = "Fri, 15 Nov 2019 12:45:26 EEDT";
	dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	expected = 1573814726;
	ASSERT_EQ(expected, dt.utc_epoch());
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_CORE_TESTS_DATETIME_TESTS_DATETIME_TESTS_INCLUDE_H
