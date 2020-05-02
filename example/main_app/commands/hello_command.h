/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#pragma once

#include "../../../src/apps/interfaces.h"
#include "../../../src/core/management/app_command.h"


class HelloCommand : public wasp::core::AppCommand
{
public:
	HelloCommand(wasp::apps::IAppConfig* config, wasp::conf::Settings* settings)
		: AppCommand(config, settings, "hello", "Prints hello from MainApp")
	{
	}

	void handle() override
	{
		this->settings->LOGGER->print("Hello from MainApp!");
	}
};
