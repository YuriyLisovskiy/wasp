/**
 * commands/command.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Base class for modules' commands.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base.h"
#include "../conf/settings.h"


__COMMANDS_BEGIN__

// TESTME: Command
// TODO: docs for 'Command'
class Command : public BaseCommand
{
protected:
	conf::Settings* settings;
	conf::IModuleConfig* module_config;

protected:
	Command(
		conf::IModuleConfig* module,
		conf::Settings* settings,
		const std::string& cmd_name,
		const std::string& help
	);
};

template <typename T>
concept command_type = std::is_base_of_v<Command, T>;

__COMMANDS_END__
