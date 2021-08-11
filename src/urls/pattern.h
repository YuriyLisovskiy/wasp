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
#include <xalwart.base/re/arg_regex.h>
#include <xalwart.base/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../conf/_def_.h"


__CONF_BEGIN__

struct Settings;

__CONF_END__


__URLS_BEGIN__

template <typename ...ArgsT>
using ControllerHandler = std::function<http::Response::Result(
	http::Request*, const std::tuple<ArgsT...>&, conf::Settings*
)>;

// TESTME: Pattern<...ArgsT>
// TODO: docs for 'Pattern<...ArgsT>'
template <typename ...ArgsT>
class Pattern final : public IPattern
{
private:
	std::string _orig;
	std::vector<std::string> _pattern_parts;
	ControllerHandler<ArgsT...> _handler;
	std::string _name;
	re::ArgRegex _regex;

private:
	inline void _reload_pattern_parts()
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
	}

public:
	inline Pattern(const std::string& rgx, ControllerHandler<ArgsT...> handler, std::string name) :
		_regex(rgx), _handler(std::move(handler)), _name(std::move(name))
	{
		if (this->_name.empty())
		{
			throw ArgumentError("the name of pattern should not be empty", _ERROR_DETAILS_);
		}

		this->_reload_pattern_parts();
	}

	[[nodiscard]]
	inline std::string get_name() const override
	{
		return this->_name;
	}

	[[nodiscard]]
	inline std::string get_pattern_str() const override
	{
		return this->_orig;
	}

	inline void add_prefix(const std::string& prefix) override
	{
		this->_regex = re::ArgRegex(prefix + this->_orig);
		this->_reload_pattern_parts();
	}

	inline void add_namespace(const std::string& ns) override
	{
		this->_name = ns + "::" + this->_name;
	}

	inline http::Response::Result apply(http::Request* request, conf::Settings* settings) override
	{
		return this->_handler(request, this->_regex.template tuple<ArgsT...>(), settings);
	}

	inline bool match(const std::string& url) override
	{
		return this->_regex.match(url) && this->_regex.search(url);
	}

	[[nodiscard]]
	inline std::string build(const std::vector<std::string>& args) const override
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
			"unable to build url: arguments do not match pattern '" + this->_orig + "'",
			_ERROR_DETAILS_
		);
	}
};

__URLS_END__
