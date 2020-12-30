/**
 * apps/xalwart.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: an implementation of main application.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/settings.h"


__APPS_BEGIN__

class MainApplication final
{
private:
	conf::Settings* _settings;
	std::string _help_message;

	// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::BaseCommand>> _commands;

	void _setup_commands();
	void _extend_settings_commands_or_error(
		const std::vector<std::shared_ptr<cmd::BaseCommand>>& from,
		const std::function<std::string(const std::string& cmd_name)>& err_fn
	);
	void _perform_checks();

public:
	explicit MainApplication(conf::Settings* settings);
	void execute(int argc, char** argv);
};

__APPS_END__
