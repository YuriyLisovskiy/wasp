/**
 * urls/pattern.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: used for mapping url pattern to it's handler.
 *
 * 	Pattern example: /profile/<user_id>(\d+)/?
 */

#pragma once

// Base libraries.
#include <xalwart.base/re/arg_regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../core/kwargs.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/_def_.h"


__CONF_BEGIN__

struct Settings;

__CONF_END__


__URLS_BEGIN__

typedef std::function<Result<std::shared_ptr<http::IHttpResponse>>(
	http::HttpRequest*, core::Kwargs*, conf::Settings*
)> ControllerHandler;

class UrlPattern final
{
private:
	std::string _orig;
	std::vector<std::string> _pattern_parts;
	ControllerHandler _handler;
	std::string _name;
	re::ArgRegex _regex;
	std::string _namespace;

public:
	UrlPattern(
		const std::string& rgx,
		const ControllerHandler& handler,
		const std::string& name
	);
	UrlPattern(
		const std::string& prefix,
		const std::shared_ptr<UrlPattern>& url_pattern,
		const std::string& namespace_
	);

	[[nodiscard]] std::string get_name() const;
	Result<std::shared_ptr<http::IHttpResponse>> apply(
		http::HttpRequest* request,
		conf::Settings* settings,
		core::Kwargs* kwargs=nullptr
	);
	bool match(const std::string& url, std::map<std::string, std::string>& args);
	[[nodiscard]] std::string build(const std::vector<std::string>& args) const;
};

__URLS_END__
