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
 * An implementation of dt.h
 */

// C++ libraries.
#include <cassert>
#include <cmath>

// Header.
#include "./dt.h"

// Framework modules.
// TODO:


__DATETIME_INTERNAL_BEGIN__

bool _is_leap(ushort year)
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

size_t _days_before_year(ushort year)
{
	auto y = year - 1;
	return y*365 + y/4 - y/100 + y/400;
}

int _days_in_month(ushort year, ushort month)
{
	m_assert(month >= 1 && month <= 12, "month must be in 1..12");
	if (month == 2 && _is_leap(year))
	{
		return 29;
	}

	return _DAYS_IN_MONTH[month];
}

size_t _days_before_month(ushort year, ushort month)
{
	m_assert(month >= 1 && month <= 12, "month must be in 1..12");
	return _DAYS_BEFORE_MONTH[month] + (month > 2 && _is_leap(year));
}

size_t _ymd2ord(ushort year, ushort month, ushort day)
{
	m_assert(month >= 1 && month <= 12, "month must be in 1..12");
	auto dim = _days_in_month(year, month);
	m_assert(
		day >= 1 && day <= dim,
		(std::string("day must be in 1..") + std::to_string(dim)).c_str()
	);
	return _days_before_year(year) + _days_before_month(year, month) + day;
}

ymd _ord2ymd(size_t n)
{
	// n is a 1-based index, starting at 1-Jan-1.  The pattern of leap years
	// repeats exactly every 400 years.  The basic strategy is to find the
	// closest 400-year boundary at or before n, then work with the offset
	// from that boundary to n.  Life is much clearer if we subtract 1 from
	// n first -- then the values of n at 400-year boundaries are exactly
	// those divisible by _DI400Y:
	//
	//     D  M   Y            n              n-1
	//     -- --- ----        ----------     ----------------
	//     31 Dec -400        -_DI400Y       -_DI400Y -1
	//      1 Jan -399         -_DI400Y +1   -_DI400Y      400-year boundary
	//     ...
	//     30 Dec  000        -1             -2
	//     31 Dec  000         0             -1
	//      1 Jan  001         1              0            400-year boundary
	//      2 Jan  001         2              1
	//      3 Jan  001         3              2
	//     ...
	//     31 Dec  400         _DI400Y        _DI400Y -1
	//      1 Jan  401         _DI400Y +1     _DI400Y      400-year boundary

	n -= 1;
	auto n400 = n / _DI400Y;
	n = n % _DI400Y;
	auto year = n400 * 400 + 1;

	// Now n is the (non-negative) offset, in days, from January 1 of year, to
	// the desired date.  Now compute how many 100-year cycles precede n.
	// Note that it's possible for n100 to equal 4!  In that case 4 full
	// 100-year cycles precede the desired day, which implies the desired
	// day is December 31 at the end of a 400-year cycle.
	auto n100 = n / _DI100Y;
	n = n % _DI100Y;

	// Now compute how many 4-year cycles precede it.
	auto n4 = n / _DI4Y;
	n = n & _DI4Y;

	// And now how many single years.  Again n1 can be 4, and again meaning
	// that the desired day is December 31 at the end of the 4-year cycle.
	auto n1 = n / 365;
	n = n % 365;

	year += n100 * 100 + n4 * 4 + n1;
	if (n1 == 4 || n100 == 4)
	{
		assert(n == 0);
		return ymd{(ushort)(year - 1), 12, 31};
	}

	// Now the year is correct, and n is the offset from January 1.  We find
	// the month via an estimate that's either exact or one too large.
	auto leap_year = n1 == 3 && (n4 != 24 || n100 == 3);
	assert(leap_year == _is_leap(year));
	auto month = (n + 50) >> 5;
	auto preceding = _DAYS_BEFORE_MONTH[month] + (month > 2 && leap_year);
	if (preceding > n)
	{
		month -= 1;
		preceding -= _DAYS_IN_MONTH[month] + (month == 2 && leap_year);
	}

	n -= preceding;
	assert(n >= 0 && n < _days_in_month(year, month));

	// Now the year and month are correct, and n is the offset from the
	// start of that month:  we're done!
	return ymd{(ushort)year, (ushort)month, (ushort)(n + 1)};
}

tm _build_struct_time(
	ushort y, ushort m, ushort d, ushort hh, ushort mm, ushort ss, int dst_flag
)
{
	tm t{};

	// TODO: check if it's not necessary to subtract 1900 from 'y'
	t.tm_year = y;
	t.tm_mon = m;
	t.tm_mday = d;
	t.tm_hour = hh;
	t.tm_min = mm;
	t.tm_sec = ss;
	t.tm_wday = (int)(_ymd2ord(y, m, d) + 6) % 7;
	t.tm_yday = (int)_days_before_month(y, m) + d;
	t.tm_isdst = dst_flag;
	return t;
}

// Prepends 'c' 'w'-times and appends 'num' as std::string;
// if 'num' string is longer than 'w', returns 'num' as std::string.
std::string _lf(int num, int w, char c)
{
	auto s = std::to_string(num);
	w -= s.size();
	return w > 0 ? std::string(std::abs(w), c) + s : s;
}

std::string _format_time(
	ushort hh, ushort mm, ushort ss, uint us, time_spec ts
)
{
	if (ts == time_spec::AUTO)
	{
		// Skip trailing microseconds when us==0.
		ts = us >= 0 ? time_spec::MICROSECONDS : time_spec::SECONDS;
	}
	else if (ts == time_spec::MILLISECONDS)
	{
		us /= 1000;
	}

	std::string result;
	switch (ts)
	{
		case time_spec::HOURS:
			result = _lf(hh);
			break;
		case time_spec::MINUTES:
			result = _lf(hh) + ":" + _lf(mm);
			break;
		case time_spec::SECONDS:
			result = _lf(hh) + ":" + _lf(mm) + ":" + _lf(ss);
			break;
		case time_spec::MILLISECONDS:
			result = _lf(hh) + ":" + _lf(mm) + ":" + _lf(ss) + "." + _lf(us, 3);
			break;
		case time_spec::MICROSECONDS:
			result = _lf(hh) + ":" + _lf(mm) + ":" + _lf(ss) + "." + _lf(us, 6);
			break;
		default:
			throw std::invalid_argument("Unknown time_spec value");
	}

	return result;
}

ymd _parse_isoformat_date(const std::string& dt_str)
{
	// It is assumed that this function will only be called with a
	// string of length exactly 10, and (though this is not used) ASCII-only
	auto year = (ushort)std::stol(dt_str.substr(0, 4));
	if (dt_str[4] != '-')
	{
		throw std::invalid_argument("Invalid date separator: " + std::to_string(dt_str[4]));
	}

	auto month = (ushort)std::stol(dt_str.substr(5, 2));
	if (dt_str[7] != '-')
	{
		throw std::invalid_argument("Invalid date separator");
	}

	auto day = (ushort)std::stol(dt_str.substr(8, 2));
	return ymd{year, month, day};
}

hmsf _parse_hh_mm_ss_ff(const std::string& t_str)
{
	auto str_len = t_str.size();
	uint time_comps[4]{0, 0, 0, 0};
	size_t pos = 0;
	for (size_t comp = 0; comp < 3; comp++)
	{
		if ((str_len - pos) < 2)
		{
			throw std::invalid_argument("Incomplete time component");
		}

		time_comps[comp] = std::stol(t_str.substr(pos, 2));
		pos += 2;
		auto next_char = t_str[pos];
		if (!next_char || comp >= 2)
		{
			break;
		}

		if (next_char != ':')
		{
			throw std::invalid_argument("Invalid time separator: " + std::to_string(next_char));
		}

		pos++;
	}

	if (pos < str_len)
	{
		if (t_str[pos] != '.')
		{
			throw std::invalid_argument("Invalid microsecond component");
		}

		pos++;
		auto len_rem = str_len - pos;
		if (len_rem != 3 && len_rem != 6)
		{
			throw std::invalid_argument("Invalid microsecond component");
		}

		time_comps[3] = std::stol(t_str.substr(pos));
		if (len_rem == 3)
		{
			time_comps[3] *= 1000;
		}
	}

	return hmsf{
		(ushort)time_comps[0],
		(ushort)time_comps[1],
		(ushort)time_comps[2],
		time_comps[3]
	};
}

void _check_date_fields(ushort year, ushort month, ushort day)
{
	if (!(year >= MIN_YEAR && year <= MAX_YEAR))
	{
		throw std::invalid_argument(
			"year must be in " + std::to_string(MIN_YEAR) + ".." +
			std::to_string(MAX_YEAR) + ": " + std::to_string(year)
		);
	}

	if (!(month >= 1 && month <= 12))
	{
		throw std::invalid_argument("month must be in 1..12: " + std::to_string(month));
	}

	auto dim = _days_in_month(year, month);
	if (!(day >= 1 && day <= dim))
	{
		throw std::invalid_argument(
			"day must be in 1.." + std::to_string(dim) + ": " + std::to_string(day)
		);
	}
}

void _check_time_fields(
	ushort hour, ushort minute, ushort second, uint microsecond, unsigned char fold
)
{
	if (!(hour >= 0 && hour <= 23))
	{
		throw std::invalid_argument("hour must be in 0..23: " + std::to_string(hour));
	}

	if (!(minute >= 0 && minute <= 59))
	{
		throw std::invalid_argument("minute must be in 0..59: " + std::to_string(minute));
	}

	if (!(second >= 0 && second <= 59))
	{
		throw std::invalid_argument("second must be in 0..59: " + std::to_string(second));
	}

	if (!(microsecond >= 0 && microsecond <= 999999))
	{
		throw std::invalid_argument("microsecond must be in 0..999999: " + std::to_string(microsecond));
	}

	if (fold != 0 && fold != 1)
	{
		throw std::invalid_argument("fold must be either 0 or 1");
	}
}

int _divide_and_round(int a, int b)
{
	auto q = a / b;
	auto r = a % b;

	// round up if either r / b > 0.5, or r / b == 0.5 and q is odd.
	// The expression r / b > 0.5 is equivalent to 2 * r > b if b is
	// positive, 2 * r < b if b negative.
	r *= 2;
	auto greater_than_half = b > 0 ? r > b : r < b;
	if (greater_than_half || (r == b && q % 2 == 1))
	{
		q += 1;
	}

	return q;
}

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

std::string Timedelta::_plural(uint n) const
{
	return n != 1 ? "s" : "";
}

Timedelta::Timedelta(
	uint days, ullint seconds, ullint microseconds,
	ullint milliseconds, uint minutes, uint hours, uint weeks
)
{
	uint d = 0;
	ullint s = 0;
	ullint us = 0;

	// Normalize everything to days, seconds, microseconds.
	days += weeks*7;
	seconds += minutes*60 + hours * 3600;
	microseconds += milliseconds*1000;

	// Get rid of all fractions, and normalize s and us.
	// Take a deep breath <wink>.
	auto day_seconds_frac = 0.0;
	d = days;
	assert(d <= 24 * 3600);

	auto seconds_frac = day_seconds_frac;
	days = seconds / 24*3600;
	seconds = seconds % (24*3600);
	d += days;
	s += seconds;
	assert(s <= 2 * 24 * 3600);

	auto us_double = seconds_frac * 1e6;
	assert(std::abs(us_double) < 2.1e6);    // exact value not critical

	seconds = microseconds / 1000000;
	microseconds = microseconds % 1000000;
	days = seconds / 24 * 3600;
	seconds = seconds % (24 * 3600);
	d += days;
	s += seconds;
	microseconds = (ullint)(microseconds + us_double);
	assert(s <= 3 * 24 * 3600);
	assert(microseconds < 3.1e6);

	// Just a little bit of carrying possible for microseconds and seconds.
	seconds = microseconds / 1000000;
	us = microseconds % 1000000;
	s += seconds;
	days = s / 24 * 3600;
	s = s % 24 * 3600;
	d += days;

	assert(s >= 0 && s < 24 * 3600);
	assert(us >= 0 && us < 1000000);

	if (d > 999999999)
	{
		throw std::invalid_argument(
			"timedelta # of days is too large: " + std::to_string(d)
		);
	}

	this->_days = d;
	this->_seconds = s;
	this->_microseconds = us;
	this->_hashcode = -1;
}

std::string Timedelta::str() const
{
	uint mm = this->_seconds / 60;
	uint ss = this->_seconds % 60;

	uint hh = mm / 60;
	mm = mm % 60;

	auto s = std::to_string(hh) + ":" + internal::_lf(mm) + ":" + internal::_lf(ss);
	if (this->_days != 0)
	{
		s = std::to_string(this->_days) + " day" + this->_plural(this->_days) + ", " + s;
	}

	if (this->_microseconds != 0)
	{
		s += "." + internal::_lf(this->_microseconds, 6);
	}

	return s;
}

ullint Timedelta::total_seconds() const
{
	auto power = std::pow(10, 6);
	return ((this->_days * 86400 + this->_seconds) * power + this->_microseconds) / power;
}

uint Timedelta::days() const
{
	return this->_days;
}

uint Timedelta::seconds() const
{
	return this->_seconds;
}

uint Timedelta::microseconds() const
{
	return this->_microseconds;
}



__DATETIME_END__
