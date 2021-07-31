/**
 * urls/pattern.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Url pattern mapping to it's handler.
 *
 * Example: /profile/<user_id>(\d+)/?
 */

#pragma once

// Base libraries.
#include <xalwart.base/kwargs.h>
#include <xalwart.base/re/arg_regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"
#include "../conf/_def_.h"


__CONF_BEGIN__

struct Settings;

__CONF_END__


__URLS_BEGIN__

typedef std::function<Result<std::shared_ptr<http::IHttpResponse>>(
	http::HttpRequest*, Kwargs*, conf::Settings*
)> ControllerHandler;

// TESTME: UrlPattern
// TODO: docs for 'UrlPattern'
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
	UrlPattern(const std::string& rgx, ControllerHandler handler, std::string name);

	UrlPattern(
		const std::string& prefix,
		const std::shared_ptr<UrlPattern>& url_pattern,
		const std::string& namespace_
	);

	[[nodiscard]]
	inline std::string get_name() const
	{
		return this->_name;
	}

	inline Result<std::shared_ptr<http::IHttpResponse>> apply(
		http::HttpRequest* request, conf::Settings* settings, Kwargs* kwargs=nullptr
	)
	{
		return this->_handler(request, kwargs, settings);
	}

	bool match(const std::string& url, std::map<std::string, std::string>& args);

	[[nodiscard]]
	std::string build(const std::vector<std::string>& args) const;
};

// TESTME: make_static
// TODO: docs for 'make_static'
extern std::shared_ptr<urls::UrlPattern> make_static(
	const std::string& static_url,
	const std::string& static_root,
	const std::string& name=""
);

__URLS_END__
