/**
 * conf/module.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Configuration of separate module with urls, commands and other
 * stuff.
 */

#pragma once

// Base libraries.
#include <xalwart.base/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../views/view.h"
#include "../urls/url.h"
#include "../commands/command.h"


__CONF_BEGIN__

// Derived class must contain constructor with
// pointer to conf::Settings parameter.
class ModuleConfig : public IModuleConfig
{
private:
	bool _is_initialized;

	friend class conf::Settings;

	std::vector<std::shared_ptr<urls::UrlPattern>> _urlpatterns;
	std::vector<std::shared_ptr<cmd::BaseCommand>> _commands;
	std::vector<std::function<void()>> _sub_modules_to_init;

	inline std::shared_ptr<IModuleConfig> _find_module(const std::string& module)
	{
		auto result = std::find_if(
			this->settings->MODULES.begin(), this->settings->MODULES.end(),
			[module](const std::shared_ptr<IModuleConfig>& entry) -> bool {
				return entry->get_name() == module;
			}
		);
		if (result == this->settings->MODULES.end())
		{
			throw ImproperlyConfigured(
				"module is used but was not registered: " + module, _ERROR_DETAILS_
			);
		}

		return *result;
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
	inline void url(const std::string& pattern, const std::string& name="")
	{
		views::ViewHandler view_handler = [this](
			http::HttpRequest* request,
			views::Args* args,
			conf::Settings* settings_ptr
		) -> Result<std::shared_ptr<http::IHttpResponse>>
		{
			ViewT view(settings_ptr);
			view.setup(request);
			return view.dispatch(args);
		};

		this->_urlpatterns.push_back(urls::make_url(
			pattern.starts_with("/") ? pattern : "/" + pattern,
			view_handler,
			name
		));
	}

	inline void include(
		const std::string& module, const std::string& prefix, const std::string& namespace_=""
	)
	{
		this->_sub_modules_to_init.emplace_back([this, prefix, namespace_, module]() -> void {
			auto app = this->_find_module(module);
			auto included_urlpatterns = app->get_urlpatterns();
			auto ns = namespace_.empty() ? app->get_name() : namespace_;
			for (const auto& pattern : included_urlpatterns)
			{
				this->_urlpatterns.push_back(std::make_shared<urls::UrlPattern>(
					str::rtrim(prefix.starts_with("/") ? prefix : "/" + prefix, "/"),
					pattern,
					ns
				));
			}
		});
	}

	template <cmd::command_type CommandT>
	inline void command()
	{
		auto cmd = std::make_shared<CommandT>(this, this->settings);
		this->_commands.push_back(cmd);
	}

	template <cmd::command_type CommandT, typename ...Args>
	inline void command(Args&& ...args)
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

	virtual void init(const std::string& name);
};

__CONF_END__