/**
 * commands/command.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./command.h"


__COMMANDS_BEGIN__

Command::Command(
	conf::Settings* settings, const std::string& cmd_name, const std::string& help
) : BaseCommand(cmd_name, help)
{
	this->settings = settings;
}

__COMMANDS_END__
