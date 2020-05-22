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
 * An implementation of core/datetime.h
 */

#include "./datetime.h"

// C++ libraries.
#include <iostream>
#include <cassert>
#include <ctime>
#include <cmath>
#include <cstring>


__DATETIME_INTERNAL_BEGIN__

void _M_Assert(
	const char* expr_str, bool expr,
	const char* function, int /* line */,
	const char* msg
)
{
	if (!expr)
	{
		std::cerr << function << ": assertion failed: '"  << expr_str << "'\n\t" << msg << "\n";
		abort();
	}
}

bool _is_leap(ushort year)
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

size_t _days_before_year(ushort year)
{
	auto y = year - 1;
	auto res = y*365 + y/4 - y/100 + y/400;
	return res;
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
	auto res = _DAYS_BEFORE_MONTH[month] + (month > 2 && _is_leap(year));
	return res;
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
	auto n400_n = internal::_div_mod(n, _DI400Y);
	auto n400 = n400_n.first;
	n = n400_n.second;
	auto year = n400 * 400 + 1;

	// Now n is the (non-negative) offset, in days, from January 1 of year, to
	// the desired date.  Now compute how many 100-year cycles precede n.
	// Note that it's possible for n100 to equal 4!  In that case 4 full
	// 100-year cycles precede the desired day, which implies the desired
	// day is December 31 at the end of a 400-year cycle.
	auto n100_n = internal::_div_mod(n, _DI100Y);
	auto n100 = n100_n.first;
	n = n100_n.second;

	// Now compute how many 4-year cycles precede it.
	auto n4_n = internal::_div_mod(n, _DI4Y);
	auto n4 = n4_n.first;
	n = n4_n.second;

	// And now how many single years.  Again n1 can be 4, and again meaning
	// that the desired day is December 31 at the end of the 4-year cycle.
	auto n1_n = internal::_div_mod(n, (size_t)365);
	auto n1 = n1_n.first;
	n = n1_n.second;

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

tm_tuple _build_struct_time(
	ushort y, ushort m, ushort d, ushort hh, ushort mm, ushort ss, int dst_flag
)
{
	tm_tuple t{};
	t.tm_year = y - 1900;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = hh;
	t.tm_min = mm;
	t.tm_sec = ss;
	t.tm_wday = (internal::_mod((int)(_ymd2ord(y, m, d) + 6), 7) + 1)%7;
	t.tm_yday = ((int)_days_before_month(y, m) + d) - 1;
	t.tm_isdst = dst_flag;
	return t;
}

void _mk_tz_info(tm_tuple* time_tuple, bool is_gmt)
{
	if (!time_tuple)
	{
		return;
	}

	if (is_gmt)
	{
		time_tuple->tm_gmtoff = 0;
		time_tuple->tm_zone = "GMT";
		return;
	}

	const short buff_sz = 15;
	char buffer[buff_sz];
	tm t = time_tuple->as_tm();
	::strftime(buffer, buff_sz, "%z|%Z", &t);
	std::string s = buffer;

	auto pos = s.find('|');
	auto offset = std::string(s.substr(0, pos));
	auto abbr = s.substr(pos + 1);

	time_tuple->tm_zone = abbr.c_str();

	// `offset` is in ISO 8601 format: "±HHMM"
	int h = std::stoi(offset.substr(0,3), nullptr, 10);
	int m = std::stoi(offset.substr(3), nullptr, 10);

	time_tuple->tm_gmtoff = h*3600 + m*60;
}

char* _strptime(const char* _s, const char* _fmt, tm_tuple* _tp)
{
	tm t{};
	auto res = ::strptime(_s, _fmt, &t);

	_tp->tm_sec = t.tm_sec;
	_tp->tm_min = t.tm_min;
	_tp->tm_hour = t.tm_hour;
	_tp->tm_mday = t.tm_mday;
	_tp->tm_mon = t.tm_mon;
	_tp->tm_year = t.tm_year;
	_tp->tm_wday = t.tm_wday;
	_tp->tm_yday = t.tm_yday;
	_tp->tm_isdst = t.tm_isdst;
	_tp->tm_gmtoff = t.tm_gmtoff;
	_tp->tm_zone = t.tm_zone;

	_tp->tm_year += 1900;
	return res;
}

Datetime _strptime_datetime(
	const std::string& datetime_str,
	const char* format
)
{
	tm_tuple dt_tm{};
	internal::_strptime(datetime_str.c_str(), format, &dt_tm);
//	internal::_mk_tz_info(&dt_tm);

	std::shared_ptr<Timezone> tz = nullptr;
	if (dt_tm.tm_zone)
	{
		// !IMPROVEMENT! Calculate microseconds.
		auto tz_delta = Timedelta(0, dt_tm.tm_gmtoff, 0);
		if (std::strlen(dt_tm.tm_zone) != 0)
		{
			tz = std::make_shared<Timezone>(tz_delta, dt_tm.tm_zone);
		}
		else
		{
			tz = std::make_shared<Timezone>(tz_delta);
		}
	}

	// !IMPROVEMENT! Calculate microseconds.
	return Datetime(
		dt_tm.tm_year, dt_tm.tm_mon + 1, dt_tm.tm_mday,
		dt_tm.tm_hour, dt_tm.tm_min, dt_tm.tm_sec, 0, tz
	);
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
	const std::string& format, const tm_tuple& time_tuple,
	const std::function<long long int()>& microsecond,
	const std::function<std::shared_ptr<Timedelta>()>& utc_offset,
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
							if (offset)
							{
								auto offset_obj = *offset;
								auto sign = "+";
								if (offset_obj.days() < 0)
								{
									offset_obj = -offset_obj;
									sign = "-";
								}

								auto h1 = Timedelta(0, 0, 0, 0, 0, 1);
								auto h = offset_obj / h1;
								auto rest = offset_obj % h1;

								auto m1 = Timedelta(0, 0, 0, 0, 1);
								auto m = rest / m1;
								rest = rest % m1;

								auto s = rest.seconds();
								auto u = offset_obj.microseconds();
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
	tm t = time_tuple.as_tm();
	std::strftime(buffer, buff_size, new_format.c_str(), &t);
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
	ushort hour, ushort minute, ushort second,
	uint microsecond, ushort fold
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
	auto q_r = internal::_div_mod(a, b);
	auto q = q_r.first;
	auto r = q_r.second;

	// round up if either r / b > 0.5, or r / b == 0.5 and q is odd.
	// The expression r / b > 0.5 is equivalent to 2 * r > b if b is
	// positive, 2 * r < b if b negative.
	r *= 2;
	auto greater_than_half = b > 0 ? r > b : r < b;
	if (greater_than_half || r == b && internal::_mod(q, 2) == 1)
	{
		q += 1;
	}

	return q;
}

time_t _time()
{
	return std::time(nullptr);
}

tm_tuple _localtime(const time_t* _timer)
{
	auto t = ::localtime(_timer);
	t->tm_year += 1900;
	return tm_tuple(t);
}

tm_tuple _gmtime(const time_t* _timer)
{
	auto t = ::gmtime(_timer);
	t->tm_year += 1900;
	return tm_tuple(t);
}

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

std::string Timedelta::_plural(long n)
{
	return n != 1 ? "s" : "";
}

long long int Timedelta::_to_microseconds() const
{
	return  ((this->_days * (24*3600) + this->_seconds) * 1000000 + this->_microseconds);
}

signed char Timedelta::_cmp(const Timedelta& other) const
{
	const short arr_sz = 3;
	long long left[arr_sz] = {this->_days, this->_seconds, this->_microseconds};
	long long right[arr_sz] = {other._days, other._seconds, other._microseconds};
	return internal::_cmp_arr(left, right, arr_sz);
}

Timedelta::Timedelta(
	long days, long seconds, long microseconds,
	long milliseconds, long minutes, long hours, long weeks
)
{
	long long int d = 0, s = 0, us = 0;

	// Normalize everything to days, seconds, microseconds.
	days += weeks * 7;
	seconds += minutes * 60 + hours * 3600;
	microseconds += milliseconds * 1000;

	// Get rid of all fractions, and normalize s and us.
	// Take a deep breath <wink>.
	auto day_seconds_frac = 0.0;
	d = days;
	assert(std::abs(day_seconds_frac) <= 1.0);
	assert(std::abs(s) <= 24 * 3600);

	auto seconds_frac = day_seconds_frac;
	assert(std::abs(seconds_frac) <= 2.0);

	auto d_s = internal::_div_mod(seconds, (long long)24*3600);
	days = d_s.first;
	seconds = d_s.second;
	d += days;
	s += seconds;
	assert(std::abs(s) <= 2 * 24 * 3600);

	auto us_double = seconds_frac * 1e6;
	assert(std::abs(us_double) < 2.1e6);    // exact value not critical

	auto s_ms = internal::_div_mod(microseconds, (long long)1000000);
	seconds = s_ms.first;
	microseconds = s_ms.second;

	d_s = internal::_div_mod(seconds, (long long)(24*3600));
	days = d_s.first;
	seconds = d_s.second;
	d += days;
	s += seconds;
	microseconds = (long long int)((double)microseconds + us_double);
	assert(std::abs(s) <= (3 * 24 * 3600));
	assert(std::abs(microseconds) < 3.1e6);

	// Just a little bit of carrying possible for microseconds and seconds.
	auto s_us = internal::_div_mod(microseconds, (long long)1000000);
	seconds = s_us.first;
	us = s_us.second;
	s += seconds;

	d_s = internal::_div_mod(s, (long long)(24*3600));
	days = d_s.first;
	s = d_s.second;
	d += days;

	assert(s >= 0 && s < (24 * 3600));
	assert(us >= 0 && us < 1000000);

	if (std::abs(d) > 999999999)
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
	auto mm_ss = internal::_div_mod(this->_seconds, (long long)60);
	long long int mm = mm_ss.first;
	long long int ss = mm_ss.second;

	auto hh_mm = internal::_div_mod(mm, (long long)60);
	long long int hh = hh_mm.first;
	mm = hh_mm.second;

	auto s = std::to_string(hh) + ":" + internal::_lf(mm) + ":" + internal::_lf(ss);
	if (this->_days != 0)
	{
		s = std::to_string(this->_days) + " day" + Timedelta::_plural(this->_days) + ", " + s;
	}

	if (this->_microseconds != 0)
	{
		s += "." + internal::_lf(this->_microseconds, 6);
	}

	return s;
}

double Timedelta::total_seconds() const
{
	auto power = std::pow(10, 6);
	return ((this->_days * 86400.0 + this->_seconds) * power + this->_microseconds) / power;
}

long Timedelta::days() const
{
	return this->_days;
}

long Timedelta::seconds() const
{
	return this->_seconds;
}

long Timedelta::microseconds() const
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

Timedelta Timedelta::operator * (const long& other) const
{
	return Timedelta(
		this->_days * other,
		this->_seconds * other,
		this->_microseconds * other
	);
}

Timedelta operator * (long left, const Timedelta& right)
{
	return Timedelta(
		right._days * left,
		right._seconds * left,
		right._microseconds * left
	);
}

double Timedelta::operator / (const Timedelta& other) const
{
	return (double)this->_to_microseconds() / (double)other._to_microseconds();
}

Timedelta Timedelta::operator / (const long& other) const
{
	return Timedelta(
		0, 0,
		internal::_divide_and_round(this->_to_microseconds(), other)
	);
}

Timedelta operator / (long left, const Timedelta& right)
{
	return Timedelta(
		0, 0,
		internal::_divide_and_round(right._to_microseconds(), left)
	);
}

Timedelta Timedelta::operator % (const Timedelta& other) const
{
	return Timedelta(
		0, 0,
		internal::_mod(this->_to_microseconds(), other._to_microseconds())
	);
}

Timedelta& Timedelta::operator = (const Timedelta& other)
{
	if (&other != this)
	{
		this->_days = other._days;
		this->_seconds = other._seconds;
		this->_microseconds = other._microseconds;
	}

	return *this;
}

Timedelta& Timedelta::operator += (const Timedelta& other)
{
	this->operator=(*this + other);
	return *this;
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

// Date class definitions.
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
	auto lt = internal::_localtime(&t);
	return Date(lt.tm_year, lt.tm_mon+1, lt.tm_mday);
}

Date Date::today()
{
	auto ms = internal::_time();
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
			auto first_weekday = internal::_mod(
				internal::_ymd2ord(year, 1, 1), 7
			);
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
	auto weekday = internal::_mod(this->to_ordinal(), 7);
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

tm_tuple Date::time_tuple() const
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
	return (short int)(internal::_mod((this->to_ordinal() + 6), 7));
}

short int Date::iso_weekday() const
{
	// 1-Jan-0001 is a Monday
	auto wd = internal::_mod(this->to_ordinal(), 7);
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
	auto w_d = internal::_div_mod(today - week1monday, 7);
	auto week = w_d.first;
	auto day = w_d.second;
	if (week < 0)
	{
		year--;
		week1monday = internal::_iso_week1monday(year);
		w_d = internal::_div_mod(today - week1monday, 7);
		week = w_d.first;
		day = w_d.second;
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

// Time class definitions.
Time::Time(
	ushort hour,
	ushort minute,
	ushort second,
	uint microsecond,
	const std::shared_ptr<Timezone>& tz_info,
	ushort fold
)
{
	internal::_check_time_fields(hour, minute, second, microsecond, fold);
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;
	this->_microsecond = microsecond;
	this->_tz_info = tz_info ? tz_info->ptr_copy() : nullptr;
	this->_fold = fold;
}

Time::Time(const Time& other)
{
	if (this != &other)
	{
		this->_hour = other._hour;
		this->_minute = other._minute;
		this->_second = other._second;
		this->_microsecond = other._microsecond;
		this->_tz_info = other._tz_info ? other._tz_info->ptr_copy() : nullptr;
		this->_fold = other._fold;
	}
}

ushort Time::hour() const
{
	return this->_hour;
}

ushort Time::minute() const
{
	return this->_minute;
}

ushort Time::second() const
{
	return this->_second;
}

uint Time::microsecond() const
{
	return this->_microsecond;
}

Timezone* Time::tz_info() const
{
	return this->_tz_info.get();
}

ushort Time::fold() const
{
	return this->_fold;
}

bool Time::operator == (const Time& other) const
{
	return this->_cmp(other, true) == 0;
}

bool Time::operator != (const Time& other) const
{
	return this->_cmp(other, true) != 0;
}

bool Time::operator <= (const Time& other) const
{
	return this->_cmp(other) <= 0;
}

bool Time::operator < (const Time& other) const
{
	return this->_cmp(other) < 0;
}

bool Time::operator >= (const Time& other) const
{
	return this->_cmp(other) >= 0;
}

bool Time::operator > (const Time& other) const
{
	return this->_cmp(other) > 0;
}

signed char Time::_cmp(const Time& other, bool allow_mixed) const
{
	auto* my_tz = this->_tz_info.get();
	auto* ot_tz = other._tz_info.get();
	std::shared_ptr<Timedelta> my_off, ot_off;
	bool base_compare;
	if ((!my_tz && !ot_tz) || (my_tz && ot_tz && *my_tz == *ot_tz))
	{
		base_compare = true;
	}
	else
	{
		my_off = this->utc_offset();
		ot_off = other.utc_offset();
		base_compare = (!my_off && !ot_off) || (my_off && ot_off && *my_off == *ot_off);
	}

	if (base_compare)
	{
		const short arr_sz = 4;
		uint left[arr_sz] = {this->_hour, this->_minute, this->_second, this->_microsecond};
		uint right[arr_sz] = {other._hour, other._minute, other._second, other._microsecond};
		return internal::_cmp_arr(left, right, arr_sz);
	}

	if (!my_off || !ot_off)
	{
		if (allow_mixed)
		{
			return 2; // arbitrary non-zero value
		}
		else
		{
			throw std::invalid_argument("cannot compare naive and aware times");
		}
	}

	auto my_hh_mm = this->_hour * 60 + this->_minute - *my_off / Timedelta(0, 0, 0, 0, 1);
	auto ot_hh_mm = other._hour * 60 + other._minute - *ot_off / Timedelta(0, 0, 0, 0, 1);

	const short arr_sz = 3;
	uint left[arr_sz] = {(uint)my_hh_mm, this->_second, this->_microsecond};
	uint right[arr_sz] = {(uint)ot_hh_mm, other._second, other._microsecond};
	return internal::_cmp_arr(left, right, arr_sz);
}

std::string Time::_tz_str() const
{
	return internal::_format_offset(this->utc_offset().get());
}

std::string Time::iso_format(time_spec ts) const
{
	auto s = internal::_format_time(
		this->_hour, this->_minute, this->_second, this->_microsecond, ts
	);
	auto tz = this->_tz_str();
	if (!tz.empty())
	{
		s += tz;
	}

	return s;
}

std::string Time::str() const
{
	return this->iso_format();
}

Time Time::from_iso_format(const std::string& time_str)
{
	auto t = internal::_parse_isoformat_time(time_str);
	return Time(t.hh, t.mm, t.ss, t.ff, t.tz);
}

std::string Time::strftime(const std::string& fmt) const
{
	tm_tuple time{};
	time.tm_year = 1900;
	time.tm_mon = 1;
	time.tm_mday = 1;
	time.tm_hour = this->_hour;
	time.tm_min = this->_minute;
	time.tm_sec = this->_second;
	time.tm_wday = 1;
	time.tm_yday = 1;
	time.tm_isdst = -1;
	auto microsecond = [this]() -> long long { return this->microsecond(); };
	auto utc_offset = [this]() -> std::shared_ptr<Timedelta> { return this->utc_offset(); };
	auto tz_name = [this]() -> std::string { return this->tz_name(); };

	return internal::_wrap_strftime(fmt, time, microsecond, utc_offset, tz_name);
}

std::shared_ptr<Timedelta> Time::utc_offset() const
{
	if (this->_tz_info == nullptr)
	{
		return nullptr;
	}

	auto offset = this->_tz_info->utc_offset(nullptr);
	internal::_check_utc_offset("utcoffset", offset.get());
	return offset;
}

std::string Time::tz_name() const
{
	if (this->_tz_info == nullptr)
	{
		return "";
	}

	return this->_tz_info->tz_name(nullptr);
}

std::shared_ptr<Timedelta> Time::dst() const
{
	if (this->_tz_info == nullptr)
	{
		return nullptr;
	}

	auto offset = this->_tz_info->dst(nullptr);
	internal::_check_utc_offset("dst", offset.get());
	return offset;
}

Time Time::replace(
	short int hour,
	short int minute,
	short int second,
	int microsecond,
	bool this_tz_info,
	const std::shared_ptr<Timezone>& tz,
	short int fold
) const
{
	if (hour < 0)
	{
		hour = this->_hour;
	}

	if (minute < 0)
	{
		minute = this->_minute;
	}

	if (second < 0)
	{
		second = this->_second;
	}

	if (microsecond < 0)
	{
		microsecond = this->_microsecond;
	}

	std::shared_ptr<Timezone> tz_info_;
	if (this_tz_info)
	{
		tz_info_ = this->_tz_info ? this->_tz_info->ptr_copy() : nullptr;
	}
	else
	{
		tz_info_ = tz ? tz->ptr_copy() : nullptr;
	}

	if (fold < 0)
	{
		fold = this->_fold;
	}

	return Time(hour, minute, second, microsecond, tz_info_, fold);
}

const Time Time::MIN = Time(0, 0, 0);
const Time Time::MAX = Time(23, 59, 59, 999999);
const Timedelta Time::RESOLUTION = Timedelta(0, 0, 1);

// Datetime class definitions.
Datetime Datetime::_from_timestamp(
	double t, bool utc, const std::shared_ptr<Timezone>& tz
)
{
	auto frac = std::modf(t, &t);
	auto us = std::round(frac * 1e6);
	if (us >= 1000000)
	{
		t += 1;
		us -= 1000000;
	}
	else if (us < 0)
	{
		t -= 1;
		us += 1000000;
	}

	std::function<tm_tuple(double)> converter;
	if (utc)
	{
		converter = [](double t) -> tm_tuple {
			time_t tt = t;
			return internal::_gmtime(&tt);
		};
	}
	else
	{
		converter = [](double t) -> tm_tuple {
			time_t tt = t;
			return internal::_localtime(&tt);
		};
	}

	auto ct = converter(t);
	ct.tm_sec = std::min(ct.tm_sec, 59);
	auto result = Datetime(
		ct.tm_year, ct.tm_mon+1, ct.tm_mday,
		ct.tm_hour, ct.tm_min, ct.tm_sec, us, tz
	);
	if (tz == nullptr)
	{
		// As of version 2015f max fold in IANA database is
		// 23 hours at 1969-09-30 13:00:00 in Kwajalein.
		// Let's probe 24 hours in the past to detect a transition:
		double max_fold_seconds = 24*3600;

		// On Windows localtime_s throws an OSError for negative values,
		// thus we can't perform fold detection for values of time less
		// than the max time fold. See comments in _datetime module's
		// version of this method for more details.
		#if defined(_WIN32) || defined(_WIN64)
		if (t < max_fold_seconds)
		{
			return result;
		}
		#endif

		ct = converter(t - max_fold_seconds);
		auto probe1 = Datetime(
			ct.tm_year, ct.tm_mon+1, ct.tm_mday,
			ct.tm_hour, ct.tm_min, ct.tm_sec, us, tz
		);
		auto trans = result - probe1 - Timedelta(0, (long long int)max_fold_seconds);
		if (trans.days() < 0)
		{
			ct = converter(t + trans / Timedelta(0, 1));
			auto probe2 = Datetime(
				ct.tm_year, ct.tm_mon+1, ct.tm_mday,
				ct.tm_hour, ct.tm_min, ct.tm_sec, us, tz
			);
			if (probe2 == result)
			{
				result._fold = 1;
			}
		}
	}
	else
	{
		result = tz->from_utc(&result);
	}

	return result;
}

time_t Datetime::_mk_time() const
{
	auto epoch = Datetime(1970, 1, 1);
	auto max_fold_seconds = 24 * 3600;
	time_t t = (*this - epoch) / Timedelta(0, 1);
	auto local = [epoch](time_t u) -> double
	{
		auto tm_ = internal::_localtime(&u);

		auto sub = Datetime(
			tm_.tm_year, tm_.tm_mon+1, tm_.tm_mday,
			tm_.tm_hour, tm_.tm_min, tm_.tm_sec
		) - epoch;

		return sub / Timedelta(0, 1);
	};

	// Our goal is to solve t = local(u) for u.
	auto a = local(t) - t;
	auto u1 = t - a;
	auto t1 = local(u1);
	double b;
	if (t1 == t)
	{
		// We found one solution, but it may not be the one we need.
		// Look for an earlier solution (if `fold` is 0), or a
		// later one (if `fold` is 1).
		int num = 0;
		switch (this->fold())
		{
			case 1:
				num = max_fold_seconds;
				break;
			case 0:
				num = -max_fold_seconds;
				break;
			default:
				throw std::invalid_argument("Invalid fold value");
		}
		auto u2 = u1 + num;
		b = local(u2) - u2;
		if (a == b)
		{
			return u1;
		}
	}
	else
	{
		b = t1 - u1;
		assert(a != b);
	}

	auto u2 = t - b;
	auto t2 = local(u2);
	if (t2 == t)
	{
		return u2;
	}

	if (t1 == t)
	{
		return u1;
	}

	// We have found both offsets a and b, but neither t - a nor t - b is
	// a solution.  This means t is in the gap.
	switch (this->fold())
	{
		case 1:
			return std::min(u1, u2);
			break;
		case 0:
			return std::max(u1, u2);
			break;
		default:
			throw std::invalid_argument("Invalid fold value");
	}
}

Timezone Datetime::_local_timezone() const
{
	time_t ts;
	if (!this->_tz_info)
	{
		ts = this->_mk_time();
	}
	else
	{
		ts = (*this - internal::_EPOCH) / Timedelta(0, 1);
	}

	auto local_tm = internal::_localtime(&ts);
	auto local = Datetime(
		local_tm.tm_year, local_tm.tm_mon+1, local_tm.tm_mday,
		local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec
	);
	if (!this->_tz_info)
	{
		internal::_mk_tz_info(&local_tm);
	}

	// Extract TZ data
	auto gmt_off = local_tm.tm_gmtoff;

	char buffer[10];
	std::strcpy(buffer, local_tm.tm_zone);
	std::string zone = buffer;
	return Timezone(Timedelta(0, gmt_off), zone);
}

Datetime::Datetime(
	ushort year, ushort month, ushort day,
	ushort hour, ushort minute, ushort second,
	uint microsecond,
	const std::shared_ptr<Timezone>& tz_info,
	ushort fold
) : Date(year, month, day)
{
	internal::_check_time_fields(hour, minute, second, microsecond, fold);
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;
	this->_microsecond = microsecond;
	this->_tz_info = tz_info ? tz_info->ptr_copy() : nullptr;
	this->_fold = fold;
}

Datetime::Datetime(const Datetime& other)
 : Date(other._year, other._month, other._day)
{
	if (this != &other)
	{
		this->_hour = other._hour;
		this->_minute = other._minute;
		this->_second = other._second;
		this->_microsecond = other._microsecond;
		this->_tz_info = other._tz_info ? other._tz_info->ptr_copy() : nullptr;
		this->_fold = other._fold;
	}
}

ushort Datetime::hour() const
{
	return this->_hour;
}

ushort Datetime::minute() const
{
	return this->_minute;
}

ushort Datetime::second() const
{
	return this->_second;
}

uint Datetime::microsecond() const
{
	return this->_microsecond;
}

Timezone* Datetime::tz_info() const
{
	return this->_tz_info.get();
}

ushort Datetime::fold() const
{
	return this->_fold;
}

Datetime Datetime::from_timestamp(
	double t, const std::shared_ptr<Timezone>& tz
)
{
	return Datetime::_from_timestamp(t, tz != nullptr, tz);
}

Datetime Datetime::utc_from_timestamp(double t)
{
	return Datetime::_from_timestamp(t, true, nullptr);
}

Datetime Datetime::now(const std::shared_ptr<Timezone>& tz)
{
	double t = internal::_time();
	return Datetime::from_timestamp(t, tz);
}

Datetime Datetime::utc_now()
{
	double t = internal::_time();
	return Datetime::utc_from_timestamp(t);
}

Datetime Datetime::combine(
	const Date& date, const Time& time, bool tz_info
)
{
	std::shared_ptr<Timezone> tz;
	if (tz_info)
	{
		auto tz_info_p = time.tz_info();
		tz = tz_info_p ? tz_info_p->ptr_copy() : nullptr;
	}

	return Datetime(
		date.year(), date.month(), date.day(),
		time.hour(), time.minute(), time.second(),
		time.microsecond(), tz, time.fold()
	);
}

Datetime Datetime::from_iso_format(const std::string& date_str)
{
	// Split this at the separator
	auto d_str = date_str.substr(0, 10);
	auto t_str = date_str.substr(11);

	auto date_comps = internal::_parse_isoformat_date(d_str);
	internal::hmsfz time_comps;
	if (!t_str.empty())
	{
		time_comps = internal::_parse_isoformat_time(t_str);
	}
	else
	{
		time_comps = {0, 0, 0, 0, nullptr};
	}

	return Datetime(
		date_comps.year, date_comps.month, date_comps.day,
		time_comps.hh, time_comps.mm, time_comps.ss, time_comps.ff, time_comps.tz
	);
}

tm_tuple Datetime::time_tuple() const
{
	auto dst = this->dst();
	int dst_flag;
	if (dst == nullptr)
	{
		dst_flag = -1;
	}
	else if ((bool)dst)
	{
		dst_flag = 1;
	}
	else
	{
		dst_flag = 0;
	}

	return internal::_build_struct_time(
		this->_year, this->_month, this->_day,
		this->_hour, this->_minute, this->_second, dst_flag
	);
}

double Datetime::timestamp() const
{
	if (this->_tz_info == nullptr)
	{
		double s = this->_mk_time();
		return s + (double)this->_microsecond / 1e6;
	}
	else
	{
		return (double)(*this - internal::_EPOCH).total_seconds();
	}
}

tm_tuple Datetime::utc_time_tuple()
{
	auto offset = this->utc_offset();
	if (offset)
	{
		this->operator-=(*offset);
	}

	return internal::_build_struct_time(
		this->_year, this->_month, this->_day,
		this->_hour, this->_minute, this->_second, 0
	);
}

Date Datetime::date() const
{
	return Date(this->_year, this->_month, this->_day);
}

Time Datetime::time() const
{
	return Time(
		this->_hour, this->_minute, this->_second,
		this->_microsecond, nullptr, this->_fold
	);
}

Time Datetime::time_tz() const
{
	return Time(
		this->_hour, this->_minute, this->_second, this->_microsecond,
		this->_tz_info->ptr_copy(), this->_fold
	);
}

Datetime Datetime::replace(
	ushort year, ushort month, ushort day,
	short int hour, short int minute, short int second,
	int microsecond, bool this_tz_info,
	const std::shared_ptr<Timezone>& tz, short int fold
) const
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

	if (hour < 0)
	{
		hour = this->_hour;
	}

	if (minute < 0)
	{
		minute = this->_minute;
	}

	if (second < 0)
	{
		second = this->_second;
	}

	if (microsecond < 0)
	{
		microsecond = this->_microsecond;
	}

	std::shared_ptr<Timezone> time_zone = nullptr;
	if (this_tz_info)
	{
		time_zone = this->_tz_info ? this->_tz_info->ptr_copy() : nullptr;
	}
	else
	{
		time_zone = tz ? tz->ptr_copy() : nullptr;
	}

	if (fold < 0)
	{
		fold = this->_fold;
	}

	return Datetime(
		year, month, day, hour, minute,
		second, microsecond, tz, fold
	);
}

Datetime Datetime::as_timezone(
	std::shared_ptr<Timezone> tz
) const
{
	if (tz == nullptr)
	{
		tz = this->_local_timezone().ptr_copy();
	}

	auto my_tz = this->_tz_info;
	std::shared_ptr<Timedelta> my_offset = nullptr;
	if (!my_tz)
	{
		my_tz = this->_local_timezone().ptr_copy();
		my_offset = my_tz->utc_offset(this);
	}
	else
	{
		my_offset = my_tz->utc_offset(this);
		if (my_offset == nullptr)
		{
			my_tz = this->replace(
				0, 0, 0, -1, -1, -1, -1, false
			)._local_timezone().ptr_copy();
			my_offset = my_tz->utc_offset(this);
		}
	}

	if (*tz == *my_tz)
	{
		auto new_dt = *this;
		if (!this->_tz_info)
		{
			new_dt._tz_info = my_tz->ptr_copy();
		}

		return new_dt;
	}

	// Convert self to UTC, and attach the new time zone object.
	auto utc = (*this - *my_offset).replace(
		0, 0, 0, -1, -1, -1, -1, false, tz
	);

	// Convert from UTC to tz's local time.
	return tz->from_utc(&utc);
}

std::string Datetime::ctime() const
{
	auto weekday = internal::_mod(this->to_ordinal(), 7);
	if (!weekday)
	{
		weekday = 7;
	}

	return internal::_DAY_NAMES[weekday] + " " +
		internal::_MONTH_NAMES[this->_month] + " " +
		internal::_lf(this->_day, 2, ' ') + " " +
		internal::_lf(this->_hour) + ":" +
		internal::_lf(this->_minute) + ":" +
		internal::_lf(this->_second) + " " +
		internal::_lf(this->_year, 4);
}

std::string Datetime::iso_format(char sep, time_spec ts) const
{
	auto s = internal::_lf(this->_year, 4) + "-" +
		internal::_lf(this->_month) + "-" +
		internal::_lf(this->_day) + sep +
		internal::_format_time(
			this->_hour, this->_minute,
			this->_second, this->_microsecond, ts
		);
	auto off = this->utc_offset();
	auto tz = internal::_format_offset(off.get());
	if (!tz.empty())
	{
		s += tz;
	}

	return s;
}

std::string Datetime::str() const
{
	return this->iso_format(' ');
}

std::string Datetime::strftime(const std::string& fmt) const
{
	auto microsecond = [this]() -> long long { return this->microsecond(); };
	auto utc_offset = [this]() -> std::shared_ptr<Timedelta> { return this->utc_offset(); };
	auto tz_name = [this]() -> std::string { return this->tz_name(); };

	return internal::_wrap_strftime(
		fmt, this->time_tuple(), microsecond, utc_offset, tz_name
	);
}

Datetime Datetime::strptime(const std::string& date_str, const char* format)
{
	return internal::_strptime_datetime(date_str, format);
}

std::shared_ptr<Timedelta> Datetime::utc_offset() const
{
	if (this->_tz_info == nullptr)
	{
		return nullptr;
	}

	auto offset = this->_tz_info->utc_offset(this);
	internal::_check_utc_offset("utcoffset", offset.get());
	return offset;
}

std::string Datetime::tz_name() const
{
	if (this->_tz_info == nullptr)
	{
		return "";
	}

	return this->_tz_info->tz_name(this);
}

std::shared_ptr<Timedelta> Datetime::dst() const
{
	if (this->_tz_info == nullptr)
	{
		return nullptr;
	}

	auto offset = this->_tz_info->dst(this);
	internal::_check_utc_offset("dst", offset.get());
	return offset;
}

bool Datetime::operator == (const Datetime& other) const
{
	return this->_cmp(other, true) == 0;
}

bool Datetime::operator != (const Datetime& other) const
{
	return this->_cmp(other, true) != 0;
}

bool Datetime::operator <= (const Datetime& other) const
{
	return this->_cmp(other) <= 0;
}

bool Datetime::operator < (const Datetime& other) const
{
	return this->_cmp(other) < 0;
}

bool Datetime::operator >= (const Datetime& other) const
{
	return this->_cmp(other) >= 0;
}

bool Datetime::operator > (const Datetime& other) const
{
	return this->_cmp(other) > 0;
}

signed char Datetime::_cmp(const Datetime& other, bool allow_mixed) const
{
	auto* my_tz = this->_tz_info.get();
	auto* ot_tz = other._tz_info.get();
	std::shared_ptr<Timedelta> my_off, ot_off;
	bool base_compare;
	if ((!my_tz && !ot_tz) || (my_tz && ot_tz && *my_tz == *ot_tz))
	{
		base_compare = true;
	}
	else
	{
		my_off = this->utc_offset();
		ot_off = other.utc_offset();

		// Assume that allow_mixed means that we are called from operator==
		if (allow_mixed)
		{
			auto this_rep_off = this->replace(
				0, 0, 0, -1, -1, -1, -1, true, nullptr, !this->fold()
			).utc_offset();
			if ((!my_off && this_rep_off) || (my_off && !this_rep_off) || *my_off != *this_rep_off)
			{
				return 2;
			}

			auto ot_rep_off = other.replace(
				0, 0, 0, -1, -1, -1, -1, true, nullptr, !this->fold()
			).utc_offset();
			if ((!ot_off && ot_rep_off) || (ot_off && !ot_rep_off) || *ot_off != *ot_rep_off)
			{
				return 2;
			}
		}

		base_compare = (!my_off && !ot_off) || (my_off && ot_off && *my_off == *ot_off);
	}

	if (base_compare)
	{
		const short arr_sz = 7;
		uint left[arr_sz] = {
			this->_year, this->_month, this->_day,
			this->_hour, this->_minute, this->_second,
			this->_microsecond
		};
		uint right[arr_sz] = {
			other._year, other._month, other._day,
			other._hour, other._minute, other._second,
			other._microsecond
		};
		return internal::_cmp_arr(left, right, arr_sz);
	}

	if (!my_off || !ot_off)
	{
		if (allow_mixed)
		{
			return 2; // arbitrary non-zero value
		}
		else
		{
			throw std::invalid_argument("cannot compare naive and aware times");
		}
	}

	// XXX What follows could be done more efficiently...
	auto diff = *this - other;
	if (diff.days() < 0)
	{
		return -1;
	}

	return diff ? 1 : 0;
}

Datetime Datetime::operator + (const Timedelta& other) const
{
	auto delta = Timedelta(
		this->to_ordinal(), this->_second,
		this->_microsecond, 0, this->_minute, this->_hour
	);
	delta += other;

	auto h_r = internal::_div_mod(delta.seconds(), (long long)3600);
	auto hour = h_r.first;
	auto rem = h_r.second;

	auto m_s = internal::_div_mod(rem, (long long)60);
	auto minute = m_s.first;
	auto second = m_s.second;

	if (delta.days() > 0 && delta.days() <= internal::_MAX_ORDINAL)
	{
		return Datetime::combine(
			Date::from_ordinal(delta.days()),
			Time(hour, minute, second, delta.microseconds(), this->_tz_info)
		);
	}

	throw std::overflow_error("result out of range");
}

Datetime& Datetime::operator = (const Datetime& other)
{
	if (&other != this)
	{
		this->_hour = other._hour;
		this->_minute = other._minute;
		this->_second = other._second;
		this->_microsecond = other._microsecond;
		this->_tz_info = other._tz_info ? other._tz_info->ptr_copy() : nullptr;
		this->_fold = other._fold;
		this->_year = other._year;
		this->_month = other._month;
		this->_day = other._day;
	}

	return *this;
}

Datetime& Datetime::operator += (const Timedelta& other)
{
	this->operator=(*this + other);
	return *this;
}

Datetime& Datetime::operator -= (const Timedelta& other)
{
	this->operator=(*this - other);
	return *this;
}

Timedelta Datetime::operator - (const Datetime& other) const
{
	auto days1 = this->to_ordinal();
	auto days2 = other.to_ordinal();
	auto secs1 = this->_second + this->_minute * 60 + this->_hour * 3600;
	auto secs2 = other._second + other._minute * 60 + other._hour * 3600;
	auto base = Timedelta(
		(long long)(days1 - days2),
		secs1 - secs2,
		this->_microsecond - other._microsecond
	);

	if (
		(!this->_tz_info && !other._tz_info) ||
		(this->_tz_info && other._tz_info && *this->_tz_info == *other._tz_info)
	)
	{
		return base;
	}

	auto my_off = this->utc_offset();
	auto ot_off = other.utc_offset();
	if ((!my_off && !ot_off) || (my_off && ot_off && *my_off == *ot_off))
	{
		return base;
	}

	if (!my_off || !ot_off)
	{
		throw std::invalid_argument("cannot mix naive and timezone-aware time");
	}

	return base + *ot_off - *my_off;
}

Datetime Datetime::operator - (const Timedelta& other) const
{
	return *this + -other;
}

const Datetime Datetime::MIN = Datetime(1, 1, 1);
const Datetime Datetime::MAX = Datetime(9999, 12, 31, 23, 59, 59, 999999);
const Timedelta Datetime::RESOLUTION = Timedelta(0, 0, 1);

// Timezone class definitions.
const std::string Timezone::_Omitted = "Omitted";

const Timezone Timezone::UTC = Timezone::_create(Timedelta(0));
const Timezone Timezone::MIN = Timezone::_create(-Timedelta(0, 0, 0, 0, 59, 23));
const Timezone Timezone::MAX = Timezone::_create(Timedelta(0, 0, 0, 0, 59, 23));

const Timedelta Timezone::_max_offset = Timedelta(0, 0, -1, 0, 0, 24);
const Timedelta Timezone::_min_offset = -Timezone::_max_offset;

Timezone::Timezone(const Timedelta& offset, const std::string& name)
{
	if (name == Timezone::_Omitted)
	{
		if (!offset)
		{
			this->operator=(Timezone::UTC);
			return;
		}
		else
		{
			this->_name = "";
		}
	}
	else if (name.empty())
	{
		throw std::invalid_argument("name can not be empty");
	}

	if (!(offset >= Timezone::_min_offset && offset <= Timezone::_max_offset))
	{
		throw std::invalid_argument(
			"offset must be a timedelta strictly between"
			"-timedelta(hours=24) and timedelta(hours=24)."
		);
	}

	this->_offset = std::make_shared<Timedelta>(offset);
	this->_name = name;
}

Timezone Timezone::_create(
	const Timedelta& offset,
	const std::string& name
)
{
	Timezone self;
	self._offset = std::make_shared<Timedelta>(offset);
	self._name = name;
	return self;
}

std::shared_ptr<Timezone> Timezone::ptr_copy() const
{
	return std::make_shared<Timezone>(*this);
}

Timezone& Timezone::operator = (const Timezone& other)
{
	if (this != &other)
	{
		this->_offset = other._offset ? std::make_shared<Timedelta>(*other._offset) : nullptr;
		this->_name = other._name;
	}

	return *this;
}

bool Timezone::operator == (const Timezone& other) const
{
	return *this->_offset == *other._offset;
}

bool Timezone::operator != (const Timezone& other) const
{
	return *this->_offset != *other._offset;
}

std::string Timezone::str() const
{
	return this->tz_name(nullptr);
}

std::shared_ptr<Timedelta> Timezone::utc_offset(const Datetime*) const
{
	if (!this->_offset)
	{
		return nullptr;
	}

	return std::make_shared<Timedelta>(*this->_offset);
}

std::string Timezone::tz_name(const Datetime*) const
{
	if (this->_name.empty())
	{
		return Timezone::_name_from_offset(this->_offset.get());
	}

	return this->_name;
}

std::shared_ptr<Timedelta> Timezone::dst(const Datetime*) const
{
	return nullptr;
}

/*
Datetime Timezone::from_utc(Datetime dt) const
{
	auto tzinfo = dt.tz_info();
	if (!tzinfo || (*tzinfo != *this))
	{
		throw std::invalid_argument("dt.tz_info() is not this");
	}

	auto dt_off = dt.utc_offset();
	if (!dt_off)
	{
		throw std::invalid_argument("from_utc() requires a non-null utc_offset() result");
	}

	// See the long comment block at the end of this file for an
	// explanation of this algorithm.
	auto dt_dst = dt.dst();
	if (!dt_dst)
	{
		throw std::invalid_argument("from_utc() requires a non-null dst() result");
	}

	auto delta = *dt_off - *dt_dst;
	if (delta)
	{
		dt += delta;
		dt_dst = dt.dst();
		if (!dt_dst)
		{
			throw std::invalid_argument(
				"from_utc(): dt.dst() gave inconsistent results; cannot convert"
			);
		}
	}

	return dt + *dt_dst;
}
*/

Datetime Timezone::from_utc(const Datetime* dt) const
{
	if (dt)
	{
		auto tzinfo = dt->tz_info();
		if (!tzinfo || (*tzinfo != *this))
		{
			throw std::invalid_argument("from_utc: dt.tz_info() is not this");
		}

		return *dt + *this->_offset;
	}

	throw std::invalid_argument("from_utc() argument must be instantiated");
}

std::string Timezone::_name_from_offset(const Timedelta* delta)
{
	if (!delta)
	{
		return "UTC";
	}

	auto delta_obj = *delta;
	std::string result = "UTC";
	if (delta_obj < Timedelta(0))
	{
		result += "-";
		delta_obj = -delta_obj;
	}
	else
	{
		result += "+";
	}

	auto td1h = Timedelta(0, 0, 0, 0, 0, 1);
	auto hours = delta_obj / td1h;
	auto rest = delta_obj % td1h;

	auto td1m = Timedelta(0, 0, 0, 0, 1);
	auto minutes = rest / td1m;
	rest = rest % td1m;

	auto seconds = rest.seconds();
	auto microseconds = rest.microseconds();

	result += internal::_lf(hours) + ":" + internal::_lf(minutes);
	if (microseconds)
	{
		result += ":" + internal::_lf(seconds) + "." + internal::_lf(microseconds, 6);
	}
	else if (seconds)
	{
		result += ":" + internal::_lf(seconds);
	}

	return result;
}

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

void _check_utc_offset(
	const std::string& name, const Timedelta* offset
)
{
	assert(name == "utcoffset" || name == "dst");
	if (!offset)
	{
		return;
	}

	if (!(*offset > -Timedelta(1) && *offset < Timedelta(1)))
	{
		throw std::invalid_argument(
			name + "() = " + offset->str() +
			", must be strictly between -timedelta(hours=24) and timedelta(hours=24)"
		);
	}
}

std::string _format_offset(const Timedelta* off)
{
	std::string s;
	if (off)
	{
		auto offset = *off;
		if (offset.days() < 0)
		{
			s += "-";
			offset = -offset;
		}
		else
		{
			s += "+";
		}

		auto td1h = Timedelta(0, 0, 0, 0, 0, 1);
		auto hh = offset / td1h;
		auto mm = offset % td1h;

		auto td1m = Timedelta(0, 0, 0, 0, 1);
		auto mm_int = mm / td1m;
		auto ss = mm % td1m;

		s += _lf(hh) + ":" + _lf(mm_int);
		if (ss || ss.microseconds())
		{
			s += ":" + _lf(ss.seconds());
			if (ss.microseconds())
			{
				s += "." + _lf(ss.seconds(), 6);
			}
		}
	}

	return s;
}

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

hmsfz _parse_isoformat_time(const std::string& t_str)
{
	// Format supported is HH[:MM[:SS[.fff[fff]]]][+HH:MM[:SS[.ffffff]]]
	auto len_str = t_str.size();
	if (len_str < 2)
	{
		throw std::invalid_argument("Isoformat time too short");
	}

	// This is equivalent to regex::search('[+-]', t_str), but faster
	auto tz_pos = t_str.find('-');
	if (tz_pos == std::string::npos)
	{
		tz_pos = t_str.find('+');
	}

	if (tz_pos == std::string::npos)
	{
		tz_pos = -1;
	}

	tz_pos++;
	auto time_str = tz_pos > 0 ? t_str.substr(0, tz_pos - 1) : t_str;
	auto hmsf_comps = _parse_hh_mm_ss_ff(time_str);
	hmsfz time_comps{};
	time_comps.hh = hmsf_comps.hh;
	time_comps.mm = hmsf_comps.mm;
	time_comps.ss = hmsf_comps.ss;
	time_comps.ff = hmsf_comps.ff;
	if (tz_pos > 0)
	{
		auto tz_str = t_str.substr(tz_pos);

		// Valid time zone strings are:
		//  HH:MM               len: 5
		//  HH:MM:SS            len: 8
		//  HH:MM:SS.ffffff     len: 15
		auto tz_str_len = tz_str.size();
		if (tz_str_len != 5 && tz_str_len != 8 && tz_str_len != 15)
		{
			throw std::invalid_argument("Malformed time zone string");
		}

		auto tz_comps = _parse_hh_mm_ss_ff(tz_str);
		if (
			tz_comps.hh == 0 && tz_comps.mm == 0 &&
			tz_comps.ss == 0 && tz_comps.ff == 0
		)
		{
			time_comps.tz = Timezone::UTC.ptr_copy();
		}
		else
		{
			int tz_sign = t_str[tz_pos - 1] == '-' ? -1 : 1;
			auto td = Timedelta(
				0, tz_comps.ss, tz_comps.ff, 0, tz_comps.mm, tz_comps.hh
			);
			time_comps.tz = Timezone(tz_sign * td).ptr_copy();
		}
	}

	return time_comps;
}

__DATETIME_INTERNAL_END__
