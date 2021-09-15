/**
 * conf/loaders/yaml_loader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./yaml_loader.h"


__CONF_BEGIN__

/*void YAMLSettingsLoader::overwrite_template_engine(
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
}*/

/*void YAMLSettingsLoader::init_template_engine_setting(Settings* settings, const YAML::Node& config)
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

/*void YAMLSettingsLoader::_init_secure(Settings* settings, const YAML::Node& secure)
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
}*/

/*void YAMLSettingsLoader::_init_databases(Settings* settings, const YAML::Node& databases)
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
}*/

/*void YAMLSettingsLoader::_init_sqlite3_database(
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
