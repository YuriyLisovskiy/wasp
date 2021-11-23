/**
 * commands/command.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Base class for modules' commands.
 */

#pragma once

// STL libraries.
#include <memory>
#include <string>

// Module definitions.
#include "./_def_.h"

// Base libraries.
#include <xalwart.base/interfaces/base.h>

// Framework libraries.
#include "./abstract_command.h"
#include "./flags/default.h"


__COMMANDS_BEGIN__

// TESTME: Command
// TODO: docs for 'Command'
class Command : public AbstractCommand
{
protected:
	std::shared_ptr<ILogger> logger;

	Command(const std::string& cmd_name, const std::string& help, std::shared_ptr<ILogger> logger) :
		AbstractCommand(cmd_name, help)
	{
		this->logger = std::move(logger);
		require_non_null(this->logger.get(), "logger is nullptr", _ERROR_DETAILS_);
	}

	void add_flags() override;

	void validate() const override;

	bool handle() override;

private:
	std::shared_ptr<flags::BoolFlag> _print_help_flag = nullptr;
	std::shared_ptr<flags::BoolFlag> _use_colors_flag = nullptr;

	inline void _use_colors_for_logging(bool use_colors)
	{
		if (use_colors)
		{
			this->logger->enable_colors();
		}
		else
		{
			this->logger->disable_colors();
		}
	}
};

__COMMANDS_END__
