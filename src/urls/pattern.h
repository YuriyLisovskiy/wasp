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
#include "../conf/settings.h"
#include "../controllers/controller.h"


__URLS_BEGIN__

// TESTME: Pattern<...ArgsT>
// TODO: docs for 'Pattern<...ArgsT>'
template <typename ...ArgsT>
class Pattern final : public IPattern
{
public:
	inline Pattern(const std::string& rgx, ctrl::Handler<ArgsT...> handler, std::string name) :
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
		return this->_original_expression;
	}

	inline void add_prefix(const std::string& prefix) override
	{
		this->_regex = re::ArgRegex(prefix + this->_original_expression);
		this->_reload_pattern_parts();
	}

	inline void add_namespace(const std::string& ns) override
	{
		this->_name = ns + "::" + this->_name;
	}

	inline std::unique_ptr<http::abc::HttpResponse> apply(http::Request* request, conf::Settings* settings) override
	{
		return this->_handler(request, this->_regex.template args_tuple<ArgsT...>(), settings);
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

		throw ArgumentError(
			"unable to build url: arguments do not match pattern '" + this->_original_expression + "'",
			_ERROR_DETAILS_
		);
	}

private:
	std::string _original_expression;
	std::vector<std::string> _pattern_parts;
	ctrl::Handler<ArgsT...> _handler;
	std::string _name;
	re::ArgRegex _regex;

	inline void _reload_pattern_parts()
	{
		this->_original_expression = this->_regex.to_string();
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
};

__URLS_END__
