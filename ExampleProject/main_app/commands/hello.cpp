/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./hello.h"


HelloCommand::HelloCommand(xw::apps::IAppConfig* config, xw::conf::Settings* settings)
	: AppCommand(config, settings, "Hello", "Prints greetings")
{
}

void HelloCommand::handle()
{
	this->settings->LOGGER->print("Hello from MainApp!");
}
