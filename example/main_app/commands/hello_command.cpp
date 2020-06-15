/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./hello_command.h"


HelloCommand::HelloCommand(xw::apps::IAppConfig* config, xw::conf::Settings* settings)
	: AppCommand(config, settings, "hello", "Prints greetings")
{
}

void HelloCommand::handle()
{
	this->settings->LOGGER->print("Hello from MainApp!");
}
