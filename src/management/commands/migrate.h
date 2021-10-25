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
#include "../../conf/abc.h"
#include "../../conf/settings.h"
#include "../../commands/command.h"
#include "../../commands/flags/default.h"


__MANAGEMENT_COMMANDS_BEGIN__

// TESTME: MigrateCommand
// TODO: docs for 'MigrateCommand'
class MigrateCommand final : public xw::cmd::Command
{
public:
	inline explicit MigrateCommand(conf::Settings* settings) :
		Command(
			"migrate", "Migrates changes to the database",
			require_non_null(settings, "settings is nullptr", _ERROR_DETAILS_)->LOGGER
		)
	{
		this->_settings = settings;
	}

protected:
	void add_flags() final;

	bool handle() final;

private:
	std::shared_ptr<xw::cmd::flags::StringFlag> _db_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::StringFlag> _migration_flag = nullptr;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _rollback_flag = nullptr;

	conf::Settings* _settings = nullptr;

	inline void _log_progress(const std::string& msg, const std::string& end) const
	{
		auto end_ch = end.empty() ? '\0' : end[0];
		this->_settings->LOGGER->print(msg, abc::ILogger::Color::Default, end_ch);
	}
};

__MANAGEMENT_COMMANDS_END__
