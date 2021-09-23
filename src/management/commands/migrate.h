/**
 * management/commands/migrate.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Command to migrate changes to the database.
 */

#pragma once

// Base libraries.
#include <xalwart.base/abc/base.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../commands/command.h"
#include "../../commands/flags/default.h"


__MANAGEMENT_COMMANDS_BEGIN__

// TESTME: MigrateCommand
// TODO: docs for 'MigrateCommand'
class MigrateCommand final : public xw::cmd::Command
{
public:
	inline explicit MigrateCommand(conf::IModuleConfig* config, conf::Settings* settings) :
		Command(config, settings, "migrate", "Migrates changes to the database")
	{
	}

protected:
	inline void log_progress(const std::string& msg, const std::string& end) const
	{
		auto end_ch = end.empty() ? '\0' : end[0];
		this->settings->LOGGER->print(msg, abc::Logger::Color::Default, end_ch);
	}

	void add_flags() final;

	void handle() final;

private:
	std::shared_ptr<xw::cmd::flags::StringFlag> _db_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::StringFlag> _migration_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _rollback_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _no_colors_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _print_help_flag = nullptr;
};

__MANAGEMENT_COMMANDS_END__
