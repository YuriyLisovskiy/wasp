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
#include <chrono>

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
std::string _lf(long long int num, int w, char c)
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

void _replace(
	std::string& src, const std::string& old, const std::string& new_
)
{
	size_t index = 0;
	while (true)
	{
		index = src.find(old, index);
		if (index == std::string::npos)
		{
			break;
		}

		src.replace(index, old.size(), new_);
		index += new_.size();
	}
}

std::string _wrap_strftime(
	const std::string& format, const tm& time_tuple,
	const std::function<long long int()>& microsecond,
	const std::function<Timedelta()>& utc_offset,
	const std::function<std::string()>& tz_name
)
{
	// Don't call utcoffset() or tzname() unless actually needed.
	std::string f_replace, z_replace, Z_replace;

	// Scan format for %z and %Z escapes, replacing as needed.
	std::string new_format;
	size_t i = 0;
	size_t n = format.size();
	while (i < n)
	{
		auto ch = format[i];
		i++;
		if (ch == '%')
		{
			if (i < n)
			{
				ch = format[i];
				i++;
				if (ch == 'f')
				{
					if (f_replace.empty())
					{
						f_replace = _lf(microsecond ? microsecond() : 0, 6);
					}

					new_format += f_replace;
				}
				else if (ch == 'z')
				{
					if (z_replace.empty())
					{
						if (utc_offset)
						{
							auto offset = utc_offset();
							auto sign = "+";
							if (offset.days() < 0)
							{
								offset = -offset;
								sign = "-";
							}

							auto h1 = Timedelta(0, 0, 0, 0, 0, 1);
							auto h = offset / h1;
							auto rest = offset % h1;

							auto m1 = Timedelta(0, 0, 0, 0, 1);
							auto m = rest / m1;
							rest = rest % m1;

							auto s = rest.seconds();
							auto u = offset.microseconds();
							if (u)
							{
								z_replace = sign + _lf(h) + _lf(m) + _lf(s) + "." + _lf(u, 6);
							}
							else if (s)
							{
								z_replace = sign + _lf(h) + _lf(m) + _lf(s);
							}
							else
							{
								z_replace = sign + _lf(h) + _lf(m);
							}
						}
					}

					assert(z_replace.find('%') == std::string::npos);
					new_format += z_replace;
				}
				else if (ch == 'Z')
				{
					if (Z_replace.empty())
					{
						if (tz_name)
						{
							// strftime is going to have at this: escape %
							Z_replace = tz_name();
							_replace(Z_replace, "%", "%%");
						}
					}

					new_format += Z_replace;
				}
				else
				{
					new_format += '%';
					new_format += ch;
				}
			}
			else
			{
				new_format += '%';
			}
		}
		else
		{
			new_format += ch;
		}
	}

	const auto buff_size = 100;
	char buffer[buff_size];
	std::strftime(buffer, buff_size, new_format.c_str(), &time_tuple);
	return buffer;
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

long long int _divide_and_round(long long int a, long long int b)
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

std::string Timedelta::_plural(long long int n) const
{
	return n != 1 ? "s" : "";
}

uint Timedelta::_to_microseconds() const
{
	return ((this->_days * (24 * 3600) + this->_seconds) * 1000000 + this->_microseconds);
}

signed char Timedelta::_cmp(const Timedelta& other) const
{
	return internal::_cmp(this->_to_microseconds(), other._to_microseconds());
}

Timedelta::Timedelta(
	long long int days, long long int seconds, long long int microseconds,
	long long int milliseconds, long long int minutes, long long int hours, long long int weeks
)
{
	long long int d = 0;
	long long int s = 0;
	long long int us = 0;

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
	microseconds = (long long int)((double)microseconds + us_double);
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
}

std::string Timedelta::str() const
{
	long long int mm = this->_seconds / 60;
	long long int ss = this->_seconds % 60;

	long long int hh = mm / 60;
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

long long int Timedelta::total_seconds() const
{
	auto power = std::pow(10, 6);
	return ((this->_days * 86400 + this->_seconds) * power + this->_microseconds) / power;
}

long long int Timedelta::days() const
{
	return this->_days;
}

long long int Timedelta::seconds() const
{
	return this->_seconds;
}

long long int Timedelta::microseconds() const
{
	return this->_microseconds;
}

Timedelta Timedelta::operator + (const Timedelta& other) const
{
	return Timedelta(
		this->_days + other._days,
		this->_seconds + other._seconds,
		this->_microseconds + other._microseconds
	);
}

Timedelta Timedelta::operator - (const Timedelta& other) const
{
	return Timedelta(
		this->_days - other._days,
		this->_seconds - other._seconds,
		this->_microseconds - other._microseconds
	);
}

Timedelta Timedelta::operator - () const
{
	return Timedelta(
		-this->_days,
		-this->_seconds,
		-this->_microseconds
	);
}

Timedelta Timedelta::abs() const
{
	if (this->_days < 0)
	{
		return -*this;
	}

	return *this;
}

Timedelta Timedelta::operator * (const Timedelta& other) const
{
	return Timedelta(
		this->_days * other._days,
		this->_seconds * other._seconds,
		this->_microseconds * other._microseconds
	);
}

uint Timedelta::operator / (const Timedelta& other) const
{
	return this->_to_microseconds() / other._to_microseconds();
}

Timedelta Timedelta::operator / (const long long int& other) const
{
	return Timedelta(
		0, 0,
		internal::_divide_and_round(this->_to_microseconds(), other)
	);
}

Timedelta Timedelta::operator % (const Timedelta& other) const
{
	return Timedelta(
		0, 0,
		this->_to_microseconds() % other._to_microseconds()
	);
}

bool Timedelta::operator == (const Timedelta& other) const
{
	return this->_cmp(other) == 0;
}

bool Timedelta::operator != (const Timedelta& other) const
{
	return this->_cmp(other) != 0;
}

bool Timedelta::operator <= (const Timedelta& other) const
{
	return this->_cmp(other) <= 0;
}

bool Timedelta::operator < (const Timedelta& other) const
{
	return this->_cmp(other) < 0;
}

bool Timedelta::operator >= (const Timedelta& other) const
{
	return this->_cmp(other) >= 0;
}

bool Timedelta::operator > (const Timedelta& other) const
{
	return this->_cmp(other) > 0;
}

Timedelta::operator bool() const
{
	return this->_days != 0 || this->_seconds != 0 || this->_microseconds != 0;
}

const Timedelta Timedelta::MIN = Timedelta(-999999999);
const Timedelta Timedelta::MAX = Timedelta(999999999, 59, 999999, 0, 59, 23, 0);
const Timedelta Timedelta::RESOLUTION = Timedelta(0, 0, 1);

signed char Date::_cmp(const Date& other) const
{
	auto y1_y2 = internal::_cmp(this->_year, other._year);
	if (y1_y2 != 0)
	{
		return y1_y2;
	}

	auto m1_m2 = internal::_cmp(this->_month, other._month);
	if (m1_m2 != 0)
	{
		return m1_m2;
	}

	auto d1_d2 = internal::_cmp(this->_day, other._day);
	if (d1_d2 != 0)
	{
		return d1_d2;
	}

	return 0;
}

Date::Date(ushort year, ushort month, ushort day)
{
	internal::_check_date_fields(year, month, day);
	this->_year = year;
	this->_month = month;
	this->_day = day;
}

Date Date::from_timestamp(time_t t)
{
	auto lt = std::localtime(&t);
	return Date(lt->tm_year, lt->tm_mon, lt->tm_mday);
}

Date Date::today()
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	return Date::from_timestamp(ms);
}

Date Date::from_ordinal(size_t n)
{
	auto ymd = internal::_ord2ymd(n);
	return Date(ymd.year, ymd.month, ymd.day);
}

Date Date::from_iso_format(const std::string& date_str)
{
	m_assert(date_str.size() == 10, ("Invalid iso format string: " + date_str).c_str());
	auto iso_ymd = internal::_parse_isoformat_date(date_str);
	return Date(iso_ymd.year, iso_ymd.month, iso_ymd.day);
}

Date Date::from_iso_calendar(ushort year, ushort week, ushort day)
{
	// Year is bounded this way because 9999-12-31 is (9999, 52, 5)
	if (!(year >= MIN_YEAR && year <= MAX_YEAR))
	{
		throw std::invalid_argument("Year is out of range: " + std::to_string(year));
	}

	if (!(week > 0 && week < 53))
	{
		auto out_of_range = true;
		if (week == 53)
		{
			// ISO years have 53 weeks in them on years starting with a
			// Thursday and leap years starting on a Wednesday
			auto first_weekday = internal::_ymd2ord(year, 1, 1) % 7;
			if (first_weekday == 4 || (first_weekday == 3 && internal::_is_leap(year)))
			{
				out_of_range = false;
			}
		}

		if (out_of_range)
		{
			throw std::invalid_argument("Invalid week: " + std::to_string(week));
		}
	}

	if (!(day > 0 && day < 8))
	{
		throw std::invalid_argument(
			"Invalid weekday: " + std::to_string(day) + " (range is [1, 7])"
		);
	}

	// Now compute the offset from (Y, 1, 1) in days:
	auto day_offset = (week - 1) * 7 + (day - 1);

	// Calculate the ordinal day for monday, week 1
	auto day_1 = internal::_iso_week1monday(year);
	auto ord_day = day_1 + day_offset;

	auto ymd = internal::_ord2ymd(ord_day);
	return Date(ymd.year, ymd.month, ymd.day);
}

std::string Date::ctime() const
{
	auto weekday = this->to_ordinal() % 7;
	if (!weekday)
	{
		weekday = 7;
	}

	return internal::_DAY_NAMES[weekday] + " " +
			internal::_MONTH_NAMES[this->_month] + " " +
			internal::_lf(this->_day, 2, ' ') + " 00:00:00 " +
			internal::_lf(this->_year, 4, 0);
}

std::string Date::strftime(const std::string& fmt) const
{
	return internal::_wrap_strftime(fmt, this->time_tuple());
}

std::string Date::iso_format() const
{
	return internal::_lf(this->_year, 4) + "-" +
			internal::_lf(this->_month) + "-" +
			internal::_lf(this->_day);
}

std::string Date::str() const
{
	return this->iso_format();
}

ushort Date::year() const
{
	return this->_year;
}

ushort Date::month() const
{
	return this->_month;
}

ushort Date::day() const
{
	return this->_day;
}

tm Date::time_tuple() const
{
	return internal::_build_struct_time(
		this->_year, this->_month, this->_day, 0, 0, 0, -1
	);
}

size_t Date::to_ordinal() const
{
	return internal::_ymd2ord(this->_year, this->_month, this->_day);
}

Date Date::replace(ushort year, ushort month, ushort day) const
{
	if (year == 0)
	{
		year = this->_year;
	}

	if (month == 0)
	{
		month = this->_month;
	}

	if (day == 0)
	{
		day = this->_day;
	}

	return Date(year, month, day);
}

bool Date::operator == (const Date& other) const
{
	return this->_cmp(other) == 0;
}

bool Date::operator != (const Date& other) const
{
	return this->_cmp(other) != 0;
}

bool Date::operator <= (const Date& other) const
{
	return this->_cmp(other) <= 0;
}

bool Date::operator < (const Date& other) const
{
	return this->_cmp(other) < 0;
}

bool Date::operator >= (const Date& other) const
{
	return this->_cmp(other) >= 0;
}

bool Date::operator > (const Date& other) const
{
	return this->_cmp(other) > 0;
}

Date Date::operator + (const Timedelta& other) const
{
	auto o = this->to_ordinal() + other.days();
	if (o > 0 && o <= internal::_MAX_ORDINAL)
	{
		return Date::from_ordinal(o);
	}

	throw std::out_of_range("result out of range");
}

Timedelta Date::operator - (const Date& other) const
{
	long long int days1 = this->to_ordinal();
	long long int days2 = other.to_ordinal();
	return Timedelta(days1 - days2);
}

Date Date::operator - (const Timedelta& other) const
{
	return *this + Timedelta(-other.days());
}

short int Date::weekday() const
{
	return (short int)((this->to_ordinal() + 6) % 7);
}

short int Date::iso_weekday() const
{
	// 1-Jan-0001 is a Monday
	auto wd = this->to_ordinal() % 7;
	if (!wd)
	{
		wd = 7;
	}

	return wd;
}

std::tuple<ushort, ushort, ushort> Date::iso_calendar() const
{
	auto year = this->_year;
	auto week1monday = internal::_iso_week1monday(year);
	auto today = internal::_ymd2ord(this->_year, this->_month, this->_day);

	// Internally, week and day have origin 0
	auto week = (today - week1monday) / 7;
	auto day = (today - week1monday) % 7;
	if (week < 0)
	{
		year--;
		week1monday = internal::_iso_week1monday(year);
		week = (today - week1monday) / 7;
		day = (today - week1monday) % 7;
	}
	else if (week >= 52)
	{
		if (today >= internal::_iso_week1monday(year + 1))
		{
			year++;
			week = 0;
		}
	}

	return std::tuple(year, week + 1, day + 1);
}

const Date Date::MIN = Date(1, 1, 1);
const Date Date::MAX = Date(9999, 12, 31);
const Timedelta Date::RESOLUTION = Timedelta(1);

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

size_t _iso_week1monday(ushort year)
{
	auto THURSDAY = 3;
	auto first_day = _ymd2ord(year, 1, 1);
	auto first_weekday = (first_day + 6) % 7;   // See weekday above
	auto week1monday = first_day - first_weekday;
	if (first_weekday > THURSDAY)
	{
		week1monday += 7;
	}

	return week1monday;
}

__DATETIME_INTERNAL_END__
