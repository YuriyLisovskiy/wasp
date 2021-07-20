/**
 * urls/pattern.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./pattern.h"

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/string_utils.h>


__URLS_BEGIN__

UrlPattern::UrlPattern(
	const std::string& rgx,
	const ViewHandler& handler,
	const std::string& name
) : _regex(rgx)
{
	this->_orig = this->_regex.str();
	this->_pattern_parts = this->_regex.parts();
	if (!this->_pattern_parts.empty())
	{
		if (this->_pattern_parts.back().ends_with("?"))
		{
			this->_pattern_parts.back().pop_back();
		}

		this->_pattern_parts.back() = str::rtrim(this->_pattern_parts.back(), "/");
	}

	this->_handler = handler;
	this->_name = name;
}

UrlPattern::UrlPattern(
	const std::string& prefix,
	const std::shared_ptr<UrlPattern>& url_pattern,
	const std::string& namespace_
) : UrlPattern(
	prefix + url_pattern->_orig,
	url_pattern->_handler,
	namespace_ + "::" + url_pattern->_name
)
{
}

std::string UrlPattern::get_name() const
{
	return this->_name;
}

Result<std::shared_ptr<http::IHttpResponse>> UrlPattern::apply(
	http::HttpRequest* request,
	conf::Settings* settings,
	views::Args* args
)
{
	return this->_handler(request, args, settings);
}

bool UrlPattern::match(const std::string& url, std::map<std::string, std::string>& args)
{
	if (this->_regex.match(url) && this->_regex.search(url))
	{
		args = this->_regex.args();
		return true;
	}

	return false;
}

std::string UrlPattern::build(const std::vector<std::string>& args) const
{
	if (this->_pattern_parts.empty())
	{
		return "";
	}

	size_t a_len = args.size();
	size_t p_len = this->_pattern_parts.size();
	if (a_len == p_len || p_len - 1 == a_len)
	{
		size_t i = 0;
		std::string built_url;
		for (const auto& arg : args)
		{
			built_url += this->_pattern_parts[i++] + arg;
		}

		if (i < this->_pattern_parts.size())
		{
			built_url += this->_pattern_parts[i];
		}

		return built_url;
	}

	throw AttributeError(
		"Unable to build url: arguments do not match pattern '" + this->_orig + "'",
		_ERROR_DETAILS_
	);
}

__URLS_END__
