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
 * An implementation of regex.h.
 */

#include "./regex.h"


__RGX_BEGIN__

Regex::Regex(const std::string& expr)
{
	this->_expr = std::regex(expr);
	this->_is_matched = false;
	this->_is_searched = false;
	this->_groups_are_made = false;
}

bool Regex::match(const std::string& to_match)
{
	this->_to_match = to_match;
	this->_is_matched = std::regex_match(this->_to_match, this->_expr);
	this->_is_searched = false;
	this->_groups_are_made = false;
	return this->_is_matched;
}

bool Regex::search(const std::string& to_search)
{
	this->_to_match = to_search;
	this->_is_searched = std::regex_search(this->_to_match, this->_matches, this->_expr);
	this->_groups_are_made = false;
	return this->_is_searched;
}

void Regex::_make_groups()
{
	this->_groups.clear();
	for (size_t i = 1; i < this->_matches.size(); i++)
	{
		if (this->_matches[i].matched)
		{
			this->_groups.push_back(this->_matches[i].str());
		}
	}
}

std::vector<std::string> Regex::groups()
{
	if (!this->_groups_are_made)
	{
		if (this->_is_searched)
		{
			this->_make_groups();
		}
		else if (this->_is_matched)
		{
			if (std::regex_search(this->_to_match, this->_matches, this->_expr))
			{
				this->_make_groups();
			}
		}
	}

	return this->_groups;
}

std::string Regex::group(size_t pos)
{
	this->groups();
	if (pos >= 0 && pos < this->_groups.size())
	{
		return this->_groups[pos];
	}

	return "";
}


ArgRegex::ArgRegex(const std::string& rgx)
{
	this->_orig = rgx;
	this->_s = this->_parse(this->_orig);
	this->_rgx = std::regex(this->_s);
	this->_is_matched = false;
	this->_is_searched = false;
	this->_groups_are_made = false;
}

bool ArgRegex::match(const std::string& to_match)
{
	this->_to_match = to_match;
	this->_is_matched = std::regex_match(this->_to_match, this->_rgx);
	this->_groups_are_made = false;
	this->_is_searched = false;
	return this->_is_matched;
}

bool ArgRegex::search(const std::string& to_search)
{
	this->_to_match = to_search;
	this->_is_searched = std::regex_search(this->_to_match, this->_matches, this->_rgx);
	this->_groups_are_made = false;
	return this->_is_searched;
}

std::map<std::string, std::string> ArgRegex::groups()
{
	if (!this->_groups_are_made)
	{
		if (this->_is_searched)
		{
			this->_make_groups();
		}
		else if (this->_is_matched)
		{
			if (this->search(this->_to_match))
			{
				this->_make_groups();
			}
		}
	}

	return this->_groups;
}

std::string ArgRegex::group(const std::string& key, const std::string& default_val)
{
	this->groups();
	if (this->_groups_are_made)
	{
		return this->_groups[key];
	}

	return default_val;
}

std::vector<std::string> ArgRegex::parts()
{
	return this->_pattern_parts;
}

std::string ArgRegex::original()
{
	return this->_orig;
}

void ArgRegex::_make_groups()
{
	this->_groups.clear();
	for (size_t i = 1; i < this->_matches.size(); i++)
	{
		if (this->_matches[i].matched)
		{
			this->_groups[this->_keys[i - 1]] = this->_matches[i].str();
		}
	}

	this->_groups_are_made = true;
}

std::string ArgRegex::_parse(const std::string& pattern)
{
	std::string new_pattern, part, arg_name;
	ArgRegex::state_enum st = ArgRegex::state_enum::s_str;
	for (char ch : pattern)
	{
		switch (st)
		{
			case ArgRegex::state_enum::s_str:
				if (ch == '<')
				{
					st = ArgRegex::state_enum::s_check_if_arg;
					continue;
				}
				else if (ch == '(')
				{
					st = ArgRegex::state_enum::s_regex;
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
			case ArgRegex::state_enum::s_check_if_arg:
				if (ch == '<')
				{
					part += "<<";
					new_pattern += "<<";
					st = ArgRegex::state_enum::s_str;
				}
				else
				{
					st = ArgRegex::state_enum::s_arg_name;
					this->_pattern_parts.push_back(part);
					part.clear();
					arg_name += ch;
				}
				break;
			case ArgRegex::state_enum::s_arg_name:
				if (ch == '>')
				{
					st = ArgRegex::state_enum::s_str;
					this->_keys.push_back(arg_name);
					arg_name.clear();
				}
				else
				{
					arg_name += ch;
				}
				break;
			case ArgRegex::state_enum::s_regex:
				if (ch == ')')
				{
					st = ArgRegex::state_enum::s_str;
				}

				new_pattern += ch;
				break;
		}
	}

	if (!part.empty())
	{
		this->_pattern_parts.push_back(part);
	}

	return new_pattern;
}

__RGX_END__
