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
#include "./interfaces.h"
#include "../urls/pattern.h"
#include "../controllers/controller.h"
#include "../commands/command.h"


__CONF_BEGIN__

// TESTME: ModuleConfig
// TODO: docs for 'ModuleConfig'
// Derived class must contain constructor with pointer
// to 'conf::Settings' parameter.
class ModuleConfig : public IModuleConfig
{
public:
	[[nodiscard]]
	inline bool is_configured() const override
	{
		return this->_is_configured;
	}

	inline void configure() override
	{
		this->set_ready();
	}

	[[nodiscard]]
	inline std::string get_name() const final
	{
		return this->_module_name;
	}

	[[nodiscard]]
	std::vector<std::shared_ptr<urls::IPattern>> get_urlpatterns() final;

	[[nodiscard]]
	std::vector<std::shared_ptr<cmd::AbstractCommand>> get_commands() final;

protected:
	conf::Settings* settings;

	inline explicit ModuleConfig(std::string name, conf::Settings* settings) :
		_module_name(std::move(name)), _is_configured(false), settings(settings)
	{
		if (_module_name.empty())
		{
			throw ValueError("module name should not be empty", _ERROR_DETAILS_);
		}
	}

	template <
		typename ControllerType, typename ...RequestArgs, typename ...ControllerArgs,
		typename = std::enable_if<std::is_base_of<ctrl::Controller<RequestArgs...>, ControllerType>::value>
	>
	inline void url(const std::string& pattern, const std::string& name, ControllerArgs ...controller_args)
	{
		ctrl::Handler<RequestArgs...> controller_handler = [controller_args...](
			http::IRequest* request,
			const std::tuple<RequestArgs...>& request_args,
			const Settings* settings_ptr
		) -> std::unique_ptr<http::IResponse>
		{
			ControllerType controller(
				require_non_null(settings_ptr, "'settings' is nullptr", _ERROR_DETAILS_)->LOGGER.get(),
				controller_args...
			);
			return std::apply(
				[&controller, request](RequestArgs ...a) mutable -> auto
				{
					return controller.dispatch(request, a...);
				},
				request_args
			);
		};

		this->_urlpatterns.push_back(std::make_shared<urls::Pattern<RequestArgs...>>(
			pattern.starts_with("/") ? pattern : "/" + pattern,
			controller_handler,
			name.empty() ? demangle(typeid(ControllerType).name()) : name
		));
	}

	inline void include(const std::string& module, const std::string& prefix, const std::string& namespace_="")
	{
		this->_sub_modules_to_init.emplace_back([this, prefix, namespace_, module]() -> void {
			auto app = this->_find_module(module);
			auto included_urlpatterns = app->get_urlpatterns();
			auto ns = namespace_.empty() ? app->get_name() : namespace_;
			for (const auto& pattern : included_urlpatterns)
			{
				pattern->add_namespace(ns);
				pattern->add_prefix(str::rtrim(prefix.starts_with("/") ? prefix : "/" + prefix, "/"));
				this->_urlpatterns.push_back(pattern);
			}
		});
	}

	template <cmd::command_type CommandType, typename ...Args>
	inline void command(Args&& ...args)
	{
		this->_commands.push_back(std::make_shared<CommandType>(std::forward<Args>(args)...));
	}

	virtual inline void urlpatterns()
	{
	}

	virtual inline void commands()
	{
	}

	void set_ready()
	{
		this->_is_configured = true;
	}

private:
	bool _is_configured;
	std::string _module_name;

	friend class conf::Settings;

	std::vector<std::shared_ptr<urls::IPattern>> _urlpatterns;
	std::vector<std::shared_ptr<cmd::AbstractCommand>> _commands;
	std::vector<std::function<void()>> _sub_modules_to_init;

	inline std::shared_ptr<IModuleConfig> _find_module(const std::string& module)
	{
		auto result = std::find_if(
			this->settings->MODULES.begin(), this->settings->MODULES.end(),
			[&module](const std::shared_ptr<IModuleConfig>& entry) -> bool {
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
};

__CONF_END__
