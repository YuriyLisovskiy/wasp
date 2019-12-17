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
 * An implementation of pattern.h.
 */

#include "./pattern.h"


__URLS_BEGIN__

UrlPattern::UrlPattern(const std::string& rgx, const views::ViewHandler& handler, const std::string& name)
{
	this->_s = this->_parse(rgx);
	this->_rgx = std::regex(this->_s);
	this->_handler = handler;
	this->_name = name;
}

std::string UrlPattern::name()
{
	return this->_name;
}

http::HttpResponse* UrlPattern::apply(
	http::HttpRequest* request,
	views::Args* args,
	utility::ILogger* logger
)
{
	return this->_handler(request, args, logger);
}

bool UrlPattern::match(const std::string& url, std::map<std::string, std::string>& args)
{
	std::smatch matches;
	bool matched = std::regex_search(url, matches, this->_rgx);
	if (matched)
	{
		for (size_t i = 1; i < matches.size(); i++)
		{
			if (matches[i].matched)
			{
				args[this->_keys[i - 1]] = matches[i].str();
			}
		}
	}

	return matched;
}

std::string UrlPattern::build(const std::vector<std::string>& args)
{
	if (this->_pattern_parts.empty())
	{
		return "";
	}

	size_t a_len = args.size();
	size_t p_len = this->_pattern_parts.size();
	if (a_len == p_len || a_len - 1 == p_len)
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

	throw core::AttributeError("unable to build url from pattern", _ERROR_DETAILS_);
}

std::string UrlPattern::_parse(const std::string& pattern)
{
	std::string new_pattern, part, arg_name;
	UrlPattern::state_enum st = UrlPattern::state_enum::s_url;
	for (char ch : pattern)
	{
		switch (st)
		{
			case UrlPattern::state_enum::s_url:
				if (ch == '<')
				{
					st = UrlPattern::state_enum::s_arg_name;
					this->_pattern_parts.push_back(part);
					part.clear();
					continue;
				}
				else if (ch == '(')
				{
					st = UrlPattern::state_enum::s_regex;
				}
				else if (ch == '/')
				{
					new_pattern += "\\";
					part += ch;
				}
				else
				{
					part += ch;
				}

				new_pattern += ch;
				break;
			case UrlPattern::state_enum::s_arg_name:
				if (ch == '>')
				{
					st = UrlPattern::state_enum::s_url;
					this->_keys.push_back(arg_name);
					arg_name.clear();
				}
				else
				{
					arg_name += ch;
				}
				break;
			case UrlPattern::state_enum::s_regex:
				if (ch == ')')
				{
					st = UrlPattern::state_enum::s_url;
				}

				new_pattern += ch;
				break;
		}
	}

	if (!part.empty())
	{
		if (str::ends_with(part, "?"))
		{
			part.pop_back();
		}

		str::rtrim(part, '/');
		this->_pattern_parts.push_back(part);
	}

	return new_pattern;
}

__URLS_END__
