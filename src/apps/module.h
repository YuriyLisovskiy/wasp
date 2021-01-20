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

#include <iostream>

// Core libraries.
#include <xalwart.core/string_utils.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../views/view.h"
#include "../urls/url.h"
#include "../commands/app_command.h"

template<unsigned N>
struct FixedString
{
	char buf[N + 1]{};
	int length = N;

	constexpr FixedString(char const* string)
	{
		for (unsigned index = 0; index < N; ++index) {
			buf[index] = string[index];
		}
	}
	constexpr operator char const*() const { return buf; }
};

template<unsigned N>
FixedString(char const (&)[N]) -> FixedString<N - 1>;

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

//	template <typename ModuleConfigT>
	std::shared_ptr<IModuleConfig> _find_module(const std::string& name)
	{
		auto module = std::find_if(
			this->settings->INSTALLED_MODULES.begin(),
			this->settings->INSTALLED_MODULES.end(),
			[name](const std::shared_ptr<IModuleConfig>& entry) -> bool {
				return entry->get_name() == name;
//				return dynamic_cast<ModuleConfigT*>(entry.get()) != nullptr;
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

//	template<unsigned N>
//	struct FixedString {
//		char buf[N + 1]{};
//		constexpr FixedString(char const* s)
//		{
//			for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
//		}
//
//		constexpr operator char const*() const { return buf; }
//	};
//
//	template<unsigned N> FixedString(char const (&)[N]) -> FixedString<N - 1>;

//	struct constexpr_str {
//		char const* str;
//		std::size_t size;
//
//		// can only construct from a char[] literal
//		template <std::size_t N>
//		constexpr explicit constexpr_str(const char (&s)[N]) : str(s), size(N - 1) // not count the trailing nul
//		{
////			this->str[N - 1] = '\0';
//		}
//	};

	template <FixedString module_config_name>
	void include(const std::string& prefix, const std::string& namespace_="")
	{
		this->_sub_modules_to_init.emplace_back([this, prefix, namespace_]() -> void {
			auto app = this->_find_module(module_config_name.buf);
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
