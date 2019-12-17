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
 * pattern.h
 * Purpose: used for mapping url pattern to it's handler.
 *
 * Pattern example: /profile/<user_id>(\d+)/?
 */

#ifndef WASP_URLS_PATTERN_H
#define WASP_URLS_PATTERN_H

// C++ libraries.
#include <string>
#include <vector>
#include <regex>

// Module definitions.
#include "_def_.h"

// Wasp libraries.
#include "../views/args.h"
#include "../views/view.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../utility/str.h"
#include "../utility/logger.h"


__URLS_BEGIN__

class UrlPattern final
{
private:
	enum state_enum
	{
		s_url,
		s_arg_name,
		s_regex
	};

	std::string _s;
	std::regex _rgx;
	std::vector<std::string> _pattern_parts;
	wasp::views::ViewHandler _handler;
	std::string _name;
	std::vector<std::string> _keys;

public:
	UrlPattern(const std::string& rgx, const wasp::views::ViewHandler& handler, const std::string& name);

	std::string name();
	wasp::http::HttpResponse* apply(
		wasp::http::HttpRequest* request, wasp::views::Args* args = nullptr, ILogger* logger = nullptr
	);
	bool match(const std::string& url, std::map<std::string, std::string>& args);
	std::string build(const std::vector<std::string>& args);

private:
	std::string _parse(const std::string& pattern);
};

__URLS_END__


#endif // WASP_URLS_PATTERN_H
