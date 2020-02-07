/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "../src/apps/wasp.h"

#include "./example_app/settings.h"

#define DETECT_MEMORY_LEAK
#include "../tests/mem_leak_check.h"


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
