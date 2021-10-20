/**
 * management/commands/migrate.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./migrate.h"

// Orm libraries.
#include <xalwart.orm/db/executor.h>


__MANAGEMENT_COMMANDS_BEGIN__

void MigrateCommand::add_flags()
{
	xw::cmd::Command::add_flags();
	this->_db_flag = this->flag_set->make_string(
		"d", "database", "default", "The database to migrate"
	);
	this->_migration_flag = this->flag_set->make_string(
		"m", "migration", "", "Database will be brought to the state after this migration"
	);
	this->_rollback_flag = this->flag_set->make_bool(
		"r", "rollback", false, "Identifies whether to rollback database migrations"
	);
}

bool MigrateCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	auto db_name = this->_db_flag->get();
	if (this->settings->DATABASES.find(db_name) == this->settings->DATABASES.end())
	{
		throw CommandError(
			"migrate: database with name '" + db_name + "' does not exist", _ERROR_DETAILS_
		);
	}

	auto backend = (orm::abc::ISQLBackend*)this->settings->DATABASES.at(db_name).get();
	require_non_null(backend, "Database backend is nullptr", _ERROR_DETAILS_);
	auto executor = orm::db::MigrationExecutor(
		backend, this->settings->build_migrations(backend),
		[this] (auto msg, auto end) { this->log_progress(msg, end); }
	);
	auto editor = backend->schema_editor();
	auto migration_name = this->_migration_flag->get();
	if (this->_rollback_flag->get())
	{
		executor.rollback(editor, migration_name);
	}
	else
	{
		executor.apply(editor, migration_name);
	}

	return true;
}

__MANAGEMENT_COMMANDS_END__
