/**
 * management/commands/migrate.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Command to migrate changes to the database.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../commands/command.h"


__MANAGEMENT_COMMANDS_BEGIN__

// TESTME: MigrateCommand
// TODO: docs for 'MigrateCommand'
class MigrateCommand final : public xw::cmd::Command
{
private:
	std::shared_ptr<core::flags::StringFlag> _db_flag;
	std::shared_ptr<core::flags::StringFlag> _migration_flag;
	std::shared_ptr<core::flags::BoolFlag> _rollback_flag;
	std::shared_ptr<core::flags::BoolFlag> _no_colors_flag;
	std::shared_ptr<core::flags::BoolFlag> _print_help_flag;

protected:
	inline void log_progress(const std::string& msg, const std::string& end) const
	{
		auto end_ch = end.empty() ? '\0' : end[0];
		this->settings->LOGGER->print(msg, log::ILogger::Color::Default, end_ch);
	}

	void add_flags() final;

	void handle() final;

public:
	inline explicit MigrateCommand(conf::IModuleConfig* config, conf::Settings* settings) :
		Command(config, settings, "migrate", "Migrates changes to the database")
	{
	}
};

__MANAGEMENT_COMMANDS_END__
