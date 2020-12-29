/**
 * apps/config.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Represents application configuration
 * 	with urls and models (will be developed in future)
 */

#pragma once

// Core libraries.
#include <xalwart.core/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../views/view.h"
#include "../urls/url.h"
#include "../commands/app_command.h"


__APPS_BEGIN__

/// Derived class must contain constructor with
/// pointer to conf::Settings parameter.
class AppConfig : public IAppConfig, public core::object::Object
{
private:
	bool _is_initialized;

	std::vector<std::shared_ptr<urls::UrlPattern>> _urlpatterns;
	std::vector<std::shared_ptr<cmd::BaseCommand>> _commands;

	template <typename AppConfigT>
	std::shared_ptr<apps::IAppConfig> find_or_create_app()
	{
		auto app = std::find_if(
			this->settings->INSTALLED_APPS.begin(),
			this->settings->INSTALLED_APPS.end(),
			[](const std::shared_ptr<apps::IAppConfig>& entry) -> bool {
				return dynamic_cast<AppConfigT*>(entry.get()) != nullptr;
			}
		);
		if (app == this->settings->INSTALLED_APPS.end())
		{
			auto created_app = std::make_shared<AppConfigT>(this->settings);
			created_app->init(created_app->__type__());
			this->settings->INSTALLED_APPS.push_back(created_app);
			return created_app;
		}

		return *app;
	}

protected:
	std::string app_name;
	std::string app_path;
	conf::Settings* settings;

	template <typename ViewT, typename = std::enable_if<std::is_base_of<views::View, ViewT>::value>>
	void url(const std::string& pattern, const std::string& name = "")
	{
		views::ViewHandler view_handler = [this](
			http::HttpRequest* request,
			views::Args* args,
			conf::Settings* settings_ptr
		) -> core::Result<std::shared_ptr<http::IHttpResponse>>
		{
			ViewT view(settings_ptr);
			view.setup(request);
			return view.dispatch(args);
		};

		this->_urlpatterns.push_back(urls::make_url(
			core::str::starts_with(pattern, "/") ? pattern : "/" + pattern,
			view_handler,
			name
		));
	}

	template <typename AppConfigT, typename = std::enable_if<std::is_base_of<IAppConfig, AppConfigT>::value>>
	void include(const std::string& prefix, const std::string& namespace_ = "")
	{
		auto app = this->find_or_create_app<AppConfigT>();
		auto included_urlpatterns = app->get_urlpatterns();
		std::string ns = namespace_.empty() ? app->get_name() : namespace_;
		for (const auto& pattern : included_urlpatterns)
		{
			this->_urlpatterns.push_back(std::make_shared<urls::UrlPattern>(
				core::str::rtrim(
					core::str::starts_with(prefix, "/") ? prefix : "/" + prefix,
					"/"
				),
				pattern,
				ns
			));
		}
	}

	template <typename CommandT, typename = std::enable_if<std::is_base_of<cmd::AppCommand, CommandT>::value>>
	void command()
	{
		auto cmd = std::make_shared<CommandT>(this, this->settings);
		this->_commands.push_back(cmd);
	}

	explicit AppConfig(
		const std::string& app_path, conf::Settings* settings
	);

public:
	void init(const core::object::Type& type);
	[[nodiscard]] bool is_initialized() const override;
	std::string get_name() final;
	std::string get_app_path() final;
	std::vector<std::shared_ptr<urls::UrlPattern>> get_urlpatterns() final;
	std::vector<std::shared_ptr<cmd::BaseCommand>> get_commands() final;
	virtual void urlpatterns();
	virtual void commands();
};

__APPS_END__
