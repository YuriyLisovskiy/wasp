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

/*
 * http response definition.
 * TODO: write docs
 */

#ifndef LEAF_HTTP_RESPONSE_H
#define LEAF_HTTP_RESPONSE_H

#include <string>

#include "../globals.h"
#include "request.h"


__INTERNAL_BEGIN__

class HttpResponse
{
public:
	explicit HttpResponse(const HttpRequest& request);
	std::string toString();
};

__INTERNAL_END__


#endif // LEAF_HTTP_RESPONSE_H
