/**
 * conf/abs.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Abstract base classes for 'conf' module.
 */

#pragma once

// Base libraries.
#include <xalwart.base/re/regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../urls/abc.h"
#include "../commands/base.h"


__CONF_BEGIN__

class IModuleConfig
{
public:
	virtual ~IModuleConfig() = default;

	[[nodiscard]]
	virtual bool ready() const = 0;

	[[nodiscard]]
	virtual std::string get_name() const = 0;

	[[nodiscard]]
	virtual std::string get_module_path() const = 0;

	virtual std::vector<std::shared_ptr<urls::IPattern>> get_urlpatterns() = 0;

	virtual std::vector<std::shared_ptr<cmd::BaseCommand>> get_commands() = 0;
};

template <typename T>
concept module_config_type = std::is_base_of_v<IModuleConfig, T>;

__CONF_END__
