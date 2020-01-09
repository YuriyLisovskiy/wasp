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
}

bool Regex::match(const std::string& to_match)
{
	this->_to_match = to_match;
	this->_is_matched = std::regex_match(this->_to_match, this->_expr);
	return this->_is_matched;
}

std::vector<std::string> Regex::groups()
{
	std::vector<std::string> groups;
	if (this->_is_matched)
	{
		std::smatch matches;
		if (std::regex_search(this->_to_match, matches, this->_expr))
		{
			for (size_t i = 1; i < matches.size(); i++)
			{
				if (matches[i].matched)
				{
					groups.push_back(matches[i].str());
				}
			}
		}
	}

	return groups;
}

__RGX_END__
