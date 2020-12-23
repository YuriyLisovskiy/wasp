/**
 * commands/command.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: base class for all commands.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base.h"
#include "../conf/settings.h"


__COMMANDS_BEGIN__

class Command : public BaseCommand
{
protected:
	conf::Settings* settings;

	Command(
		conf::Settings* settings, const std::string& cmd_name, const std::string& help
	);
};

__COMMANDS_END__
