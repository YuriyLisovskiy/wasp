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

/**
 * An implementation of utility.h.
 */

#include "./utility.h"


__HTTP_BEGIN__

size_t parse_http_datetime(const std::string& http_datetime)
{
	return dt::DateTime::strptime(http_datetime.c_str(), "Wdy, DD Mon YYYY HH:MM:SS GMT").utc_epoch();
}

std::string format_http_datetime(size_t epoch_seconds)
{
	return dt::DateTime(epoch_seconds).strftime("Wdy, DD Mon YYYY HH:MM:SS GMT");
}

__HTTP_END__
