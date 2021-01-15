/**
 * conf/loaders/yaml_settings_loader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./yaml_settings_loader.h"

// C++ libraries.
// TODO

// Core libraries.
#include <xalwart.core/string_utils.h>

// Render libraries.
#include <xalwart.render/engine.h>

// Framework libraries.
#include "../../render/library/default.h"


__CONF_BEGIN__

std::string YamlSettingsLoader::config_name() const
{
	return "config.yaml";
}

std::string YamlSettingsLoader::local_config_name() const
{
	return "config.local.yaml";
}

YAML::Node YamlSettingsLoader::load_yaml(const std::string& file_path)
{
	if (path::exists(file_path))
	{
		auto config = YAML::LoadFile(file_path);
//		this->check_config(config, file_path);
		return config;
	}

	return YAML::Node(YAML::NodeType::Undefined);
}

void YamlSettingsLoader::check_config(const YAML::Node& config, const std::string& file_path)
{
	if (!config || !config.IsMap())
	{
		throw core::ValueError("'" + file_path + "' file must have map type");
	}
}

void YamlSettingsLoader::overwrite_scalar(
	YAML::Node& config, const YAML::Node& local_config, const std::string& key
)
{
	auto value = local_config[key];
	if (value)
	{
		if (value.IsScalar())
		{
			config[key] = local_config[key];
		}
		else if (value.IsNull())
		{
			if (config[key])
			{
				config.remove(key);
			}
		}
	}
}

void YamlSettingsLoader::overwrite_sequence(
	YAML::Node& config, const YAML::Node& local_config, const std::string& key
)
{
	auto value = local_config[key];
	if (value)
	{
		if (value.IsSequence())
		{
			config[key] = local_config[key];
		}
		else if (value.IsNull())
		{
			if (config[key])
			{
				config.remove(key);
			}
		}
	}
}

void YamlSettingsLoader::overwrite_config(YAML::Node& config, const YAML::Node& local_config)
{
	this->overwrite_scalar(config, local_config, "debug");

	auto local_logger = local_config["logger"];
	if (local_logger)
	{
		if (local_logger.IsNull() && config["logger"])
		{
			config.remove("logger");
		}
		else if (local_logger.IsMap())
		{
			auto logger = config["logger"];
			if (!logger)
			{
				config["logger"] = local_logger;
			}
			else
			{
				auto local_levels = local_logger["levels"];
				if (local_levels)
				{
					if (local_levels.IsNull() && logger["levels"])
					{
						logger.remove("levels");
					}
					else if (local_levels.IsMap())
					{
						auto levels = logger["levels"];
						if (!levels)
						{
							logger["levels"] = local_levels;
						}
						else
						{
							this->overwrite_scalar(levels, local_levels, "info");
							this->overwrite_scalar(levels, local_levels, "debug");
							this->overwrite_scalar(levels, local_levels, "warning");
							this->overwrite_scalar(levels, local_levels, "error");
							this->overwrite_scalar(levels, local_levels, "fatal");
							this->overwrite_scalar(levels, local_levels, "print");
							logger["levels"] = levels;
						}
					}
				}

				auto local_out = local_logger["out"];
				if (local_out)
				{
					if (local_out.IsNull() && logger["out"])
					{
						logger.remove("out");
					}
					else if (local_out.IsMap())
					{
						auto out = logger["out"];
						if (!out)
						{
							logger["out"] = local_out;
						}
						else
						{
							this->overwrite_scalar(out, local_out, "console");
							this->overwrite_scalar(out, local_out, "file");
							this->overwrite_sequence(out, local_out, "files");
						}
					}
				}

				config["logger"] = logger;
			}
		}
	}

	this->overwrite_sequence(config, local_config, "allowed_hosts");

	auto local_timezone = config["timezone"];
	if (local_timezone)
	{
		if (local_timezone.IsNull() && config["timezone"])
		{
			config.remove("timezone");
		}
		else if (local_timezone.IsMap())
		{
			auto timezone = config["timezone"];
			if (!timezone)
			{
				config["timezone"] = local_timezone;
			}
			else
			{
				this->overwrite_scalar(timezone, local_timezone, "name");
				auto local_offset = local_timezone["offset"];
				if (local_offset.IsNull() && timezone["offset"])
				{
					timezone.remove("offset");
				}
				else if (local_offset.IsMap())
				{
					auto offset = timezone["offset"];
					if (!offset)
					{
						timezone["offset"] = local_offset;
					}
					else
					{
						this->overwrite_scalar(offset, local_offset, "days");
						this->overwrite_scalar(offset, local_offset, "seconds");
						this->overwrite_scalar(offset, local_offset, "microseconds");
						this->overwrite_scalar(offset, local_offset, "milliseconds");
						this->overwrite_scalar(offset, local_offset, "minutes");
						this->overwrite_scalar(offset, local_offset, "hours");
						this->overwrite_scalar(offset, local_offset, "weeks");
						timezone["offset"] = offset;
					}
				}

				config["timezone"] = timezone;
			}
		}
	}

	this->overwrite_scalar(config, local_config, "use_tz");
	this->overwrite_scalar(config, local_config, "charset");
	this->overwrite_scalar(config, local_config, "append_slash");
	this->overwrite_sequence(config, local_config, "disallowed_user_agents");
	this->overwrite_sequence(config, local_config, "ignorable_404_urls");
	this->overwrite_scalar(config, local_config, "secret_key");

	auto local_media = local_config["media"];
	if (local_media)
	{
		if (local_media.IsNull() && config["media"])
		{
			config.remove("media");
		}
		else if (local_media.IsMap())
		{
			auto media = config["media"];
			if (!media)
			{
				config["media"] = local_media;
			}
			else
			{
				this->overwrite_scalar(media, local_media, "root");
				this->overwrite_scalar(media, local_media, "url");
				config["media"] = media;
			}
		}
	}

	auto local_static = local_config["static"];
	if (local_static)
	{
		if (local_static.IsNull() && config["static"])
		{
			config.remove("static");
		}
		else if (local_static.IsMap())
		{
			auto static_ = config["static"];
			if (!static_)
			{
				config["static"] = local_static;
			}
			else
			{
				this->overwrite_scalar(static_, local_static, "root");
				this->overwrite_scalar(static_, local_static, "url");
				config["static"] = static_;
			}
		}
	}

	this->overwrite_scalar(config, local_config, "file_upload_max_memory_size");
	this->overwrite_scalar(config, local_config, "data_upload_max_memory_size");
	this->overwrite_scalar(config, local_config, "prepend_www");
	this->overwrite_scalar(config, local_config, "data_upload_max_number_fields");

	auto local_formats = local_config["formats"];
	if (local_formats)
	{
		if (local_formats.IsNull() && config["formats"])
		{
			config.remove("formats");
		}
		else if (local_formats.IsMap())
		{
			auto formats = config["formats"];
			if (!formats)
			{
				config["formats"] = local_formats;
			}
			else
			{
				this->overwrite_scalar(formats, local_formats, "date");
				this->overwrite_scalar(formats, local_formats, "datetime");
				this->overwrite_scalar(formats, local_formats, "time");
				this->overwrite_scalar(formats, local_formats, "year_month");
				this->overwrite_scalar(formats, local_formats, "month_day");
				this->overwrite_scalar(formats, local_formats, "short_date");
				this->overwrite_scalar(formats, local_formats, "short_datetime");
				config["formats"] = formats;
			}
		}
	}

	this->overwrite_scalar(config, local_config, "first_day_of_week");
	this->overwrite_scalar(config, local_config, "decimal_separator");
	this->overwrite_scalar(config, local_config, "use_thousand_separator");
	this->overwrite_scalar(config, local_config, "thousand_separator");
	this->overwrite_scalar(config, local_config, "x_frame_options");
	this->overwrite_scalar(config, local_config, "use_x_forwarded_host");
	this->overwrite_scalar(config, local_config, "use_x_forwarded_port");

	auto local_csrf = local_config["csrf"];
	if (local_csrf)
	{
		if (local_csrf.IsNull() && config["csrf"])
		{
			config.remove("csrf");
		}
		else if (local_csrf.IsMap())
		{
			auto csrf = config["csrf"];
			if (!csrf)
			{
				config["csrf"] = local_csrf;
			}
			else
			{
				auto local_cookie = local_csrf["cookie"];
				if (local_cookie)
				{
					if (local_cookie.IsNull() && csrf["cookie"])
					{
						csrf.remove("cookie");
					}
					else if (local_cookie.IsMap())
					{
						auto cookie = csrf["cookie"];
						if (!cookie)
						{
							csrf["cookie"] = local_cookie;
						}
						else
						{
							this->overwrite_scalar(cookie, local_cookie, "name");
							this->overwrite_scalar(cookie, local_cookie, "age");
							this->overwrite_scalar(cookie, local_cookie, "domain");
							this->overwrite_scalar(cookie, local_cookie, "path");
							this->overwrite_scalar(cookie, local_cookie, "secure");
							this->overwrite_scalar(cookie, local_cookie, "http_only");
							this->overwrite_scalar(cookie, local_cookie, "same_site");
							csrf["cookie"] = cookie;
						}
					}
				}

				this->overwrite_scalar(csrf, local_csrf, "header_name");
				this->overwrite_scalar(csrf, local_csrf, "use_sessions");
				this->overwrite_sequence(csrf, local_csrf, "trusted_origins");
				config["csrf"] = csrf;
			}
		}
	}

	this->overwrite_scalar(config, local_config, "use_ssl");
	auto local_secure = local_config["secure"];
	if (local_secure)
	{
		if (local_secure.IsNull() && config["secure"])
		{
			config.remove("secure");
		}
		else if (local_secure.IsMap())
		{
			auto secure = config["secure"];
			if (!secure)
			{
				config["secure"] = local_secure;
			}
			else
			{
				this->overwrite_scalar(secure, local_secure, "browser_xss_filter");
				this->overwrite_scalar(secure, local_secure, "content_type_no_sniff");
				this->overwrite_scalar(secure, local_secure, "hsts_include_subdomains");
				this->overwrite_scalar(secure, local_secure, "hsts_preload");
				this->overwrite_scalar(secure, local_secure, "hsts_seconds");
				this->overwrite_sequence(secure, local_secure, "redirect_exempt");
				this->overwrite_scalar(secure, local_secure, "referrer_policy");
				this->overwrite_scalar(secure, local_secure, "ssl_host");
				this->overwrite_scalar(secure, local_secure, "ssl_redirect");
				auto local_psslh = local_secure["proxy_ssl_header"];
				if (local_psslh)
				{
					if (local_psslh.IsNull() && secure["proxy_ssl_header"])
					{
						secure.remove("proxy_ssl_header");
					}
					else if (local_psslh.IsMap())
					{
						auto psslh = secure["proxy_ssl_header"];
						if (!psslh)
						{
							secure["proxy_ssl_header"] = local_psslh;
						}
						else
						{
							this->overwrite_scalar(psslh, local_psslh, "name");
							this->overwrite_scalar(psslh, local_psslh, "value");
							secure["proxy_ssl_header"] = psslh;
						}
					}
				}

				config["secure"] = secure;
			}
		}
	}

	this->overwrite_sequence(config, local_config, "installed_apps");
	this->overwrite_sequence(config, local_config, "middleware");

	auto local_template_engine = local_config["template_engine"];
	if (local_template_engine)
	{
		if (local_template_engine.IsNull() && config["template_engine"])
		{
			config.remove("template_engine");
		}
		else if (local_template_engine.IsMap())
		{
			auto env = config["template_engine"];
			if (!env)
			{
				config["template_engine"] = local_template_engine;
			}
			else
			{
				this->overwrite_sequence(env, local_template_engine, "directories");
				this->overwrite_sequence(env, local_template_engine, "libraries");
				this->overwrite_scalar(env, local_template_engine, "use_app_directories");
				this->overwrite_scalar(env, local_template_engine, "auto_escape");
				this->overwrite_scalar(env, local_template_engine, "use_default_engine");
			}
		}
	}
}

void YamlSettingsLoader::init(Settings* settings, const YAML::Node& config)
{
	settings->DEBUG = config["debug"].as<bool>(false);

	auto logger = config["logger"];
	if (logger && logger.IsMap())
	{
		this->init_logger(settings, logger);
	}

	auto allowed_hosts = config["allowed_hosts"];
	if (allowed_hosts && allowed_hosts.IsSequence())
	{
		this->init_allowed_hosts(settings, allowed_hosts);
	}

	auto timezone = config["timezone"];
	if (!timezone || !timezone.IsMap())
	{
		settings->TIME_ZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);
	}
	else
	{
		auto timezone_name = str::upper(timezone["name"].as<std::string>("UTC"));
		if (timezone_name == "UTC")
		{
			settings->TIME_ZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);
		}
		else
		{
			auto timezone_offset = timezone["offset"];
			if (timezone_offset && timezone_offset.IsMap())
			{
				settings->TIME_ZONE = std::make_shared<dt::Timezone>(
					dt::Timedelta(
						timezone_offset["days"].as<long>(0),
						timezone_offset["seconds"].as<long>(0),
						timezone_offset["microseconds"].as<long>(0),
						timezone_offset["milliseconds"].as<long>(0),
						timezone_offset["minutes"].as<long>(0),
						timezone_offset["hours"].as<long>(0),
						timezone_offset["weeks"].as<long>(0)
					),
					timezone_name
				);
			}
			else
			{
				settings->TIME_ZONE = std::make_shared<dt::Timezone>(
					dt::Timedelta(), timezone_name
				);
			}
		}
	}

	settings->USE_TZ = config["use_tz"].as<bool>(false);

	settings->CHARSET = config["charset"].as<std::string>("utf-8");

	settings->APPEND_SLASH = config["append_slash"].as<bool>(true);

	auto disallowed_user_agents = config["disallowed_user_agents"];
	if (disallowed_user_agents && disallowed_user_agents.IsSequence())
	{
		this->init_disallowed_user_agents(settings, disallowed_user_agents);
	}

	auto ignorable_404_urls = config["ignorable_404_urls"];
	if (ignorable_404_urls && ignorable_404_urls.IsSequence())
	{
		this->init_ignorable_404_urls(settings, ignorable_404_urls);
	}

	settings->SECRET_KEY = config["secret_key"].as<std::string>("");

	auto media = config["media"];
	if (media && media.IsMap())
	{
		auto p = media["root"].as<std::string>("media");
		settings->MEDIA_ROOT = path::is_absolute(p) ? p : path::join(settings->BASE_DIR, p);
		settings->MEDIA_URL = media["url"].as<std::string>("/media/");
	}

	auto static_ = config["static"];
	if (static_ && static_.IsMap())
	{
		auto p = static_["root"].as<std::string>("static");
		settings->STATIC_ROOT = path::is_absolute(p) ? p : path::join(settings->BASE_DIR, p);
		settings->STATIC_URL = static_["url"].as<std::string>("/static/");
	}

	settings->FILE_UPLOAD_MAX_MEMORY_SIZE = config["file_upload_max_memory_size"].as<int>(2621440);
	settings->DATA_UPLOAD_MAX_MEMORY_SIZE = config["data_upload_max_memory_size"].as<int>(2621440);

	settings->PREPEND_WWW = config["prepend_www"].as<bool>(false);

	settings->DATA_UPLOAD_MAX_NUMBER_FIELDS = config["data_upload_max_number_fields"].as<int>(1000);

	this->init_formats(settings, config);

	settings->FIRST_DAY_OF_WEEK = config["first_day_of_week"].as<int>(0);

	settings->DECIMAL_SEPARATOR = config["decimal_separator"].as<char>('.');

	settings->USE_THOUSAND_SEPARATOR = config["use_thousand_separator"].as<bool>(false);

	settings->THOUSAND_SEPARATOR = config["thousand_separator"].as<char>(',');

	settings->X_FRAME_OPTIONS = config["x_frame_options"].as<std::string>("SAMEORIGIN");

	settings->USE_X_FORWARDED_HOST = config["use_x_forwarded_host"].as<bool>(false);

	settings->USE_X_FORWARDED_PORT = config["use_x_forwarded_port"].as<bool>(false);

	this->init_csrf(settings, config);

	settings->USE_SSL = config["use_ssl"].as<bool>(false);

	this->init_secure(settings, config);

	auto apps = config["installed_apps"];
	if (apps && apps.IsSequence() && apps.size() > 0)
	{
		settings->register_apps();
		this->init_apps(settings, apps);
	}

	auto middleware = config["middleware"];
	if (middleware && middleware.IsSequence() && middleware.size() > 0)
	{
		settings->register_middleware();
		this->init_middleware(settings, middleware);
	}

	auto template_engine = config["template_engine"];
	if (template_engine && template_engine.IsMap())
	{
		this->init_template_engine(settings, template_engine);
	}
}

void YamlSettingsLoader::init_logger(Settings* settings, const YAML::Node& config)
{
	core::LoggerConfig logger_config{};
	auto levels = config["levels"];
	if (levels && levels.IsMap())
	{
		logger_config.enable_info = levels["info"].as<bool>(true);
		logger_config.enable_debug = levels["debug"].as<bool>(true);
		logger_config.enable_warning = levels["warning"].as<bool>(true);
		logger_config.enable_error = levels["error"].as<bool>(true);
		logger_config.enable_fatal = levels["fatal"].as<bool>(true);
		logger_config.enable_trace = levels["trace"].as<bool>(true);
		logger_config.enable_print = levels["print"].as<bool>(true);
	}

	auto out = config["out"];
	if (out && out.IsMap())
	{
		if (out["console"].as<bool>(true))
		{
			logger_config.add_console_stream();
		}

		auto file_out = out["file"];
		if (file_out && file_out.IsScalar())
		{
			auto f_path = file_out.as<std::string>();
			auto full_path = path::is_absolute(f_path) ? f_path : path::join(settings->BASE_DIR, f_path);
			logger_config.add_file_stream(full_path);
		}
		else
		{
			auto files_out = out["files"];
			if (files_out && files_out.IsSequence())
			{
				for (auto it = files_out.begin(); it != files_out.end(); it++)
				{
					if (!it->IsNull() && it->IsScalar())
					{
						auto f_path = it->as<std::string>();
						auto full_path = path::is_absolute(f_path) ? f_path : path::join(settings->BASE_DIR, f_path);
						logger_config.add_file_stream(full_path);
					}
				}
			}
		}
	}

	settings->LOGGER = core::Logger::get_instance(logger_config);
}

void YamlSettingsLoader::init_allowed_hosts(Settings* settings, const YAML::Node& allowed_hosts)
{
	for (auto it = allowed_hosts.begin(); it != allowed_hosts.end(); it++)
	{
		if (!it->IsNull() && it->IsScalar())
		{
			settings->ALLOWED_HOSTS.push_back(it->as<std::string>());
		}
	}
}

void YamlSettingsLoader::init_disallowed_user_agents(Settings* settings, const YAML::Node& agents)
{
	for (auto it = agents.begin(); it != agents.end(); it++)
	{
		if (!it->IsNull() && it->IsScalar())
		{
			settings->DISALLOWED_USER_AGENTS.emplace_back(it->as<std::string>());
		}
	}
}

void YamlSettingsLoader::init_ignorable_404_urls(Settings* settings, const YAML::Node& urls)
{
	for (auto it = urls.begin(); it != urls.end(); it++)
	{
		if (!it->IsNull() && it->IsScalar())
		{
			settings->IGNORABLE_404_URLS.emplace_back(it->as<std::string>());
		}
	}
}

void YamlSettingsLoader::init_formats(Settings* settings, const YAML::Node& config)
{
	YAML::Node formats;
	if (config["formats"])
	{
		formats = config["formats"];
	}

	if (formats.IsMap())
	{
		settings->DATE_FORMAT = formats["date"].as<std::string>("%b %d, %Y");
		settings->DATETIME_FORMAT = formats["datetime"].as<std::string>("%b %d, %Y, %T");
		settings->TIME_FORMAT = formats["time"].as<std::string>("%T");
		settings->YEAR_MONTH_FORMAT = formats["year_month"].as<std::string>("%B %Y");
		settings->MONTH_DAY_FORMAT = formats["month_day"].as<std::string>("%B %d");
		settings->SHORT_DATE_FORMAT = formats["short_date"].as<std::string>("%m/%d/%Y");
		settings->SHORT_DATETIME_FORMAT = formats["short_datetime"].as<std::string>("%m/%d/%Y %T");
	}
}

void YamlSettingsLoader::init_csrf(Settings* settings, const YAML::Node& config)
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

	settings->CSRF_COOKIE_NAME = cookie["name"].as<std::string>("csrftoken");
	settings->CSRF_COOKIE_AGE = cookie["age"].as<size_t>(60 * 60 * 24 * 7 * 52);
	settings->CSRF_COOKIE_DOMAIN = cookie["domain"].as<std::string>("");
	settings->CSRF_COOKIE_PATH = cookie["path"].as<std::string>("/");
	settings->CSRF_COOKIE_SECURE = cookie["secure"].as<bool>(false);
	settings->CSRF_COOKIE_HTTP_ONLY = cookie["http_only"].as<bool>(false);
	settings->CSRF_COOKIE_SAME_SITE = cookie["same_site"].as<std::string>("Lax");

	settings->CSRF_HEADER_NAME = csrf["header_name"].as<std::string>("X-XSRF-TOKEN");
	settings->CSRF_USE_SESSIONS = csrf["use_sessions"].as<bool>(false);

	auto origins = csrf["trusted_origins"];
	if (origins && origins.IsSequence())
	{
		for (auto it = origins.begin(); it != origins.end(); it++)
		{
			if (!it->IsNull() && it->IsScalar())
			{
				settings->CSRF_TRUSTED_ORIGINS.push_back(it->as<std::string>());
			}
		}
	}
}

void YamlSettingsLoader::init_secure(Settings* settings, const YAML::Node& config)
{
	YAML::Node secure;
	if (config["secure"])
	{
		secure = config["secure"];
	}

	settings->SECURE_BROWSER_XSS_FILTER = secure["browser_xss_filter"].as<bool>(false);
	settings->SECURE_CONTENT_TYPE_NO_SNIFF = secure["content_type_no_sniff"].as<bool>(true);
	settings->SECURE_HSTS_INCLUDE_SUBDOMAINS = secure["hsts_include_subdomains"].as<bool>(false);
	settings->SECURE_HSTS_PRELOAD = secure["hsts_preload"].as<bool>(false);
	settings->SECURE_HSTS_SECONDS = secure["hsts_seconds"].as<size_t>(0);
	auto redirect_exempt = secure["redirect_exempt"];
	if (redirect_exempt && redirect_exempt.IsSequence())
	{
		for (auto it = redirect_exempt.begin(); it != redirect_exempt.end(); it++)
		{
			if (!it->IsNull() && it->IsScalar())
			{
				settings->SECURE_REDIRECT_EXEMPT.push_back(it->as<std::string>());
			}
		}
	}

	settings->SECURE_REFERRER_POLICY = secure["referrer_policy"].as<std::string>("");
	settings->SECURE_SSL_HOST = secure["ssl_host"].as<std::string>("");
	settings->SECURE_SSL_REDIRECT = secure["ssl_redirect"].as<bool>(false);
	auto psslh = secure["proxy_ssl_header"];
	if (psslh && psslh.IsMap())
	{
		settings->SECURE_PROXY_SSL_HEADER = std::make_shared<std::pair<std::string, std::string>>(
			psslh["name"].as<std::string>("X-Forwarded-Proto"),
			psslh["value"].as<std::string>("https")
		);
	}
}

void YamlSettingsLoader::init_apps(Settings* settings, const YAML::Node& apps)
{
	for (auto it = apps.begin(); it != apps.end(); it++)
	{
		if (!it->IsNull() && it->IsScalar())
		{
			auto item = settings->get_app(it->as<std::string>());
			if (item)
			{
				settings->INSTALLED_APPS.push_back(item);
			}
		}
	}
}

void YamlSettingsLoader::init_middleware(Settings* settings, const YAML::Node& middleware)
{
	for (auto it = middleware.begin(); it != middleware.end(); it++)
	{
		if (!it->IsNull() && it->IsScalar())
		{
			auto item = settings->get_middleware(it->as<std::string>());
			if (item)
			{
				settings->MIDDLEWARE.push_back(item);
			}
		}
	}
}

void YamlSettingsLoader::init_template_engine(Settings* settings, const YAML::Node& config)
{
	std::vector<std::string> dirs;
	auto directories = config["directories"];
	if (directories && directories.IsSequence())
	{
		for (auto it = directories.begin(); it != directories.end(); it++)
		{
			if (!it->IsNull() && it->IsScalar())
			{
				auto p = it->as<std::string>();
				dirs.push_back(
					path::is_absolute(p) ? p : path::join(settings->BASE_DIR, p)
				);
			}
		}
	}

	std::vector<std::shared_ptr<render::lib::ILibrary>> libs{
		settings->get_library(render::lib::DefaultLibrary::FULL_NAME)
	};
	auto libraries = config["libraries"];
	if (libraries && libraries.IsSequence() && libraries.size() > 0)
	{
		settings->register_libraries();
		for (auto it = libraries.begin(); it != libraries.end(); it++)
		{
			if (!it->IsNull() && it->IsScalar())
			{
				libs.push_back(settings->get_library(it->as<std::string>()));
			}
		}
	}

	std::vector<std::shared_ptr<render::ILoader>> loaders_vec;
	auto loaders = config["loaders"];
	if (loaders && loaders.IsSequence() && loaders.size() > 0)
	{
		settings->register_loaders();
		for (auto it = loaders.begin(); it != loaders.end(); it++)
		{
			if (!it->IsNull() && it->IsScalar())
			{
				loaders_vec.push_back(settings->get_loader(it->as<std::string>()));
			}
		}
	}

	auto use_app_dirs = config["use_app_directories"];
	if (use_app_dirs && use_app_dirs.IsScalar() && use_app_dirs.as<bool>())
	{
		for (const auto& app : settings->INSTALLED_APPS)
		{
			dirs.push_back(path::dirname(app->get_app_path()));
		}
	}

	auto auto_escape = config["auto_escape"];
	settings->TEMPLATE_ENGINE = std::make_unique<render::DefaultEngine>(
		dirs,
		settings->DEBUG,
		auto_escape && auto_escape.as<bool>(),
		loaders_vec,
		libs,
		settings->LOGGER.get()
	);
}

__CONF_END__
