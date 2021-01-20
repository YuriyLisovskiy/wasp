/**
 * apps/abs.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: abstract base classes for apps module.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../urls/pattern.h"
#include "../commands/base.h"


__APPS_BEGIN__

class IModuleConfig
{
public:
	virtual ~IModuleConfig() = default;

	virtual bool ready() const = 0;
	virtual std::string get_name() const = 0;
	virtual std::string get_module_path() const = 0;
	virtual std::vector<std::shared_ptr<urls::UrlPattern>> get_urlpatterns() = 0;
	virtual std::vector<std::shared_ptr<cmd::BaseCommand>> get_commands() = 0;
};

template <typename T>
concept ModuleConfigType = std::is_base_of_v<IModuleConfig, T>;

const rgx::Regex _R_CONFIG_NAME = rgx::Regex(
	R"((((M|m)odule)|((C|c)onfig)|((M|m)odule_*(C|c)onfig))$)"
);

__APPS_END__
