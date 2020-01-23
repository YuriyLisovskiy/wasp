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
 * An implementation of http.h.
 */

#include "./http.h"


__UTILS_HTTP_INTERNAL_BEGIN__

core::rgx::Regex ETAG_REGEX = core::rgx::Regex(R"(\A((?:W\/)?"[^"]*")\Z)");

__UTILS_HTTP_INTERNAL_END__


__UTILS_HTTP_BEGIN__

std::string quote_etag(const std::string& e_tag)
{
	if (internal::ETAG_REGEX.match(e_tag))
	{
		return e_tag;
	}

	return "\"" + e_tag + "\"";
}

__UTILS_HTTP_END__
