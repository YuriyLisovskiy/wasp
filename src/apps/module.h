/**
 * apps/module.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Represents module configuration with urls and commands.
 */

#pragma once

// Core libraries.
#include <xalwart.core/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../views/view.h"
#include "../urls/url.h"
#include "../commands/app_command.h"


__APPS_BEGIN__

/// Derived class must contain constructor with
/// pointer to conf::Settings parameter.
class ModuleConfig : public IModuleConfig
{
private:
	bool _is_initialized;

	friend class conf::Settings;

	std::vector<std::shared_ptr<urls::UrlPattern>> _urlpatterns;
	std::vector<std::shared_ptr<cmd::BaseCommand>> _commands;
	std::vector<std::function<void()>> _sub_modules_to_init;

	std::shared_ptr<IModuleConfig> _find_module(const std::string& name)
	{
		auto module = std::find_if(
			this->settings->INSTALLED_MODULES.begin(),
			this->settings->INSTALLED_MODULES.end(),
			[name](const std::shared_ptr<IModuleConfig>& entry) -> bool {
				return entry->get_name() == name;
			}
		);
		if (module == this->settings->INSTALLED_MODULES.end())
		{
			throw core::ImproperlyConfigured(
				"module is used but was not enabled and(or) registered: " + name
			);
		}

		return *module;
	}

protected:
	std::string module_name;
	std::string module_path;
	conf::Settings* settings;

protected:
	explicit ModuleConfig(
		std::string app_path, conf::Settings* settings
	);

	template <typename ViewT, typename = std::enable_if<std::is_base_of<views::View, ViewT>::value>>
	void url(const std::string& pattern, const std::string& name="")
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
			str::starts_with(pattern, "/") ? pattern : "/" + pattern,
			view_handler,
			name
		));
	}

	template <str::fixed_string module_name>
	void include(const std::string& prefix, const std::string& namespace_="")
	{
		this->_sub_modules_to_init.emplace_back([this, prefix, namespace_]() -> void {
			auto app = this->_find_module(module_name.value);
			auto included_urlpatterns = app->get_urlpatterns();
			auto ns = namespace_.empty() ? app->get_name() : namespace_;
			for (const auto& pattern : included_urlpatterns)
			{
				this->_urlpatterns.push_back(std::make_shared<urls::UrlPattern>(
					str::rtrim(
						str::starts_with(prefix, "/") ? prefix : "/" + prefix,
						"/"
					),
					pattern,
					ns
				));
			}
		});
	}

	template <typename CommandT, typename = std::enable_if<std::is_base_of<cmd::AppCommand, CommandT>::value>>
	void command()
	{
		auto cmd = std::make_shared<CommandT>(this, this->settings);
		this->_commands.push_back(cmd);
	}

	template <
		typename CommandT,
		typename = std::enable_if<std::is_base_of<cmd::AppCommand, CommandT>::value>,
		typename ...Args
	>
	void command(Args&& ...args)
	{
		auto cmd = std::make_shared<CommandT>(std::forward<Args>(args)...);
		this->_commands.push_back(cmd);
	}

	virtual void urlpatterns();

	virtual void commands();

public:
	[[nodiscard]]
	bool ready() const override;

	[[nodiscard]]
	std::string get_name() const final;

	[[nodiscard]]
	std::string get_module_path() const final;

	[[nodiscard]]
	std::vector<std::shared_ptr<urls::UrlPattern>> get_urlpatterns() final;

	[[nodiscard]]
	std::vector<std::shared_ptr<cmd::BaseCommand>> get_commands() final;

	virtual void init(const std::string& custom_name);
};

__APPS_END__
