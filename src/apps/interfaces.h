/**
 * apps/interface.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: apps interfaces.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../urls/pattern.h"
#include "../commands/base.h"


__APPS_BEGIN__

class IAppConfig
{
public:
	virtual ~IAppConfig() = default;

	virtual bool is_initialized() const = 0;
	virtual std::string get_name() = 0;
	virtual std::string get_app_path() = 0;
	virtual std::vector<std::shared_ptr<urls::UrlPattern>> get_urlpatterns() = 0;
	virtual std::vector<std::shared_ptr<cmd::BaseCommand>> get_commands() = 0;
};

__APPS_END__
