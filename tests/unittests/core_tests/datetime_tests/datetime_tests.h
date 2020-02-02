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

TEST(TzTestCase, BothDirectionAssertTest)
{
	using core::dt::internal::OFFSET_TO_TZ_ABBR;
	using core::dt::internal::TZ_ABBR_TO_OFFSET;

	ASSERT_EQ(OFFSET_TO_TZ_ABBR.size(), TZ_ABBR_TO_OFFSET.size());

	auto it1 = OFFSET_TO_TZ_ABBR.cbegin();
	auto it2 = TZ_ABBR_TO_OFFSET.cbegin();
	while (it1 != OFFSET_TO_TZ_ABBR.cend())
	{
		ASSERT_TRUE(OFFSET_TO_TZ_ABBR.contains(it2->second));
		ASSERT_TRUE(TZ_ABBR_TO_OFFSET.contains(it1->second));
		it1++;
		it2++;
	}
}

TEST(DateTimeTestCase, StrptimeTzAbbreviationsTest)
{
	using core::dt::internal::OFFSET_TO_TZ_ABBR;
	for (auto it = OFFSET_TO_TZ_ABBR.cbegin(); it != OFFSET_TO_TZ_ABBR.cend(); it++)
	{
		std::string str_dt("Fri, 15 Nov 2019 12:45:26 " + it->second);
		auto dt = core::dt::DateTime::strptime(str_dt.c_str(), "%a, %e %b %Y %T %Z");
		ASSERT_EQ(dt.date().day_of_month(), 15);
		ASSERT_EQ(dt.date().day_of_week(), 5);
		ASSERT_EQ(dt.date().month(), 11);
		ASSERT_EQ(dt.date().year(), 2019);
		ASSERT_EQ(dt.time().hour(), 12);
		ASSERT_EQ(dt.time().minute(), 45);
		ASSERT_EQ(dt.time().second(), 26);
		ASSERT_EQ(dt.tz().abbr(), it->second);
		ASSERT_EQ(dt.tz().name(), it->second);
		ASSERT_EQ(dt.tz().str_offset(), it->first);
	}
}

TEST(DateTimeTestCase, StrptimeTzHourMinTest)
{
	using core::dt::internal::OFFSET_TO_TZ_ABBR;
	for (auto it = OFFSET_TO_TZ_ABBR.cbegin(); it != OFFSET_TO_TZ_ABBR.cend(); it++)
	{
		std::string str_dt("Fri, 15 Nov 2019 12:45:26 " + it->first);
		auto dt = core::dt::DateTime::strptime(str_dt.c_str(), "%a, %e %b %Y %T %z");
		ASSERT_EQ(dt.date().day_of_month(), 15);
		ASSERT_EQ(dt.date().day_of_week(), 5);
		ASSERT_EQ(dt.date().month(), 11);
		ASSERT_EQ(dt.date().year(), 2019);
		ASSERT_EQ(dt.time().hour(), 12);
		ASSERT_EQ(dt.time().minute(), 45);
		ASSERT_EQ(dt.time().second(), 26);
		ASSERT_EQ(dt.tz().abbr(), it->second);
		ASSERT_EQ(dt.tz().name(), it->second);
		ASSERT_EQ(dt.tz().str_offset(), it->first);
	}
}

TEST(DateTimeTestCase, StrftimeTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 EET";
	auto dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	auto strf_time = dt.strftime("%a, %e %b %Y %T %Z");
	ASSERT_EQ(str_dt, strf_time);
}

TEST(DateTimeTestCase, TimestampTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 GMT";
	auto dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	long expected = 1573821926;
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 EET";
	dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 Y";
	dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 AFT";
	dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 IRST";
	dt = core::dt::DateTime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_CORE_TESTS_DATETIME_TESTS_DATETIME_TESTS_INCLUDE_H
