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

/*
 * encoding implementation.
 * TODO: write docs.
 */

#include "encoding.h"


__WASP_BEGIN__

std::string encodeUrl(const std::string& url)
{
	enum UrlState
	{
		Scheme,
		SlashAfterScheme,
		Host,
		Path,
		Query,
		Fragment
	} state = UrlState::Scheme;

	std::ostringstream oss;
	oss.fill('0');
	oss << std::hex;

	for (const char& c : url)
	{
		switch (state)
		{
			case UrlState::Scheme:
				if (c == '/')
				{
					state = UrlState::SlashAfterScheme;
				}
				oss << c;
				break;
			case UrlState::SlashAfterScheme:
				if (c == '/')
				{
					state = UrlState::Host;
				}
				oss << c;
				break;
			case UrlState::Host:
				if (c == '/')
				{
					state = UrlState::Path;
				}
				oss << c;
				break;
			case UrlState::Path:
				if (c == '?')
				{
					oss << c;
					state = UrlState::Query;
				}
				else
				{
					internal::escape(oss, c, "/");
				}
				break;
			case UrlState::Query:
				if (c == '#')
				{
					oss << c;
					state = UrlState::Fragment;
				}
				else
				{
					internal::escape(oss, c, "&=");
				}
				break;
			case UrlState::Fragment:
				internal::escape(oss, c);
				break;
		}
	}

	return oss.str();
}

__WASP_END__


__INTERNAL_BEGIN__

void escape(std::ostringstream& stream, char c, const std::string& safe)
{
	if (isalnum((unsigned char) c) || c == '-' || c == '_' || c == '.' || c == '~' || safe.find(c) != -1)
	{
		stream << c;
	}
	else
	{
		stream << std::uppercase;
		stream << '%' << std::setw(2) << int((unsigned char) c);
		stream << std::nouppercase;
	}
}

__INTERNAL_END__
