/**
 * conf/interfaces.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Interfaces for 'conf' module.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <memory>

// Base libraries.
#include <xalwart.base/re/regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../urls/interfaces.h"
#include "../commands/abstract_command.h"


__CONF_BEGIN__

class IModuleConfig
{
public:
	virtual ~IModuleConfig() = default;

	[[nodiscard]]
	virtual bool ready() const = 0;

	[[nodiscard]]
	virtual std::string get_name() const = 0;

	virtual std::vector<std::shared_ptr<urls::IPattern>> get_urlpatterns() = 0;

	virtual std::vector<std::shared_ptr<cmd::AbstractCommand>> get_commands() = 0;
};

template <typename T>
concept module_config_type = std::is_base_of_v<IModuleConfig, T>;

__CONF_END__
