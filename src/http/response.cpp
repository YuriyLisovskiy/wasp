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
 * http response implementation.
 * TODO: write docs
 */

#include "response.h"


__WASP_BEGIN__

HttpResponse::HttpResponse(const HttpRequest& request)
{
	// TODO:
}

// TODO:
std::string HttpResponse::toString()
{
	std::string body("<form action=\"/hello\" method=\"post\" enctype=\"text/plain\">\n"
					 "\t<input type=\"file\" name=\"super_file\" />\n"
					 "\t<input type=\"password\" name=\"first_name\" />\n"
					 "\t<input type=\"submit\" value=\"send\" />\n"
					 "\t</form>");

	return std::string("HTTP/1.1 200 OK\n"
				  "Date: Sun, 10 Oct 2010 23:26:07 GMT\n"
				  "Last-Modified: Sun, 26 Sep 2010 22:04:35 GMT\n"
				  "ETag: \"45b6-834-49130cc1182c0\"\n"
				  "Accept-Ranges: bytes\n"
				  "Content-Length: " + std::to_string(body.size()) + "\n"
					"Connection: close\n"
					"Content-Type: text/html\n"
					"Server: Apache/2.2.8 (Ubuntu) mod_ssl/2.2.8 OpenSSL/0.9.8g\n\n" +
				  body);
}

__WASP_END__
