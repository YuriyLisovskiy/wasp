/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include <xalwart/apps/xalwart.h>

#include "./example_app/settings.h"


int main(int argc, char** argv)
{
	auto settings = std::make_shared<Settings>();
	try
	{
		auto app = xw::apps::MainApplication(settings.get());
		app.execute(argc, argv);
	}
	catch (const xw::core::ImproperlyConfigured& exc)
	{
		if (settings->LOGGER)
		{
			settings->LOGGER->error(exc);
		}
	}

	return 0;
}
