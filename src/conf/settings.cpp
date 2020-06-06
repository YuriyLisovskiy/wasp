/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of conf/settings.h
 */

// Header.
#include "./settings.h"

// Vendor.
#include <yaml-cpp/yaml.h>

// Framework modules.
#include "../core/path.h"
#include "../render/env/default.h"
#include "../render/library/builtin.h"


__CONF_BEGIN__

YAML::Node Settings::_load_config()
{
	auto config_path = core::path::join(this->BASE_DIR, this->CONFIG_NAME);
	if (core::path::exists(config_path))
	{
		auto config = YAML::LoadFile(config_path);
		if (!config.IsMap())
		{
			throw core::ValueError("configuration file must have map type");
		}

		if (!config[this->CONFIG_ROOT])
		{
			throw core::ValueError(
				"configuration file is not valid: missing '" + this->CONFIG_ROOT + "' which is root node"
			);
		}

		return config[this->CONFIG_ROOT];
	}

	return YAML::Node();
}

void Settings::_override_config(YAML::Node& config)
{
	// TODO: load config.local.yml and override @config
}

Settings::Settings(const std::string& base_dir)
{
	this->BASE_DIR = base_dir;
}

void Settings::_init_env(YAML::Node& env)
{
	std::vector<std::string> dirs;
	auto directories = env["directories"];
	if (directories && directories.IsSequence())
	{
		for (auto it = directories.begin(); it != directories.end(); it++)
		{
			if (!it->IsNull())
			{
				auto p = it->as<std::string>();
				dirs.push_back(
					core::path::is_absolute(p) ? p : core::path::join(this->BASE_DIR, p)
				);
			}
		}
	}

	std::vector<std::shared_ptr<render::lib::ILibrary>> libs{
		this->_factory->get_library(xw::render::lib::BuiltinLibrary::FULL_NAME)
	};
	auto libraries = env["libraries"];
	if (libraries && libraries.IsSequence())
	{
		for (auto it = libraries.begin(); it != libraries.end(); it++)
		{
			if (!it->IsNull())
			{
				libs.push_back(this->_factory->get_library(it->as<std::string>()));
			}
		}
	}

	auto use_app_dirs = env["use_app_directories"];
	auto auto_escape = env["auto_escape"];
	this->TEMPLATES_ENV = render::env::DefaultEnvironment::Config(
		dirs,
		use_app_dirs ? use_app_dirs.as<bool>() : true,
		this->INSTALLED_APPS,
		this->DEBUG,
		this->LOGGER.get(),
		auto_escape ? auto_escape.as<bool>() : true,
		libs
	).make_env();
}

void Settings::_init_logger(YAML::Node& logger)
{
	core::LoggerConfig logger_config{};
	auto levels = logger["levels"];
	if (levels)
	{
		logger_config.enable_info = levels["info"].as<bool>(true);
		logger_config.enable_debug = levels["debug"].as<bool>(true);
		logger_config.enable_warning = levels["warning"].as<bool>(true);
		logger_config.enable_error = levels["error"].as<bool>(true);
		logger_config.enable_fatal = levels["fatal"].as<bool>(true);
		logger_config.enable_print = levels["print"].as<bool>(true);
	}

	auto out = logger["out"];
	if (out)
	{
		// TODO: setup logger's streams
	}

	this->LOGGER = core::Logger::get_instance(logger_config);
}

void Settings::_init_allowed_hosts(YAML::Node& allowed_hosts)
{
	for (auto it = allowed_hosts.begin(); it != allowed_hosts.end(); it++)
	{
		if (!it->IsNull())
		{
			this->ALLOWED_HOSTS.push_back(it->as<std::string>());
		}
	}
}

void Settings::_init_disallowed_user_agents(YAML::Node& agents)
{
	for (auto it = agents.begin(); it != agents.end(); it++)
	{
		if (!it->IsNull())
		{
			this->DISALLOWED_USER_AGENTS.emplace_back(it->as<std::string>());
		}
	}
}

void Settings::_init_ignorable_404_urls(YAML::Node& urls)
{
	for (auto it = urls.begin(); it != urls.end(); it++)
	{
		if (!it->IsNull())
		{
			this->IGNORABLE_404_URLS.emplace_back(it->as<std::string>());
		}
	}
}

void Settings::_init_formats(YAML::Node& config)
{
	YAML::Node formats;
	if (config["formats"])
	{
		formats = config["formats"];
	}

	if (formats.IsMap())
	{
		this->DATE_FORMAT = formats["date"].as<std::string>("%b %d, %Y");
		this->DATETIME_FORMAT = formats["datetime"].as<std::string>("%b %d, %Y, %T");
		this->TIME_FORMAT = formats["time"].as<std::string>("%T");
		this->YEAR_MONTH_FORMAT = formats["year_month"].as<std::string>("%B %Y");
		this->MONTH_DAY_FORMAT = formats["month_day"].as<std::string>("%B %d");
		this->SHORT_DATE_FORMAT = formats["short_date"].as<std::string>("%m/%d/%Y");
		this->SHORT_DATETIME_FORMAT = formats["short_datetime"].as<std::string>("%m/%d/%Y %T");
	}
}

void Settings::_init_csrf(YAML::Node& config)
{
	YAML::Node csrf;
	if (config["csrf"])
	{
		csrf = config["csrf"];
	}

	YAML::Node cookie;
	if (csrf["cookie"])
	{
		cookie = csrf["cookie"];
	}

	this->CSRF_COOKIE_NAME = cookie["name"].as<std::string>("csrftoken");
	this->CSRF_COOKIE_AGE = cookie["age"].as<size_t>(60 * 60 * 24 * 7 * 52);
	this->CSRF_COOKIE_DOMAIN = cookie["domain"].as<std::string>("");
	this->CSRF_COOKIE_PATH = cookie["path"].as<std::string>("/");
	this->CSRF_COOKIE_SECURE = cookie["secure"].as<bool>(false);
	this->CSRF_COOKIE_HTTP_ONLY = cookie["http_only"].as<bool>(false);
	this->CSRF_COOKIE_SAME_SITE = cookie["same_site"].as<std::string>("Lax");

	this->CSRF_HEADER_NAME = csrf["header_name"].as<std::string>("X-XSRF-TOKEN");
	this->CSRF_USE_SESSIONS = csrf["use_sessions"].as<bool>(false);

	auto origins = csrf["trusted_origins"];
	if (origins && origins.IsSequence())
	{
		for (auto it = origins.begin(); it != origins.end(); it++)
		{
			if (!it->IsNull())
			{
				this->CSRF_TRUSTED_ORIGINS.push_back(it->as<std::string>());
			}
		}
	}
}

void Settings::_init_secure(YAML::Node& config)
{
	YAML::Node secure;
	if (config["secure"])
	{
		secure = config["secure"];
	}

	this->SECURE_BROWSER_XSS_FILTER = secure["browser_xss_filter"].as<bool>(false);
	this->SECURE_CONTENT_TYPE_NO_SNIFF = secure["content_type_no_sniff"].as<bool>(true);
	this->SECURE_HSTS_INCLUDE_SUBDOMAINS = secure["hsts_include_subdomains"].as<bool>(false);
	this->SECURE_HSTS_PRELOAD = secure["hsts_preload"].as<bool>(false);
	this->SECURE_HSTS_SECONDS = secure["hsts_seconds"].as<size_t>(0);
	auto redirect_exempt = secure["redirect_exempt"];
	if (redirect_exempt && redirect_exempt.IsSequence())
	{
		for (auto it = redirect_exempt.begin(); it != redirect_exempt.end(); it++)
		{
			if (!it->IsNull())
			{
				this->SECURE_REDIRECT_EXEMPT.push_back(it->as<std::string>());
			}
		}
	}

	this->SECURE_REFERRER_POLICY = secure["referrer_policy"].as<std::string>("");
	this->SECURE_SSL_HOST = secure["ssl_host"].as<std::string>("");
	this->SECURE_SSL_REDIRECT = secure["ssl_redirect"].as<bool>(false);
}

void Settings::_init_apps(YAML::Node& apps)
{
	for (auto it = apps.begin(); it != apps.end(); it++)
	{
		if (!it->IsNull())
		{
			auto item = this->_factory->get_app(it->as<std::string>());
			if (item)
			{
				this->INSTALLED_APPS.push_back(item);
			}
		}
	}
}

void Settings::_init_middleware(YAML::Node& middleware)
{
	for (auto it = middleware.begin(); it != middleware.end(); it++)
	{
		if (!it->IsNull())
		{
			auto item = this->_factory->get_middleware(it->as<std::string>());
			if (item)
			{
				this->MIDDLEWARE.push_back(item);
			}
		}
	}
}

void Settings::init()
{
	auto config = this->_load_config();
	this->_override_config(config);

	this->DEBUG = config["debug"].as<bool>(false);

	auto logger = config["logger"];
	if (logger && logger.IsMap())
	{
		this->_init_logger(logger);
	}
	else
	{
		this->register_logger();
	}

	auto allowed_hosts = config["allowed_hosts"];
	if (allowed_hosts && allowed_hosts.IsSequence())
	{
		this->_init_allowed_hosts(allowed_hosts);
	}

	// TODO: create initialization of TZ
	this->TIME_ZONE = std::make_shared<core::dt::Timezone>(core::dt::Timezone::UTC);

	this->USE_TZ = config["use_tz"].as<bool>(false);

	this->CHARSET = config["charset"].as<std::string>("utf-8");

	this->APPEND_SLASH = config["append_slash"].as<bool>(true);

	auto disallowed_user_agents = config["disallowed_user_agents"];
	if (disallowed_user_agents && disallowed_user_agents.IsSequence())
	{
		this->_init_disallowed_user_agents(disallowed_user_agents);
	}

	auto ignorable_404_urls = config["ignorable_404_urls"];
	if (ignorable_404_urls && ignorable_404_urls.IsSequence())
	{
		this->_init_ignorable_404_urls(ignorable_404_urls);
	}

	this->SECRET_KEY = config["secret_key"].as<std::string>("");

	auto media = config["media"];
	if (media && media.IsMap())
	{
		auto p = media["root"].as<std::string>("media");
		this->MEDIA_ROOT = core::path::is_absolute(p) ? p : core::path::join(this->BASE_DIR, p);
		this->MEDIA_URL = media["url"].as<std::string>("/media/");
	}

	auto static_ = config["static"];
	if (static_ && static_.IsMap())
	{
		auto p = static_["root"].as<std::string>("static");
		this->STATIC_ROOT = core::path::is_absolute(p) ? p : core::path::join(this->BASE_DIR, p);
		this->STATIC_URL = static_["url"].as<std::string>("/static/");
	}

	this->FILE_UPLOAD_MAX_MEMORY_SIZE = config["file_upload_max_memory_size"].as<int>(2621440);
	this->DATA_UPLOAD_MAX_MEMORY_SIZE = config["data_upload_max_memory_size"].as<int>(2621440);

	this->PREPEND_WWW = config["prepend_www"].as<bool>(false);

	this->DATA_UPLOAD_MAX_NUMBER_FIELDS = config["data_upload_max_number_fields"].as<int>(1000);

	this->_init_formats(config);

	this->FIRST_DAY_OF_WEEK = config["first_day_of_week"].as<int>(0);

	this->DECIMAL_SEPARATOR = config["decimal_separator"].as<char>('.');

	this->USE_THOUSAND_SEPARATOR = config["use_thousand_separator"].as<bool>(false);

	this->THOUSAND_SEPARATOR = config["thousand_separator"].as<char>(',');

	this->X_FRAME_OPTIONS = config["x_frame_options"].as<std::string>("SAMEORIGIN");

	this->USE_X_FORWARDED_HOST = config["use_x_forwarded_host"].as<bool>(false);

	this->USE_X_FORWARDED_PORT = config["use_x_forwarded_port"].as<bool>(false);

	auto spsslh = config["secure_proxy_ssl_header"];
	if (spsslh && spsslh.IsMap())
	{
		this->SECURE_PROXY_SSL_HEADER = std::make_shared<std::pair<std::string, std::string>>(
			spsslh["name"].as<std::string>("X-Forwarded-Proto"),
			spsslh["value"].as<std::string>("https")
		);
	}

	this->_init_csrf(config);

	this->USE_SSL = config["use_ssl"].as<bool>(false);

	this->_init_secure(config);

	this->init_factory();

	this->register_apps();
	auto apps = config["installed_apps"];
	if (apps && apps.IsSequence())
	{
		this->_init_apps(apps);
	}

	this->register_middleware();
	auto middleware = config["middleware"];
	if (middleware && middleware.IsSequence())
	{
		this->_init_middleware(middleware);
	}

	this->register_libraries();
	auto env = config["templates_env"];
	if (env && env.IsMap())
	{
		this->_init_env(env);
	}
	else
	{
		this->register_templates_env();
	}

	delete _factory;
}

void Settings::init_factory()
{
	this->_factory = new internal::SettingsFactory(this, this->LOGGER.get());
}

void Settings::register_logger()
{
}

void Settings::register_apps()
{
}

void Settings::register_middleware()
{
}

void Settings::register_libraries()
{
}

void Settings::register_templates_env()
{
}

void Settings::prepare()
{
	if (!this->ROOT_APP && !this->INSTALLED_APPS.empty())
	{
		this->ROOT_APP = this->INSTALLED_APPS.front();
	}
}

__CONF_END__
