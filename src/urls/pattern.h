/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * urls/pattern.h
 *
 * Purpose: used for mapping url pattern to it's handler.
 *
 * Pattern example: /profile/<user_id>(\d+)/?
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include <xalwart.core/regex.h>
#include "../views/args.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/settings.h"


__CONF_BEGIN__
struct Settings;
__CONF_END__


__URLS_BEGIN__

typedef std::function<std::unique_ptr<http::IHttpResponse>(
	http::HttpRequest*, views::Args*, conf::Settings*
)> ViewHandler;

class UrlPattern final
{
private:
	std::string _orig;
	std::vector<std::string> _pattern_parts;
	ViewHandler _handler;
	std::string _name;
	core::rgx::ArgRegex _regex;
	std::string _namespace;

public:
	UrlPattern(
		const std::string& rgx,
		const ViewHandler& handler,
		const std::string& name
	);
	UrlPattern(
		const std::string& prefix,
		const std::shared_ptr<UrlPattern>& url_pattern,
		const std::string& namespace_
	);

	[[nodiscard]] std::string get_name() const;
	std::unique_ptr<http::IHttpResponse> apply(
		http::HttpRequest* request,
		conf::Settings* settings,
		views::Args* args = nullptr
	);
	bool match(const std::string& url, std::map<std::string, std::string>& args);
	[[nodiscard]] std::string build(const std::vector<std::string>& args) const;
};

__URLS_END__
