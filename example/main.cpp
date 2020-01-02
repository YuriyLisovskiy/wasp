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

#include "../src/apps/wasp.h"
#include "../src/core/exceptions.h"

#include "./example_app/settings.h"


int main(int argc, char** argv)
{
	auto* settings = new Settings();
	try
	{
		auto app = wasp::apps::WaspApplication(settings);
		app.execute_from_command_line(argc, argv);
	}
	catch (const wasp::core::ImproperlyConfigured& exc)
	{
		if (settings->LOGGER)
		{
			settings->LOGGER->error(exc);
		}
	}

	delete settings;
	return 0;
}
