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
 * core/datetime.h
 *
 * Purpose:
 *  C++ implementation of Python's datetime utilities.
 *  (Not finished yet, check "!IMPROVEMENT!" keywords)
 *
 *  Concrete date/time and related types.
 *
 *  See http://www.iana.org/time-zones/repository/tz-link.html for
 *  time zone and DST data sources.
 */

#pragma once

// C++ libraries.
#include <memory>
#include <functional>

// Module definitions.
#include "./_def_.h"


__DATETIME_INTERNAL_BEGIN__

extern void _M_Assert(
	const char* expr_str,
	bool expr, const char* function,
	int /* line */, const char* msg
);

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

enum time_spec
{
	AUTO, HOURS, MINUTES, SECONDS, MILLISECONDS, MICROSECONDS
};

#ifndef ushort
typedef unsigned short int ushort;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#ifdef _MSC_VER
#define m_assert(_expr, _msg) \
	internal::_M_Assert(#_expr, _expr, __FUNCTION__, __LINE__, _msg)
#else
#define m_assert(_expr, _msg) \
	internal::_M_Assert(#_expr, _expr, __PRETTY_FUNCTION__, __LINE__, _msg)
#endif

struct tm_tuple
{
	int tm_sec;             /* Seconds (0-60) */
	int tm_min;             /* Minutes (0-59) */
	int tm_hour;            /* Hours (0-23) */
	int tm_mday;            /* Day of the month (1-31) */
	int tm_mon;             /* Month (0-11) */
	int tm_year;            /* Year - 1900 */
	int tm_wday;            /* Day of the week (0-6, Sunday = 0) */
	int tm_yday;            /* Day in the year (0-365, 1 Jan = 0) */
	int tm_isdst;           /* Daylight saving time */
	long int tm_gmtoff;     /* Seconds east of UTC.  */
	const char* tm_zone;	/* Timezone abbreviation.  */

	[[nodiscard]] struct tm as_tm() const
	{
		struct tm res{};
		res.tm_sec = this->tm_sec;
		res.tm_min = this->tm_min;
		res.tm_hour = this->tm_hour;
		res.tm_mday = this->tm_mday;
		res.tm_mon = this->tm_mon;
		res.tm_year = this->tm_year;
		res.tm_wday = this->tm_wday;
		res.tm_yday = this->tm_yday;
		res.tm_isdst = this->tm_isdst;
#if defined(__unix__) || defined(__linux__)
		res.tm_gmtoff = this->tm_gmtoff;
		res.tm_zone = this->tm_zone;
#endif
		return res;
	}

	tm_tuple() = default;

	explicit tm_tuple(tm* t)
	{
		this->tm_sec = t->tm_sec;
		this->tm_min = t->tm_min;
		this->tm_hour = t->tm_hour;
		this->tm_mday = t->tm_mday;
		this->tm_mon = t->tm_mon;
		this->tm_year = t->tm_year;
		this->tm_wday = t->tm_wday;
		this->tm_yday = t->tm_yday;
		this->tm_isdst = t->tm_isdst;
#if defined(_WIN32) || defined(_WIN64)
		this->tm_gmtoff = 0;
		this->tm_zone = "";
#elif defined(__unix__) || defined(__linux__)
		this->tm_gmtoff = t->tm_gmtoff;
		this->tm_zone = t->tm_zone;
#else
#error Library is not supported on this platform
#endif
	}
};

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

template <typename T1, typename T2>
T1 _mod(const T1& a, const T2& b)
{
	return (b + (a % b)) % b;
}

// !IMPORTANT! Use only integer as ResultT!
template <typename T1, typename T2, typename ResultT>
ResultT _true_div(T1 a, T2 b)
{
	ResultT r = a / b;
	return r < 0 ? r - 1 : r;
}

template <typename T1, typename T2>
std::pair<T1, T1> _div_mod(const T1& a, const T2& b)
{
	return {_true_div<T1, T2, T1>(a, b), _mod(a, b)};
}

template <typename T>
signed char _cmp(T left, T right)
{
	if (left == right)
	{
		return 0;
	}

	return left > right ? 1 : -1;
}

template <typename T>
signed char _cmp_arr(const T* left, const T* right, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		auto l_r = internal::_cmp(left[i], right[i]);
		if (l_r != 0)
		{
			return l_r;
		}
	}

	return 0;
}

const uint _MAX_ORDINAL = 3652059;

const signed char _MONTHS_COUNT = 12;

const signed char _DAYS_IN_MONTH[_MONTHS_COUNT + 1] = {
	-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

const short int _DAYS_BEFORE_MONTH[_MONTHS_COUNT + 1] = {
	-1,             // a placeholder for indexing purposes
	0,              // Jan
	31,             // Feb
	31+28,          // Mar
	2*31+28,        // Apr
	2*31+28+30,     // May
	3*31+28+30,     // Jun
	3*31+28+2*30,   // Jul
	4*31+28+2*30,   // Aug
	5*31+28+2*30,   // Sep
	5*31+28+3*30,   // Oct
	6*31+28+3*30,   // Nov
	6*31+28+4*30    // Dec
};

// year -> true if leap year, else false.
extern bool _is_leap(ushort year);

// year -> number of days before January 1st of year.
extern size_t _days_before_year(ushort year);

// year, month -> number of days in that month in that year.
extern int _days_in_month(ushort year, ushort month);

// year, month -> number of days in year preceding first day of month.
extern size_t _days_before_month(ushort year, ushort month);

// year, month, day -> ordinal, considering 01-Jan-0001 as day 1.
extern size_t _ymd2ord(ushort year, ushort month, ushort day);

const size_t _DI400Y = _days_before_year(401);  // number of days in 400 years
const size_t _DI100Y = _days_before_year(101);  // ----------------- 100 -----
const size_t _DI4Y = _days_before_year(5);      // -----------------   4 -----

struct ymd
{
	ushort year;
	ushort month;
	ushort day;
};

struct hmsf
{
	ushort hh;
	ushort mm;
	ushort ss;
	uint ff;
};

// ordinal -> ymd{year, month, day}, considering 01-Jan-0001 as day 1.
extern ymd _ord2ymd(size_t n);

// Month and day names.  For localized versions, see the calendar module.
const std::string _MONTH_NAMES[_MONTHS_COUNT + 1] = {
	"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
const std::string _DAY_NAMES[7 + 1] = {
	"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

extern tm_tuple _build_struct_time(
	ushort y, ushort m, ushort d, ushort hh, ushort mm, ushort ss, int dst_flag
);

extern void _mk_tz_info(tm_tuple* time_tuple, bool is_gmt = false);

extern void _strptime(const char* _s, const char* _fmt, tm_tuple* _tp);

// Prepends 'c' 'w'-times and appends 'num' as std::string;
// if 'num' string is longer than 'w', returns 'num' as std::string.
extern std::string _lf(long long  num, int w = 2, char c = '0');

extern std::string _format_time(
	ushort hh, ushort mm, ushort ss, uint us, time_spec ts = time_spec::AUTO
);

// Helpers for parsing the result of isoformat()
extern ymd _parse_isoformat_date(const std::string& dt_str);

extern hmsf _parse_hh_mm_ss_ff(const std::string& t_str);

extern void _check_date_fields(ushort year, ushort month, ushort day);

extern void _check_time_fields(
	ushort hour, ushort minute, ushort second,
	uint microsecond, ushort fold
);

// Divide a by b and round result to the nearest integer.
//
// When the ratio is exactly half-way between two integers,
// the even integer is returned.
extern long long int _divide_and_round(long long int a, long long int b);

extern time_t _time();

extern tm_tuple _localtime(const time_t* _timer);
extern tm_tuple _gmtime(const time_t* _timer);

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

const ushort MIN_YEAR = 1;
const ushort MAX_YEAR = 9999;

// Represent the difference between two datetime objects.
//
// Supported operators:
//
// - add, subtract timedelta
// - unary plus, minus, abs
// - compare to timedelta
// - multiply, divide by int
//
// In addition, datetime supports subtraction of two datetime objects
// returning a timedelta, and addition or subtraction of a datetime
// and a timedelta giving a datetime.
//
// Representation: (days, seconds, microseconds).
class Timedelta final
{
private:
	long _days;
	long _seconds;
	long _microseconds;

private:
	[[nodiscard]] static std::string _plural(long n) ;
	[[nodiscard]] long long int _to_microseconds() const;
	[[nodiscard]] signed char _cmp(const Timedelta& other) const;

public:
	static const Timedelta MIN;
	static const Timedelta MAX;
	static const Timedelta RESOLUTION;

public:
	explicit Timedelta(
		long days = 0, long seconds = 0,
		long microseconds = 0, long milliseconds = 0,
		long minutes = 0, long hours = 0,
		long weeks = 0
	);
	[[nodiscard]] std::string str() const;

	// Total seconds in the duration.
	[[nodiscard]] double total_seconds() const;

	// Read-only field accessors.
	[[nodiscard]] long days() const;
	[[nodiscard]] long seconds() const;
	[[nodiscard]] long microseconds() const;

	Timedelta operator + (const Timedelta & other) const;
	Timedelta operator - (const Timedelta & other) const;
	Timedelta operator - () const;
	[[nodiscard]] Timedelta abs() const;
	Timedelta operator * (const long& other) const;
	friend Timedelta operator * (long left, const Timedelta& right);
	double operator / (const Timedelta& other) const;
	Timedelta operator / (const long& other) const;
	friend Timedelta operator / (long left, const Timedelta& right);
	Timedelta operator % (const Timedelta& other) const;

	Timedelta& operator = (const Timedelta& other);
	Timedelta& operator += (const Timedelta& other);

	bool operator == (const Timedelta& other) const;
	bool operator != (const Timedelta& other) const;
	bool operator <= (const Timedelta& other) const;
	bool operator < (const Timedelta& other) const;
	bool operator >= (const Timedelta& other) const;
	bool operator > (const Timedelta& other) const;

	explicit operator bool () const;
};

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

// name is the offset-producing method, "utcoffset" or "dst".
// offset is what it returned.
// If offset isn't zero, throws std::invalid_argument.
// If offset is zero, returns void.
// Else offset is checked for being in range.
// If it is, its integer value is returned.  Else std::invalid_argument is raised.
extern void _check_utc_offset(
	const std::string& name, const Timedelta* offset
);

extern std::string _format_offset(const Timedelta* off);

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

// Concrete date type.
//
// Constructors:
//  Date(), from_timestamp(), today(), from_ordinal()
//
// Operators:
//  ==, <=, <, >=, >, +, -
//
// Methods:
//  str(), timetuple(), to_ordinal(), weekday()
//  iso_weekday(), iso_calendar(), iso_format()
//  ctime(), strftime()
//
// Readonly fields' getters:
//  year(), month(), day()
class Date
{
protected:
	ushort _year;
	ushort _month;
	ushort _day;

protected:
	[[nodiscard]] signed char _cmp(const Date& other) const;

public:
	static const Date MIN;
	static const Date MAX;
	static const Timedelta RESOLUTION;

public:
	Date(ushort year, ushort month, ushort day);

	// Construct a date from a POSIX timestamp (like time()).
	static Date from_timestamp(time_t t);

	// Construct a date from time().
	static Date today();

	// Construct a date from a proleptic Gregorian ordinal.
	//
	// January 1 of year 1 is day 1.  Only the year, month and day are
	// non-zero in the result.
	static Date from_ordinal(size_t n);

	// Construct a date from the output of date.iso_format().
	static Date from_iso_format(const std::string& date_str);

	// Construct a date from the ISO year, week number and weekday.
	//
	// This is the inverse of the date.iso_calendar()
	static Date from_iso_calendar(ushort year, ushort week, ushort day);

	// XXX These shouldn't depend on time.localtime(), because that
	// clips the usable dates to [1970 .. 2038).  At least ctime() is
	// easily done without using strftime() -- that's better too because
	// strftime("%c", ...) is locale specific.

	// Return ctime() style string.
	[[nodiscard]] virtual std::string ctime() const;

	// Format using strftime().
	[[nodiscard]] virtual std::string strftime(const std::string& fmt) const;

	// Return the date formatted according to ISO.
	//
	// This is 'YYYY-MM-DD'.
	//
	// References:
	//  - http://www.w3.org/TR/NOTE-datetime
	//  - http://www.cl.cam.ac.uk/~mgk25/iso-time.html
	[[nodiscard]] std::string iso_format() const;

	[[nodiscard]] virtual std::string str() const;

	// Read-only field accessors.
	[[nodiscard]] ushort year() const;
	[[nodiscard]] ushort month() const;
	[[nodiscard]] ushort day() const;

	// Standard conversions, ==, !=, <=, <, >=, > and helpers.

	// Return local time of struct time_tuple type.
	[[nodiscard]] virtual tm_tuple time_tuple() const;

	// Return proleptic Gregorian ordinal for the year, month and day.
	//
	// January 1 of year 1 is day 1.  Only the year, month and day values
	// contribute to the result.
	[[nodiscard]] size_t to_ordinal() const;

	// Return a new date with new values for the specified fields.
	[[nodiscard]] Date replace(
		ushort year = 0, ushort month = 0, ushort day = 0
	) const;

	// Comparisons of date objects with other.
	bool operator == (const Date& other) const;
	bool operator != (const Date& other) const;
	bool operator <= (const Date& other) const;
	bool operator < (const Date& other) const;
	bool operator >= (const Date& other) const;
	bool operator > (const Date& other) const;

	// Computations

	// Add a date to a timedelta.
	Date operator + (const Timedelta& other) const;

	// Subtract two dates.
	Timedelta operator - (const Date& other) const;

	// Subtract a date and a timedelta.
	Date operator - (const Timedelta& other) const;

	// Return day of the week, where Monday == 0 ... Sunday == 6.
	[[nodiscard]] short int weekday() const;

	// Day-of-the-week and week-of-the-year, according to ISO

	// Return day of the week, where Monday == 1 ... Sunday == 7.
	[[nodiscard]] short int iso_weekday() const;

	// Return a 3-tuple containing ISO year, week number, and weekday.
	//
	// The first ISO week of the year is the (Mon-Sun) week
	// containing the year's first Thursday; everything else derives
	// from that.
	//
	// The first week is 1; Monday is 1 ... Sunday is 7.
	//
	// ISO calendar algorithm taken from
	// http://www.phys.uu.nl/~vgent/calendar/isocalendar.htm
	// (used with permission)
	[[nodiscard]] std::tuple<ushort, ushort, ushort> iso_calendar() const;
};

class Timezone;

// Time with time zone.
//
// Constructors:
//  Time()
//
// Operators:
//  ==, !=, <=, <, >=, >
//
// Methods:
//  str(), strftime(), iso_format()
//  utc_offset(), tz_name(), dst()
//
// Readonly-access member functions:
// hour, minute, second, microsecond, tz_info, fold
class Time final
{
private:
	ushort _hour;
	ushort _minute;
	ushort _second;
	uint _microsecond;
	std::shared_ptr<Timezone> _tz_info;
	ushort _fold;

public:
	static const Time MIN;
	static const Time MAX;
	static const Timedelta RESOLUTION;

public:
	// Constructor.
	//
	// Arguments:
	// hour, minute (required)
	// second, microsecond (default to zero)
	// tz_info (default to nullptr)
	// fold (keyword only, default to zero)
	explicit Time(
		ushort hour = 0,
		ushort minute = 0,
		ushort second = 0,
		uint microsecond = 0,
		const std::shared_ptr<Timezone>& tz_info = nullptr,
		ushort fold = 0
	);

	Time(const Time& other);

	// Read-only field accessors

	// hour (0-23)
	[[nodiscard]] ushort hour() const;

	// minute (0-59)
	[[nodiscard]] ushort minute() const;

	// second (0-59)
	[[nodiscard]] ushort second() const;

	// microsecond (0-999999)
	[[nodiscard]] uint microsecond() const;

	// pointer to TzInfo object
	[[nodiscard]] Timezone* tz_info() const;

	[[nodiscard]] ushort fold() const;

	// Standard conversions and helpers

	// Comparisons of Time object with other.
	bool operator == (const Time& other) const;
	bool operator != (const Time& other) const;
	bool operator <= (const Time& other) const;
	bool operator < (const Time& other) const;
	bool operator >= (const Time& other) const;
	bool operator > (const Time& other) const;

	[[nodiscard]] signed char _cmp(
		const Time& other, bool allow_mixed = false
	) const;

	// Return formatted timezone offset (+xx:xx) or an empty string.
	[[nodiscard]] std::string _tz_str() const;

	// Return the time formatted according to ISO.
	//
	// The full format is 'HH:MM:SS.mmmmmm+zz:zz'. By default, the fractional
	// part is omitted if self.microsecond == 0.
	//
	// The optional argument timespec specifies the number of additional
	// terms of the time to include.
	[[nodiscard]] std::string iso_format(
		time_spec ts = time_spec::AUTO
	) const;

	[[nodiscard]] std::string str() const;

	// Construct a time from the output of iso_format().
	static Time from_iso_format(const std::string& time_str);

	// Format using strftime(). The date part of the timestamp passed
	// to underlying strftime should not be used.
	[[nodiscard]] std::string strftime(const std::string& fmt) const;

	// Return the timezone offset as timedelta, positive east of UTC
	// (negative west of UTC).
	[[nodiscard]] std::shared_ptr<Timedelta> utc_offset() const;

	// Return the timezone name.
	//
	// Note that the name is 100% informational -- there's no requirement that
	// it mean anything in particular. For example, "GMT", "UTC", "-500",
	// "-5:00", "EDT", "US/Eastern", "America/New York" are all valid replies.
	[[nodiscard]] std::string tz_name() const;

	// Return 0 if DST is not in effect, or the DST offset (as timedelta
	// positive eastward) if DST is in effect.
	//
	// This is purely informational; the DST offset has already been added to
	// the UTC offset returned by utcoffset() if applicable, so there's no
	// need to consult dst() unless you're interested in displaying the DST
	// info.
	[[nodiscard]] std::shared_ptr<Timedelta> dst() const;

	// Return a new time with new values for the specified fields.
	// If this_tz_info is true, then set current _tz_info, otherwise set tz
	[[nodiscard]] Time replace(
		short int hour = -1,
		short int minute = -1,
		short int second = -1,
		int microsecond = -1,
		bool this_tz_info = true,
		const std::shared_ptr<Timezone>& tz = nullptr,
		short int fold = -1
	) const;
};

// Datetime(year, month, day[, hour[, minute[, second[, microsecond[,TzInfo]]]]])
//
// The year, month and day arguments are required. TzInfo may be None, or an
// instance of a TzInfo subclass. The remaining arguments may be ints.
class Datetime final : public Date
{
private:
	ushort _hour;
	ushort _minute;
	ushort _second;
	uint _microsecond;
	std::shared_ptr<Timezone> _tz_info;
	ushort _fold;

private:
	// Construct a datetime from a POSIX timestamp (like time.time()).
	//
	// A timezone info object may be passed in as well.
	static Datetime _from_timestamp(
		double t, bool utc, const std::shared_ptr<Timezone>& tz
	);

	// Return integer POSIX timestamp.
	[[nodiscard]] time_t _mk_time() const;

	[[nodiscard]] Timezone _local_timezone() const;

	[[nodiscard]] signed char _cmp(
		const Datetime& other, bool allow_mixed = false
	) const;

public:
	static const Datetime MIN;
	static const Datetime MAX;
	static const Timedelta RESOLUTION;

public:
	Datetime(
		ushort year, ushort month, ushort day,
		ushort hour = 0, ushort minute = 0, ushort second = 0,
		uint microsecond = 0,
		const std::shared_ptr<Timezone>& tz_info = nullptr,
		ushort fold = 0
	);

	Datetime(const Datetime& other);

	// Read-only field accessors.

	// hour (0-23)
	[[nodiscard]] ushort hour() const;

	// minute (0-59)
	[[nodiscard]] ushort minute() const;

	// second (0-59)
	[[nodiscard]] ushort second() const;

	// microsecond (0-999999)
	[[nodiscard]] uint microsecond() const;

	// pointer to TzInfo object
	[[nodiscard]] Timezone* tz_info() const;

	[[nodiscard]] ushort fold() const;

	// Construct a datetime from a POSIX timestamp (like time.time()).
	//
	// A timezone info object may be passed in as well.
	static Datetime from_timestamp(
		double t, const std::shared_ptr<Timezone>& tz = nullptr
	);

	// Construct a naive UTC datetime from a POSIX timestamp.
	static Datetime utc_from_timestamp(double t);

	// Construct a datetime from time.time() and optional time zone info.
	static Datetime now(const std::shared_ptr<Timezone>& tz = nullptr);

	// Construct a UTC datetime from time.time().
	static Datetime utc_now();

	// Construct a datetime from a given date and a given time.
	static Datetime combine(
		const Date& date, const Time& time, bool tz_info = true
	);

	// Construct a datetime from the output of datetime.iso_format().
	static Datetime from_iso_format(const std::string& date_str);

	// Return local time tuple compatible with time.localtime().
	[[nodiscard]] tm_tuple time_tuple() const override;

	// Return POSIX timestamp as double.
	[[nodiscard]] double timestamp() const;

	// Return UTC time tuple compatible with time.gmtime().
	tm_tuple utc_time_tuple();

	// Return the date part.
	[[nodiscard]] Date date() const;

	// Return the time part, with tz_info None.
	[[nodiscard]] Time time() const;

	// Return the time part, with same tz_info.
	[[nodiscard]] Time time_tz() const;

	// Return a new datetime with new values for the specified fields.
	//
	// If this_tz_info is true, then set current _tz_info, otherwise set tz
	[[nodiscard]] Datetime replace(
		ushort year = 0, ushort month = 0, ushort day = 0,
		short int hour = -1, short int minute = -1, short int second = -1,
		int microsecond = -1, bool this_tz_info = true,
		const std::shared_ptr<Timezone>& tz = nullptr, short int fold = -1
	) const;

	[[nodiscard]] Datetime as_timezone(
		std::shared_ptr<Timezone> tz = nullptr
	) const;

	// Ways to produce a string.

	// Return ctime() style string.
	[[nodiscard]] std::string ctime() const override;

	// Return the time formatted according to ISO.
	//
	// The full format looks like 'YYYY-MM-DD HH:MM:SS.mmmmmm'.
	// By default, the fractional part is omitted if this->_microsecond == 0.
	//
	// If this->_tz_info is not None, the UTC offset is also attached, giving
	// giving a full format of 'YYYY-MM-DD HH:MM:SS.mmmmmm+HH:MM'.
	//
	// Optional argument sep specifies the separator between date and
	// time, default 'T'.
	//
	// The optional argument timespec specifies the number of additional
	// terms of the time to include.
	[[nodiscard]] std::string iso_format(
		char sep = 'T', time_spec ts = time_spec::AUTO
	) const;

	// Convert to string, for str().
	[[nodiscard]] std::string str() const override;

	[[nodiscard]] std::string strftime(const std::string& fmt) const override;

	// string, format -> new datetime parsed from a string (like std::strptime()).
	static Datetime strptime(const std::string& date_str, const char* format);

	// Return the timezone offset as timedelta positive east of UTC (negative west of
	// UTC).
	[[nodiscard]] std::shared_ptr<Timedelta> utc_offset() const;

	// Return the timezone name.

	// Note that the name is 100% informational -- there's no requirement that
	// it mean anything in particular. For example, "GMT", "UTC", "-500",
	// "-5:00", "EDT", "US/Eastern", "America/New York" are all valid replies.
	[[nodiscard]] std::string tz_name() const;

	// Return 0 if DST is not in effect, or the DST offset (as timedelta
	// positive eastward) if DST is in effect.
	// This is purely informational; the DST offset has already been added to
	// the UTC offset returned by utcoffset() if applicable, so there's no
	// need to consult dst() unless you're interested in displaying the DST
	// info.
	[[nodiscard]] std::shared_ptr<Timedelta> dst() const;

	// Comparisons of datetime objects with other.
	bool operator == (const Datetime& other) const;
	bool operator != (const Datetime& other) const;
	bool operator <= (const Datetime& other) const;
	bool operator < (const Datetime& other) const;
	bool operator >= (const Datetime& other) const;
	bool operator > (const Datetime& other) const;

	// Add a Datetime and a Timedelta.
	Datetime operator + (const Timedelta& other) const;

	Datetime& operator = (const Datetime& other);
	Datetime& operator += (const Timedelta& other);
	Datetime& operator -= (const Timedelta& other);

	// Subtract two Datetimes.
	Timedelta operator - (const Datetime& other) const;

	// Subtract a datetime and a timedelta.
	Datetime operator - (const Timedelta& other) const;
};

class Timezone
{
private:
	std::shared_ptr<Timedelta> _offset;
	std::string _name;

	// Sentinel value to disallow empty name.
	static const std::string _Omitted;

	static const Timedelta _max_offset;
	static const Timedelta _min_offset;

private:
	Timezone() = default;

public:
	const static Timezone UTC;

	// bpo-37642: These attributes are rounded to the nearest minute for backwards
	// compatibility, even though the constructor will accept a wider range of
	// values. This may change in the future.
	static const Timezone MIN;
	static const Timezone MAX;

public:
	explicit Timezone(
		const Timedelta& offset,
		const std::string& name = _Omitted
	);

	static Timezone _create(
		const Timedelta& offset,
		const std::string& name = ""
	);

	// Required for copying inherited timezone objects or
	// converting them to std::shared_ptr<Timezone> with derived functionality.
	//
	// Sample:
	//  [[nodiscard]] std::shared_ptr<Timezone> ptr_copy() const override
	//  {
	//      auto ptr = std::make_shared<TimezoneDerived>(*this);
	//      return {ptr, static_cast<Timezone*>(ptr.get())};
	//  }
	[[nodiscard]] virtual std::shared_ptr<Timezone> ptr_copy() const;

	Timezone& operator = (const Timezone& other);

	bool operator == (const Timezone& other) const;
	bool operator != (const Timezone& other) const;

	[[nodiscard]] std::string str() const;

	// datetime -> timedelta, positive for east of UTC, negative for west of UTC
	virtual std::shared_ptr<Timedelta> utc_offset(const Datetime* dt) const;

	// datetime -> string name of time zone.
	virtual std::string tz_name(const Datetime* dt) const;

	// datetime -> DST offset as timedelta, positive for east of UTC.
	//
	// Return 0 if DST not in effect.  utc_offset() must include the DST
	// offset.
	virtual std::shared_ptr<Timedelta> dst(const Datetime* dt) const;

	/*
	// datetime in UTC -> datetime in local time.
	[[nodiscard]] virtual Datetime from_utc(Datetime dt) const;
	*/

	virtual Datetime from_utc(const Datetime* dt) const;

	static std::string _name_from_offset(const Timedelta* delta) ;
};

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

extern void _replace(
	std::string& src, const std::string& old, const std::string& new_
);

extern std::string _wrap_strftime(
	const std::string& format, const tm_tuple& time_tuple,
	const std::function<long long int()>& microsecond = nullptr,
	const std::function<std::shared_ptr<Timedelta>()>& utc_offset = nullptr,
	const std::function<std::string()>& tz_name = nullptr
);

// Helper to calculate the day number of the Monday starting week 1
extern size_t _iso_week1monday(ushort year);

struct hmsfz : public hmsf
{
	std::shared_ptr<Timezone> tz;
};

extern hmsfz _parse_isoformat_time(const std::string& t_str);

extern Datetime _strptime_datetime(
	const std::string& datetime_str,
	const char* format = "%a %b %d %H:%M:%S %Y"
);

const static Datetime _EPOCH = Datetime(
	1970, 1, 1, 0, 0, 0, 0, std::make_shared<Timezone>(Timezone::_create(Timedelta(0)))
);

__DATETIME_INTERNAL_END__
