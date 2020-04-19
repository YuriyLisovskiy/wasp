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

using namespace wasp::core;


TEST(DatetimeTestCase, StrftimeTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 EET";
	auto dt = dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	dt = dt.as_timezone(std::make_shared<dt::Timezone>(
		dt::Timedelta(0, 0, 0 ,0 , 0, 2), "EET"
	));

	auto strf_time = dt.strftime("%a, %e %b %Y %T %Z");
	ASSERT_EQ(str_dt, strf_time);
}

TEST(DatetimeTestCase, TimestampTest)
{
	const char* str_dt = "Fri, 15 Nov 2019 12:45:26 GMT";
	auto dt = dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	double expected = 1573814726.0;
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 EET";
	dt = dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 Y";
	dt = dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 AFT";
	dt = dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());

	str_dt = "Fri, 15 Nov 2019 12:45:26 IRST";
	dt = dt::Datetime::strptime(str_dt, "%a, %e %b %Y %T %Z");
	ASSERT_EQ(expected, dt.timestamp());
}

TEST(ModuleTestCase, TestContacts)
{
	ASSERT_EQ(dt::MIN_YEAR, 1);
	ASSERT_EQ(dt::MAX_YEAR, 9999);
}

TEST(ModuleTestCase, TestDivideAndRound)
{
	ASSERT_EQ(dt::internal::_divide_and_round(-10, -3), 3);
	ASSERT_EQ(dt::internal::_divide_and_round(5, -2), -2);

	// four cases: (2 signs of a) x (2 signs of b)
	ASSERT_EQ(dt::internal::_divide_and_round(7, 3), 2);
	ASSERT_EQ(dt::internal::_divide_and_round(-7, 3), -2);
	ASSERT_EQ(dt::internal::_divide_and_round(7, -3), -2);
	ASSERT_EQ(dt::internal::_divide_and_round(-7, -3), 2);

	// ties to even - eight cases:
	// (2 signs of a) x (2 signs of b) x (even / odd quotient)
	ASSERT_EQ(dt::internal::_divide_and_round(10, 4), 2);
	ASSERT_EQ(dt::internal::_divide_and_round(-10, 4), -2);
	ASSERT_EQ(dt::internal::_divide_and_round(10, -4), -2);
	ASSERT_EQ(dt::internal::_divide_and_round(-10, -4), 2);

	ASSERT_EQ(dt::internal::_divide_and_round(6, 4), 2);
	ASSERT_EQ(dt::internal::_divide_and_round(-6, 4), -2);
	ASSERT_EQ(dt::internal::_divide_and_round(6, -4), -2);
	ASSERT_EQ(dt::internal::_divide_and_round(-6, -4), 2);
}

class FixedOffset : public dt::Timezone
{
private:
	dt::Timedelta _dst_offset;

public:
	FixedOffset(
		int offset, const std::string& name, int dst_offset = 42
	) : Timezone(dt::Timedelta(0, 0, 0, 0, offset), name)
	{
		this->_dst_offset = dt::Timedelta(0, 0, 0, 0, dst_offset);
	}

	std::shared_ptr<dt::Timedelta> dst(const dt::Datetime* dt) const override
	{
		return std::make_shared<dt::Timedelta>(this->_dst_offset);
	}
};

TEST(TimezoneTestCase, NormalTest)
{
	auto fo = FixedOffset(3, "Three");

	auto now = dt::Datetime::now();
	ASSERT_EQ(*fo.utc_offset(&now), dt::Timedelta(0, 0, 0, 0, 3));
	ASSERT_EQ(fo.tz_name(&now), std::string("Three"));
	ASSERT_EQ(*fo.dst(&now), dt::Timedelta(0, 0, 0, 0, 42));

	ASSERT_EQ(*fo.utc_offset(nullptr), dt::Timedelta(0, 0, 0, 0, 3));
	ASSERT_EQ(fo.tz_name(nullptr), std::string("Three"));
	ASSERT_EQ(*fo.dst(nullptr), dt::Timedelta(0, 0, 0, 0, 42));
}
