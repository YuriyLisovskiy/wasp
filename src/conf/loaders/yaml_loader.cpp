/**
 * conf/loaders/yaml_loader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./yaml_loader.h"

// Base libraries.
//#include <xalwart.base/path.h>

// Render libraries.
//#include <xalwart.render/engine.h>

// Framework libraries.
//#include "./components/default.h"
//#include "./components/logger.h"
//#include "./components/timezone.h"
//
//#include "./utilities.h"
//
//#include "../../../render/default_library.h"


__CONF_BEGIN__

/*oid YAMLSettingsLoader::check_config(const YAML::Node& config, const std::string& file_path)
{
	if (config && config.IsMap())
	{
		return;
	}

	throw ValueError("'" + file_path + "' file must have map type", _ERROR_DETAILS_);
}*/
/*
void YAMLSettingsLoader::overwrite_scalar_or_remove_if_null(
	YAML::Node& config, const YAML::Node& local_config, const std::string& key
)
{
	auto value = local_config[key];
	if (!value || value.IsNull())
	{
		config[key] = null_node();
	}
	else if (value.IsScalar())
	{
		config[key] = value;
	}
}

void YAMLSettingsLoader::overwrite_sequence_or_remove_if_null(
	YAML::Node& config, const YAML::Node& local_config, const std::string& key
)
{
	auto value = local_config[key];
	if (!value || value.IsNull())
	{
		config[key] = null_node();
	}
	else if (value.IsSequence())
	{
		config[key] = value;
	}
}

void YAMLSettingsLoader::overwrite_template_engine(
	YAML::Node& template_engine, const YAML::Node& local_template_engine
)
{
	if (local_template_engine.IsNull())
	{
		template_engine = null_node();
	}
	else if (local_template_engine.IsMap())
	{
		if (!template_engine)
		{
			template_engine = map_node();
		}

		this->overwrite_sequence_or_remove_if_null(
			template_engine, local_template_engine, "directories"
		);
		this->overwrite_sequence_or_remove_if_null(
			template_engine, local_template_engine, "libraries"
		);
		this->overwrite_sequence_or_remove_if_null(
			template_engine, local_template_engine, "loaders"
		);
		this->overwrite_scalar_or_remove_if_null(
			template_engine, local_template_engine, "use_module_directories"
		);
		this->overwrite_scalar_or_remove_if_null(
			template_engine, local_template_engine, "auto_escape"
		);
	}
}

void YAMLSettingsLoader::init_template_engine_setting(Settings* settings, const YAML::Node& config)
{
	std::vector<std::string> dirs;
	auto directories = config["directories"];
	if (directories && directories.IsSequence())
	{
		for (auto it = directories.begin(); it != directories.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				auto p = it->as<std::string>();
				dirs.push_back(
					path::is_absolute(p) ? p : path::join(settings->BASE_DIR, p)
				);
			}
		}
	}

	std::vector<std::shared_ptr<render::abc::ILibrary>> libs{
		settings->build_library(render::DefaultLibrary::FULL_NAME)
	};
	auto libraries = config["libraries"];
	if (libraries && libraries.IsSequence() && libraries.size() > 0)
	{
		settings->register_libraries();
		for (auto it = libraries.begin(); it != libraries.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				libs.push_back(settings->build_library(it->as<std::string>()));
			}
		}
	}

	std::vector<std::shared_ptr<render::abc::ILoader>> loaders_vec;
	auto loaders = config["loaders"];
	if (loaders && loaders.IsSequence() && loaders.size() > 0)
	{
		settings->register_loaders();
		for (auto it = loaders.begin(); it != loaders.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				loaders_vec.push_back(settings->build_loader(it->as<std::string>()));
			}
		}
	}

	auto auto_escape = config["auto_escape"];
	settings->TEMPLATE_ENGINE = std::make_unique<render::DefaultEngine>(
		dirs,
		settings->DEBUG,
		auto_escape && !auto_escape.IsNull() && auto_escape.as<bool>(false),
		loaders_vec,
		libs,
		settings->LOGGER.get()
	);
}*/
/*
YAML::Node YAMLSettingsLoader::load_file(const std::string& base_dir, const std::regex& file_name_pattern)
{
	auto file_entry = this->find_file(base_dir, file_name_pattern);
	if (file_entry.exists())
	{
		auto config = YAML::LoadFile(file_entry.path());
		this->check_config(config, file_entry.path());
		return config;
	}

	return null_node();
}*/

/*void YAMLSettingsLoader::overwrite_config(YAML::Node& config, const YAML::Node& local_config)
{
	if (!local_config || local_config.IsNull())
	{
		return;
	}

	auto local_debug = local_config["debug"];
	if (local_debug && (local_debug.IsNull() || local_debug.IsScalar()))
	{
		config["debug"] = local_debug;
	}

	auto local_logger = local_config["logger"];
	if (local_logger)
	{
		auto logger = config["logger"];
		this->overwrite_logger(logger, local_logger);
		config["logger"] = logger;
	}

	auto local_allowed_hosts = local_config["allowed_hosts"];
	if (local_allowed_hosts && (local_allowed_hosts.IsNull() || local_allowed_hosts.IsSequence()))
	{
		config["allowed_hosts"] = local_allowed_hosts;
	}

	auto local_timezone = local_config["timezone"];
	if (!local_timezone)
	{
		// skip
	}
	else if (local_timezone.IsNull())
	{
		config["timezone"] = YamlSettingsLoader::null();
	}
	else if (local_timezone.IsMap())
	{
		auto timezone = config["timezone"];
		if (timezone && timezone.IsNull())
		{
			config["timezone"] = local_timezone;
		}
		else if (local_timezone.IsMap())
		{
			if (!timezone)
			{
				timezone = YamlSettingsLoader::map_node();
			}

			this->overwrite_scalar_or_remove_if_null(timezone, local_timezone, "name");
			auto local_offset = local_timezone["offset"];
			if (!local_offset)
			{
				// skip
			}
			else if (local_offset.IsNull())
			{
				timezone["offset"] = YamlSettingsLoader::null();
			}
			else if (local_offset.IsMap())
			{
				auto offset = timezone["offset"];
				if (offset && offset.IsNull())
				{
					timezone["offset"] = local_offset;
				}
				else
				{
					if (!offset)
					{
						offset = YamlSettingsLoader::map_node();
					}

					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "days");
					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "seconds");
					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "microseconds");
					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "milliseconds");
					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "minutes");
					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "hours");
					this->overwrite_scalar_or_remove_if_null(offset, local_offset, "weeks");
					timezone["offset"] = offset;
				}
			}

			config["timezone"] = timezone;
		}
	}

	auto local_use_tz = local_config["use_tz"];
	if (local_use_tz && (local_use_tz.IsNull() || local_use_tz.IsScalar()))
	{
		config["use_tz"] = local_use_tz;
	}

	auto local_charset = local_config["charset"];
	if (local_charset && (local_charset.IsNull() || local_charset.IsScalar()))
	{
		config["charset"] = local_charset;
	}

	auto local_append_slash = local_config["append_slash"];
	if (local_append_slash && (local_append_slash.IsNull() || local_append_slash.IsScalar()))
	{
		config["append_slash"] = local_append_slash;
	}

	auto local_disallowed_user_agents = local_config["disallowed_user_agents"];
	if (local_disallowed_user_agents && (
		local_disallowed_user_agents.IsNull() || local_disallowed_user_agents.IsSequence()
	))
	{
		config["disallowed_user_agents"] = local_disallowed_user_agents;
	}

	auto local_ignorable_404_urls = local_config["ignorable_404_urls"];
	if (local_ignorable_404_urls && (
		local_ignorable_404_urls.IsNull() || local_ignorable_404_urls.IsSequence()
	))
	{
		config["ignorable_404_urls"] = local_ignorable_404_urls;
	}

	auto local_secret_key = local_config["secret_key"];
	if (local_secret_key && (local_secret_key.IsNull() || local_secret_key.IsScalar()))
	{
		config["secret_key"] = local_secret_key;
	}

	auto local_media = local_config["media"];
	if (!local_media)
	{
		// skip
	}
	else if (local_media.IsNull())
	{
		config["media"] = YamlSettingsLoader::null();
	}
	else if (local_media.IsMap())
	{
		auto media = config["media"];
		if (media && media.IsNull())
		{
			config["media"] = local_media;
		}
		else
		{
			if (!media)
			{
				media = YamlSettingsLoader::map_node();
			}

			this->overwrite_scalar_or_remove_if_null(media, local_media, "root");
			this->overwrite_scalar_or_remove_if_null(media, local_media, "url");
			config["media"] = media;
		}
	}

	auto local_static = local_config["static"];
	if (!local_static)
	{
		// skip
	}
	else if (local_static.IsNull())
	{
		config["static"] = YamlSettingsLoader::null();
	}
	else if (local_static.IsMap())
	{
		auto static_ = config["static"];
		if (static_ && static_.IsNull())
		{
			config["static"] = local_static;
		}
		else
		{
			if (!static_)
			{
				static_ = YamlSettingsLoader::map_node();
			}

			this->overwrite_scalar_or_remove_if_null(static_, local_static, "root");
			this->overwrite_scalar_or_remove_if_null(static_, local_static, "url");
			config["static"] = static_;
		}
	}

	// TODO: mot working
	auto local_file_upload_max_memory_size = local_config["file_upload_max_memory_size"];
	if (local_file_upload_max_memory_size && (
		local_file_upload_max_memory_size.IsNull() || local_file_upload_max_memory_size.IsScalar()
	))
	{
		config["file_upload_max_memory_size"] = local_file_upload_max_memory_size;
	}

	// TODO: mot working
	auto local_data_upload_max_memory_size = local_config["data_upload_max_memory_size"];
	if (local_data_upload_max_memory_size && (
		local_data_upload_max_memory_size.IsNull() || local_data_upload_max_memory_size.IsScalar()
	))
	{
		config["data_upload_max_memory_size"] = local_data_upload_max_memory_size;
	}

	auto local_prepend_www = local_config["prepend_www"];
	if (local_prepend_www && (local_prepend_www.IsNull() || local_prepend_www.IsScalar()))
	{
		config["prepend_www"] = local_prepend_www;
	}

	// TODO: mot working
	auto local_data_upload_max_number_fields = local_config["data_upload_max_number_fields"];
	if (local_data_upload_max_number_fields && (
		local_data_upload_max_number_fields.IsNull() || local_data_upload_max_number_fields.IsScalar()
	))
	{
		config["data_upload_max_number_fields"] = local_data_upload_max_number_fields;
	}

	auto local_formats = local_config["formats"];
	if (!local_formats)
	{
		// skip
	}
	else if (local_formats.IsNull())
	{
		config["formats"] = YamlSettingsLoader::null();
	}
	else if (local_formats.IsMap())
	{
		auto formats = config["formats"];
		if (formats && formats.IsNull())
		{
			config["formats"] = local_formats;
		}
		else
		{
			if (!formats)
			{
				formats = YamlSettingsLoader::map_node();
			}

			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "date");
			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "datetime");
			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "time");
			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "year_month");
			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "month_day");
			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "short_date");
			this->overwrite_scalar_or_remove_if_null(formats, local_formats, "short_datetime");
			config["formats"] = formats;
		}
	}

	auto local_first_day_of_week = local_config["first_day_of_week"];
	if (local_first_day_of_week && (
		local_first_day_of_week.IsNull() || local_first_day_of_week.IsScalar()
	))
	{
		config["first_day_of_week"] = local_first_day_of_week;
	}

	auto local_decimal_separator = local_config["decimal_separator"];
	if (local_decimal_separator && (
		local_decimal_separator.IsNull() || local_decimal_separator.IsScalar()
	))
	{
		config["decimal_separator"] = local_decimal_separator;
	}

	auto local_use_thousand_separator = local_config["use_thousand_separator"];
	if (local_use_thousand_separator && (
		local_use_thousand_separator.IsNull() || local_use_thousand_separator.IsScalar()
	))
	{
		config["use_thousand_separator"] = local_use_thousand_separator;
	}

	auto local_thousand_separator = local_config["thousand_separator"];
	if (local_thousand_separator && (
		local_thousand_separator.IsNull() || local_thousand_separator.IsScalar()
	))
	{
		config["thousand_separator"] = local_thousand_separator;
	}

	auto local_x_frame_options = local_config["x_frame_options"];
	if (local_x_frame_options && (
		local_x_frame_options.IsNull() || local_x_frame_options.IsScalar()
	))
	{
		config["x_frame_options"] = local_x_frame_options;
	}

	auto local_use_x_forwarded_host = local_config["use_x_forwarded_host"];
	if (local_use_x_forwarded_host && (
		local_use_x_forwarded_host.IsNull() || local_use_x_forwarded_host.IsScalar()
	))
	{
		config["use_x_forwarded_host"] = local_use_x_forwarded_host;
	}

	auto local_use_x_forwarded_port = local_config["use_x_forwarded_port"];
	if (local_use_x_forwarded_port && (
		local_use_x_forwarded_port.IsNull() || local_use_x_forwarded_port.IsScalar()
	))
	{
		config["use_x_forwarded_port"] = local_use_x_forwarded_port;
	}

	auto local_csrf = local_config["csrf"];
	if (!local_csrf)
	{
		// skip
	}
	else if (local_csrf.IsNull())
	{
		config["csrf"] = YamlSettingsLoader::null();
	}
	else if (local_csrf.IsMap())
	{
		auto csrf = config["csrf"];
		if (csrf && csrf.IsNull())
		{
			config["csrf"] = local_csrf;
		}
		else
		{
			if (!csrf)
			{
				csrf = YamlSettingsLoader::map_node();
			}

			auto local_cookie = local_csrf["cookie"];
			if (!local_cookie)
			{
				// skip
			}
			else if (local_cookie.IsNull())
			{
				csrf["cookie"] = YamlSettingsLoader::null();
			}
			else if (local_cookie.IsMap())
			{
				auto cookie = csrf["cookie"];
				if (cookie && cookie.IsNull())
				{
					csrf["cookie"] = local_cookie;
				}
				else
				{
					if (!cookie)
					{
						cookie = YamlSettingsLoader::map_node();
					}

					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "name");
					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "age");
					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "domain");
					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "path");
					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "secure");
					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "http_only");
					this->overwrite_scalar_or_remove_if_null(cookie, local_cookie, "same_site");
					csrf["cookie"] = cookie;
				}
			}

			this->overwrite_scalar_or_remove_if_null(csrf, local_csrf, "header_name");
			this->overwrite_scalar_or_remove_if_null(csrf, local_csrf, "use_sessions");
			this->overwrite_sequence_or_remove_if_null(csrf, local_csrf, "trusted_origins");
			config["csrf"] = csrf;
		}
	}

	auto local_use_ssl = local_config["use_ssl"];
	if (local_use_ssl && (local_use_ssl.IsNull() || local_use_ssl.IsScalar()))
	{
		config["use_ssl"] = local_use_ssl;
	}

	auto local_secure = local_config["secure"];
	if (!local_secure)
	{
		// skip
	}
	else if (local_secure.IsNull())
	{
		config["secure"] = YamlSettingsLoader::null();
	}
	else if (local_secure.IsMap())
	{
		auto secure = config["secure"];
		if (secure && secure.IsNull())
		{
			config["secure"] = local_secure;
		}
		else
		{
			if (!secure)
			{
				secure = YamlSettingsLoader::map_node();
			}

			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "browser_xss_filter");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "content_type_no_sniff");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "hsts_include_subdomains");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "hsts_preload");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "hsts_seconds");
			this->overwrite_sequence_or_remove_if_null(secure, local_secure, "redirect_exempt");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "referrer_policy");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "ssl_host");
			this->overwrite_scalar_or_remove_if_null(secure, local_secure, "ssl_redirect");
			auto local_psslh = local_secure["proxy_ssl_header"];
			if (!local_psslh)
			{
				// skip
			}
			else if (local_psslh.IsNull())
			{
				secure["proxy_ssl_header"] = YamlSettingsLoader::null();
			}
			else if (local_psslh.IsMap())
			{
				auto psslh = secure["proxy_ssl_header"];
				if (psslh && psslh.IsNull())
				{
					secure["proxy_ssl_header"] = local_psslh;
				}
				else
				{
					if (!psslh)
					{
						psslh = YamlSettingsLoader::map_node();
					}

					this->overwrite_scalar_or_remove_if_null(psslh, local_psslh, "name");
					this->overwrite_scalar_or_remove_if_null(psslh, local_psslh, "value");
					secure["proxy_ssl_header"] = psslh;
				}
			}

			config["secure"] = secure;
		}
	}

	auto local_modules = local_config["modules"];
	if (local_modules && (
		local_modules.IsNull() || local_modules.IsSequence()
	))
	{
		config["modules"] = local_modules;
	}

	auto local_middleware = local_config["middleware"];
	if (local_middleware && (
		local_middleware.IsNull() || local_middleware.IsSequence()
	))
	{
		config["middleware"] = local_middleware;
	}

	auto local_databases = local_config["databases"];
	if (local_databases && local_databases.IsSequence())
	{
		config["databases"] = local_databases;
	}

	auto local_template_engine = local_config["template_engine"];
	if (local_template_engine)
	{
		auto template_engine = config["template_engine"];
		this->overwrite_template_engine(template_engine, local_template_engine);
		config["template_engine"] = template_engine;
	}
}*/

/*void YAMLSettingsLoader::init_settings(Settings* settings, const YAML::Node& config)
{
//	auto debug_mode = config["debug"];
//	settings->DEBUG = !debug_mode.IsNull() && debug_mode.as<bool>(false);

//	auto logger = config["logger"];
//	if (logger && logger.IsMap())
//	{
//		this->init_logger_setting(settings, logger);
//	}

//	auto allowed_hosts = config["allowed_hosts"];
//	if (allowed_hosts && allowed_hosts.IsSequence())
//	{
//		_init_allowed_hosts(settings, allowed_hosts);
//	}

//	settings->USE_TIMEZONE = config["use_timezone"].as<bool>(false);

//	auto timezone = config["timezone"];
//	if (!timezone || !timezone.IsMap())
//	{
//		settings->TIMEZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);
//	}
//	else
//	{
//		auto timezone_name = str::to_upper(timezone["name"].as<std::string>("UTC"));
//		if (timezone_name == "UTC")
//		{
//			settings->TIMEZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);
//		}
//		else
//		{
//			auto timezone_offset = timezone["offset"];
//			if (timezone_offset && timezone_offset.IsMap())
//			{
//				settings->TIMEZONE = std::make_shared<dt::Timezone>(
//					dt::Timedelta(
//						timezone_offset["days"].as<long>(0),
//						timezone_offset["seconds"].as<long>(0),
//						timezone_offset["microseconds"].as<long>(0),
//						timezone_offset["milliseconds"].as<long>(0),
//						timezone_offset["minutes"].as<long>(0),
//						timezone_offset["hours"].as<long>(0),
//						timezone_offset["weeks"].as<long>(0)
//					),
//					timezone_name
//				);
//			}
//			else
//			{
//				settings->TIMEZONE = std::make_shared<dt::Timezone>(
//					dt::Timedelta(), timezone_name
//				);
//			}
//		}
//	}

//	settings->CHARSET = config["charset"].as<std::string>("utf-8");

//	auto append_slash = config["append_slash"];
//	settings->APPEND_SLASH = !append_slash || (!append_slash.IsNull() && append_slash.as<bool>(true));

//	auto disallowed_user_agents = config["disallowed_user_agents"];
//	if (disallowed_user_agents && disallowed_user_agents.IsSequence())
//	{
//		_init_disallowed_user_agents(settings, disallowed_user_agents);
//	}

//	auto ignorable_404_urls = config["ignorable_404_urls"];
//	if (ignorable_404_urls && ignorable_404_urls.IsSequence())
//	{
//		_init_ignorable_404_urls(settings, ignorable_404_urls);
//	}

//	settings->SECRET_KEY = config["secret_key"].as<std::string>("");

//	auto media = config["media"];
//	if (media && media.IsMap())
//	{
//		auto p = media["root"].as<std::string>("");
//		settings->MEDIA.ROOT = path::is_absolute(p) ? p : path::join(settings->BASE_DIR, p);
//		settings->MEDIA.URL = media["url"].as<std::string>("");
//	}
//
//	auto static_ = config["static"];
//	if (static_ && static_.IsMap())
//	{
//		auto p = static_["root"].as<std::string>("");
//		settings->STATIC.ROOT = path::is_absolute(p) ? p : path::join(settings->BASE_DIR, p);
//		settings->STATIC.URL = static_["url"].as<std::string>("");
//	}

//	auto limits = config["limits"];
//	if (limits && limits.IsMap())
//	{
//		settings->LIMITS = {
//			.FILE_UPLOAD_MAX_MEMORY_SIZE = limits["file_upload_max_memory_size"].as<size_t>(2621440),
//			.DATA_UPLOAD_MAX_MEMORY_SIZE = limits["data_upload_max_memory_size"].as<size_t>(2621440),
//			.DATA_UPLOAD_MAX_NUMBER_FIELDS = limits["data_upload_max_number_fields"].as<size_t>(1000),
//			.MAX_HEADER_LENGTH = limits["max_header_length"].as<size_t>(65535),
//			.MAX_HEADERS_COUNT = limits["max_headers_count"].as<size_t>(100)
//		};
//	}

//	settings->PREPEND_WWW = config["prepend_www"].as<bool>(false);
//
//	auto formats = config["formats"];
//	if (formats && formats.IsMap())
//	{
//		_init_formats(settings, formats);
//	}
//
//	settings->FIRST_DAY_OF_WEEK = config["first_day_of_week"].as<int>(0);
//
//	settings->DECIMAL_SEPARATOR = config["decimal_separator"].as<char>('.');
//
//	settings->USE_THOUSAND_SEPARATOR = config["use_thousand_separator"].as<bool>(false);
//
//	settings->THOUSAND_SEPARATOR = config["thousand_separator"].as<char>(',');
//
//	settings->X_FRAME_OPTIONS = XFrameOptions::from_string(config["x_frame_options"].as<std::string>("SAMEORIGIN"));
//
//	settings->USE_X_FORWARDED_HOST = config["use_x_forwarded_host"].as<bool>(false);
//
//	settings->USE_X_FORWARDED_PORT = config["use_x_forwarded_port"].as<bool>(false);
//
//	auto csrf = config["csrf"];
//	if (csrf && csrf.IsMap())
//	{
//		_init_csrf(settings, csrf);
//	}
//
//	settings->USE_SSL = config["use_ssl"].as<bool>(false);
//
//	auto secure = config["secure"];
//	if (secure && secure.IsMap())
//	{
//		_init_secure(settings, secure);
//	}
//
//	auto modules = config["modules"];
//	if (modules && modules.IsSequence() && modules.size() > 0)
//	{
//		settings->register_modules();
//		_init_modules(settings, modules);
//	}
//
//	auto middleware = config["middleware"];
//	if (middleware && middleware.IsSequence() && middleware.size() > 0)
//	{
//		settings->register_middleware();
//		_init_middleware(settings, middleware);
//	}

	auto databases = config["databases"];
	if (databases && databases.IsSequence() && databases.size() > 0)
	{
		_init_databases(settings, databases);
	}
	else
	{
		settings->LOGGER->warning("databases: databases were not detected", _ERROR_DETAILS_);
	}

	auto template_engine = config["template_engine"];
	if (template_engine && template_engine.IsMap())
	{
		this->init_template_engine_setting(settings, template_engine);
	}
}*/
/*
void YAMLSettingsLoader::_init_formats(Settings* settings, const YAML::Node& formats)
{
	settings->FORMATS = {
		.DATE_FORMAT = formats["date"].as<std::string>("%b %d, %Y"),
		.DATETIME_FORMAT = formats["datetime"].as<std::string>("%b %d, %Y, %T"),
		.TIME_FORMAT = formats["time"].as<std::string>("%T"),
		.YEAR_MONTH_FORMAT = formats["year_month"].as<std::string>("%B %Y"),
		.MONTH_DAY_FORMAT = formats["month_day"].as<std::string>("%B %d"),
		.SHORT_DATE_FORMAT = formats["short_date"].as<std::string>("%m/%d/%Y"),
		.SHORT_DATETIME_FORMAT = formats["short_datetime"].as<std::string>("%m/%d/%Y %T")
	};
}

void YAMLSettingsLoader::_init_csrf(Settings* settings, const YAML::Node& csrf)
{
	auto cookie = csrf["cookie"];
	settings->CSRF = {};
	if (cookie && cookie.IsMap())
	{
		settings->CSRF = {
			.COOKIE = {
				.NAME = cookie["name"].as<std::string>("csrftoken"),
				.AGE = cookie["age"].as<size_t>(60 * 60 * 24 * 7 * 52),
				.DOMAIN = cookie["domain"].as<std::string>(""),
				.PATH = cookie["path"].as<std::string>("/"),
				.SECURE = cookie["secure"].as<bool>(false),
				.HTTP_ONLY = cookie["http_only"].as<bool>(false),
				.SAME_SITE = cookie["same_site"].as<std::string>("Lax")
			}
		};
	}

	settings->CSRF.HEADER_NAME = csrf["header_name"].as<std::string>("X-XSRF-TOKEN");
	settings->CSRF.USE_SESSIONS = csrf["use_sessions"].as<bool>(false);

	auto origins = csrf["trusted_origins"];
	if (origins && origins.IsSequence())
	{
		for (auto it = origins.begin(); it != origins.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				auto value = it->as<std::string>();
				if (!value.empty())
				{
					settings->CSRF.TRUSTED_ORIGINS.push_back(value);
				}
			}
		}
	}
}

void YAMLSettingsLoader::_init_secure(Settings* settings, const YAML::Node& secure)
{
	auto content_type_no_sniff = secure["content_type_no_sniff"];
	settings->SECURE = {
		.BROWSER_XSS_FILTER = secure["browser_xss_filter"].as<bool>(false),
		.CONTENT_TYPE_NO_SNIFF = !content_type_no_sniff ||
			(!content_type_no_sniff.IsNull() && content_type_no_sniff.as<bool>(true)),
		.HSTS_INCLUDE_SUBDOMAINS = secure["hsts_include_subdomains"].as<bool>(false),
		.HSTS_PRELOAD = secure["hsts_preload"].as<bool>(false),
		.HSTS_SECONDS = secure["hsts_seconds"].as<size_t>(0),
		.REFERRER_POLICY = secure["referrer_policy"].as<std::string>(""),
		.SSL_HOST = secure["ssl_host"].as<std::string>(""),
		.SSL_REDIRECT = secure["ssl_redirect"].as<bool>(false)
	};

	auto redirect_exempt = secure["redirect_exempt"];
	if (redirect_exempt && redirect_exempt.IsSequence())
	{
		for (auto it = redirect_exempt.begin(); it != redirect_exempt.end(); it++)
		{
			if (it->IsDefined() && it->IsScalar())
			{
				auto value = it->as<std::string>();
				if (!value.empty())
				{
					settings->SECURE.REDIRECT_EXEMPT.push_back(value);
				}
			}
		}
	}

	auto proxy_ssl_header = secure["proxy_ssl_header"];
	if (proxy_ssl_header && proxy_ssl_header.IsMap())
	{
		settings->SECURE.PROXY_SSL_HEADER = std::pair<std::string, std::string>(
			proxy_ssl_header["name"].as<std::string>("X-Forwarded-Proto"),
			proxy_ssl_header["value"].as<std::string>("https")
		);
	}
}

void YAMLSettingsLoader::_init_modules(Settings* settings, const YAML::Node& modules)
{
	for (auto it = modules.begin(); it != modules.end(); it++)
	{
		if (it->IsDefined() && it->IsScalar())
		{
			auto item = settings->build_module(it->as<std::string>());
			if (item)
			{
				settings->MODULES.push_back(item);
			}
		}
	}
}

void YAMLSettingsLoader::_init_middleware(Settings* settings, const YAML::Node& middleware)
{
	for (auto it = middleware.begin(); it != middleware.end(); it++)
	{
		if (it->IsDefined() && it->IsScalar())
		{
			auto name = it->as<std::string>();
			auto item = settings->get_middleware_by_name(name);
			if (item)
			{
				settings->MIDDLEWARE.push_back(item);
			}
			else
			{
				if (settings->LOGGER)
				{
					settings->LOGGER->warning("Middleware not registered: '" + name + "'");
				}
			}
		}
	}
}

void YAMLSettingsLoader::_init_databases(Settings* settings, const YAML::Node& databases)
{
	for (auto it = databases.begin(); it != databases.end(); it++)
	{
		if (it->IsDefined() && it->IsMap())
		{
			auto db_info = *it;
			if (!db_info || !db_info.IsMap())
			{
				throw ImproperlyConfigured(
					"databases: parameter must be non-empty map", _ERROR_DETAILS_
				);
			}

			auto driver_node = db_info["driver"];
			if (!driver_node || !driver_node.IsScalar())
			{
				throw ImproperlyConfigured(
					"databases: the 'driver' parameter is required and must have a string type",
					_ERROR_DETAILS_
				);
			}

			auto name_node = db_info["name"];
			if (!name_node || !name_node.IsScalar())
			{
				throw ImproperlyConfigured(
					"databases: the 'name' parameter is required and must have a string type",
					_ERROR_DETAILS_
				);
			}

			auto db_name = name_node.as<std::string>();
			auto driver_name = driver_node.as<std::string>();
			std::shared_ptr<orm::abc::ISQLDriver> driver;
			if (driver_name == "sqlite3")
			{
				_init_sqlite3_database(db_name, settings, driver, db_info);
			}
			else
			{
				// TODO: add more driver initializations.
				driver = settings->build_database(db_name, db_info);
			}

			if (driver)
			{
				settings->DATABASES[db_name] = std::make_shared<orm::Client>(driver);
//				if (db_name == "default")
//				{
//					settings->DB = std::make_shared<orm::Client>(driver);
//				}
//				else
//				{
//					settings->DATABASES.push_back();
//				}
			}
		}
	}

	if (settings->DATABASES.find("default") != settings->DATABASES.end())
	{
		settings->DB = settings->DATABASES["default"];
	}
	else
	{
		throw ImproperlyConfigured(
			"databases: default database is required to be configured",
			_ERROR_DETAILS_
		);
	}
}

void YAMLSettingsLoader::_init_sqlite3_database(
	const std::string& name, Settings* settings,
	std::shared_ptr<orm::abc::ISQLDriver>& driver, const YAML::Node& database
)
{
	auto filepath = database["file"];
	if (!filepath || !filepath.IsScalar())
	{
		throw ImproperlyConfigured(
			"databases: the 'file' parameter of SQLite3 database info is required and must have a string type",
			_ERROR_DETAILS_
		);
	}

	driver = settings->build_sqlite3_database(name, filepath.as<std::string>());
}*/

__CONF_END__
