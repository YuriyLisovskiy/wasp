/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "../src/apps/wasp.h"

#include "./example_app/settings.h"

//#include "../tests/mem_leak_check.h"


int main(int argc, char** argv)
{
	auto settings = std::make_shared<Settings>();
	try
	{
		auto app = wasp::apps::WaspApplication(settings.get());
		app.execute(argc, argv);
	}
	catch (const wasp::core::ImproperlyConfigured& exc)
	{
		if (settings->LOGGER)
		{
			settings->LOGGER->error(exc);
		}
	}

	return 0;
}
