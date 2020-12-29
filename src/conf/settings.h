/**
 * conf/settings.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: entire application's settings.
 */

#pragma once

// Core libraries.
#include <xalwart.core/datetime.h>
#include <xalwart.core/logger.h>
#include <xalwart.core/result.h>

// Render libraries.
#include <xalwart.render/library/base.h>

// Vendor libraries.
#include "../yaml/yaml-cpp/yaml.h"

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../urls/url.h"
#include "../apps/interfaces.h"
#include "../middleware/interfaces.h"


__CORE_BEGIN__
class IServer;
__CORE_END__


__CONF_BEGIN__

struct Settings
{
public:
	bool DEBUG;

	std::shared_ptr<core::ILogger> LOGGER;

	/// Build paths inside the project like this: path::join(BASE_DIR, ...)
	std::string BASE_DIR;

	/// Hosts/domain names that are valid for this site.
	///
	/// For example: "127.0.0.1" - matches localhost, "*" - matches all
	std::vector<std::string> ALLOWED_HOSTS;

	/// Local time zone for this installation. All choices can be found here:
	/// https://en.wikipedia.org/wiki/List_of_tz_zones_by_name (although not all
	/// systems may support all possibilities). When USE_TZ is true, this is
	/// interpreted as the default user time zone.
	std::shared_ptr<core::dt::Timezone> TIME_ZONE;

	/// If you set this to True, Django will use timezone-aware datetimes.
	bool USE_TZ;

	/// Default charset to use for all HttpResponse objects, if a MIME type isn't
	/// manually specified. It's used to construct the Content-Type header.
	std::string CHARSET;

	/// Root application where framework will load urlpatterns.
	///
	/// ROOT_APP is the first installed app by default, it can
	/// be overridden in project settings.
	std::shared_ptr<apps::IAppConfig> ROOT_APP;

	/// Vector of patterns which will be loaded from ROOT_APP.
	/// To change this setting, setup ROOT_APP in your project
	/// settings.
	std::vector<std::shared_ptr<urls::UrlPattern>> ROOT_URLCONF;

	/// List of AppConfig-derived objects representing apps.
	/// Order is required. The first app is interpreted as
	/// main application configuration.
	std::vector<std::shared_ptr<apps::IAppConfig>> INSTALLED_APPS;

	/// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::BaseCommand>> COMMANDS;

	/// Backend for rendering templates.
	std::unique_ptr<render::IEngine> TEMPLATES_ENGINE;

	/// Whether to append trailing slashes to URLs.
	bool APPEND_SLASH;

	/// List of regular expression objects representing User-Agent strings
	/// that are not allowed to visit any page, systemwide. Use this for bad
	/// robots/crawlers.
	///
	/// Here are a few examples:
	///     DISALLOWED_USER_AGENTS = {
	///         core::rgx::Regex(R"(NaverBot.*)"),
	///         core::rgx::Regex(R"(EmailSiphon.*)"),
	///         core::rgx::Regex(R"(SiteSucker.*)"),
	///         core::rgx::Regex(R"(sohu-search.*)")
	///     };
	std::vector<core::rgx::Regex> DISALLOWED_USER_AGENTS;

	/// List of compiled regular expression objects representing URLs that need not
	/// be reported by BrokenLinkEmailsMiddleware.
	///
	/// Here are a few examples:
	///    IGNORABLE_404_URLS = {
	///        core::rgx::Regex(R"(/apple-touch-icon.*\.png)"),
	///        core::rgx::Regex(R"(/favicon.ico)"),
	///        core::rgx::Regex(R"(/robots.txt)"),
	///        core::rgx::Regex(R"(/phpmyadmin/)"),
	///        core::rgx::Regex(R"(/apple-touch-icon.*\.png)")
	///    };
	std::vector<core::rgx::Regex> IGNORABLE_404_URLS;

	/// A secret key for this particular installation. Used in secret-key
	/// hashing algorithms. Set this in your settings.
	std::string SECRET_KEY;

	/// Absolute filesystem path to the directory that will hold user-uploaded files.
	///
	/// Example: "/var/www/example.com/media/"
	std::string MEDIA_ROOT;

	/// URL that handles the media served from MEDIA_ROOT.
	///
	/// Examples: "http://example.com/media/", "http://media.example.com/"
	std::string MEDIA_URL;

	/// Absolute path to the directory static files should be collected to.
	///
	/// Example: "/var/www/example.com/static/"
	std::string STATIC_ROOT;

	/// URL that handles the static files served from STATIC_ROOT.
	/// Example: "http://example.com/static/", "http://static.example.com/"
	std::string STATIC_URL;

	/// Maximum size, in bytes, of a request before it will be streamed to the
	/// file system instead of into memory.
	size_t FILE_UPLOAD_MAX_MEMORY_SIZE;

	/// Maximum size in bytes of request data (excluding file uploads) that will be
	/// read before a SuspiciousOperation (RequestDataTooBig) is raised.
	size_t DATA_UPLOAD_MAX_MEMORY_SIZE;

	/// Whether to prepend the "www." subdomain to URLs that don't have it.
	bool PREPEND_WWW;

	/// Maximum number of GET/POST parameters that will be read before a
	/// SuspiciousOperation (TooManyFieldsSent) is thrown.
	size_t DATA_UPLOAD_MAX_NUMBER_FIELDS;

	/// Default formatting for date objects.
	std::string DATE_FORMAT;

	/// Default formatting for datetime objects.
	std::string DATETIME_FORMAT;

	/// Default formatting for time objects.
	std::string TIME_FORMAT;

	/// Default formatting for date objects when only the year and month are relevant.
	std::string YEAR_MONTH_FORMAT;

	/// Default formatting for date objects when only the month and day are relevant.
	std::string MONTH_DAY_FORMAT;

	/// Default short formatting for date objects.
	std::string SHORT_DATE_FORMAT;

	/// Default short formatting for datetime objects.
	std::string SHORT_DATETIME_FORMAT;

	/// First day of week, to be used on calendars
	/// 0 means Sunday, 1 means Monday...
	uint FIRST_DAY_OF_WEEK;

	/// Decimal separator symbol
	char DECIMAL_SEPARATOR;

	/// Boolean that sets whether to add thousand separator when formatting numbers
	bool USE_THOUSAND_SEPARATOR;

	/// Thousand separator symbol
	char THOUSAND_SEPARATOR;

	/// Default X-Frame-Options header value
	std::string X_FRAME_OPTIONS;

	bool USE_X_FORWARDED_HOST;
	bool USE_X_FORWARDED_PORT;

	/// If your app is behind a proxy that sets a header to specify secure
	/// connections, AND that proxy ensures that user-submitted headers with the
	/// same name are ignored (so that people can't spoof it), set this value to
	/// a std::pair of (header_name, header_value). For any requests that come in with
	/// that header/value, request.is_secure() will return true.
	/// WARNING! Only set this if you fully understand what you're doing. Otherwise,
	/// you may be opening yourself up to a security risk.
	std::shared_ptr<std::pair<std::string, std::string>> SECURE_PROXY_SSL_HEADER;

	/// List of middleware to use. Order is important; in the request phase, these
	/// middleware will be applied in the order given, and in the response
	/// phase the middleware will be applied in reverse order.
	std::vector<std::shared_ptr<middleware::IMiddleware>> MIDDLEWARE;

	/// Settings for CSRF cookie.
	std::string CSRF_COOKIE_NAME;
	size_t CSRF_COOKIE_AGE;
	std::string CSRF_COOKIE_DOMAIN;
	std::string CSRF_COOKIE_PATH;
	bool CSRF_COOKIE_SECURE;
	bool CSRF_COOKIE_HTTP_ONLY;
	std::string CSRF_COOKIE_SAME_SITE;
	std::string CSRF_HEADER_NAME;
	std::vector<std::string> CSRF_TRUSTED_ORIGINS;
	bool CSRF_USE_SESSIONS;

	/// SSL settings (will be added in future).
	bool USE_SSL;

	/// Security middleware.
	bool SECURE_BROWSER_XSS_FILTER;
	bool SECURE_CONTENT_TYPE_NO_SNIFF;
	bool SECURE_HSTS_INCLUDE_SUBDOMAINS;
	bool SECURE_HSTS_PRELOAD;
	size_t SECURE_HSTS_SECONDS;
	std::vector<std::string> SECURE_REDIRECT_EXEMPT;
	std::string SECURE_REFERRER_POLICY;
	std::string SECURE_SSL_HOST;
	bool SECURE_SSL_REDIRECT;

	explicit Settings(const std::string& base_dir);
	virtual ~Settings() = default;
	void init();
	void prepare();
	virtual std::shared_ptr<core::IServer> use_server(
		bool verbose,
		size_t threads_count,
		size_t max_body_size,
		const std::shared_ptr<core::ILogger>& logger,
		const std::function<core::Result<std::shared_ptr<http::IHttpResponse>>(
			http::HttpRequest* request, const int& client
		)>& handler
	);

protected:
	virtual void register_logger();
	virtual void register_apps();
	virtual void register_middleware();
	virtual void register_libraries();
	virtual void register_templates_engine();
	virtual void register_templates_engine(const YAML::Node& config);
	virtual void register_loaders();

	template <typename T, typename = std::enable_if<std::is_base_of<apps::IAppConfig, T>::value>>
	void app(const std::string& full_name)
	{
		if (this->_apps.find(full_name) != this->_apps.end())
		{
			if (this->LOGGER)
			{
				this->LOGGER->warning(
						"unable to register '" + full_name + "' app which already exists"
				);
			}
		}
		else
		{
			this->_apps[full_name] = [this]() -> std::shared_ptr<apps::IAppConfig> {
				auto app = std::make_shared<T>(this);
				app->init(app->__type__());
				return app;
			};
		}
	}

	template <typename T, typename = std::enable_if<std::is_base_of<middleware::IMiddleware, T>::value>>
	void middleware(const std::string& full_name)
	{
		if (this->_middleware.find(full_name) != this->_middleware.end())
		{
			if (this->LOGGER)
			{
				this->LOGGER->warning(
						"unable to register '" + full_name + "' middleware which already exists"
				);
			}
		}
		else
		{
			this->_middleware[full_name] = [this]() -> std::shared_ptr<middleware::IMiddleware> {
				return std::make_shared<T>(this);
			};
		}
	}

	template <typename T, typename = std::enable_if<std::is_base_of<render::lib::ILibrary, T>::value>>
	void library(const std::string& full_name)
	{
		if (this->_libraries.find(full_name) != this->_libraries.end())
		{
			if (this->LOGGER)
			{
				this->LOGGER->warning(
						"unable to register '" + full_name + "' library which already exists"
				);
			}
		}
		else
		{
			this->_libraries[full_name] = [this]() -> std::shared_ptr<render::lib::ILibrary> {
				return std::make_shared<T>(this);
			};
		}
	}

	template <typename T, typename = std::enable_if<std::is_base_of<render::ILoader, T>::value>>
	void loader(const std::string& full_name)
	{
		if (this->_loaders.find(full_name) != this->_loaders.end())
		{
			if (this->LOGGER)
			{
				this->LOGGER->warning(
						"unable to register '" + full_name + "' loader which already exists"
				);
			}
		}
		else
		{
			this->_loaders[full_name] = [this]() -> std::shared_ptr<render::ILoader> {
				return std::make_shared<T>(this);
			};
		}
	}

private:
	const std::string CONFIG_ROOT = "application";
	const std::string CONFIG_NAME = "config.yml";
	const std::string LOCAL_CONFIG_NAME = "config.local.yml";

	[[nodiscard]] YAML::Node _load_config(const std::string& file_name) const;

	// Loads local configuration file and overrides existing one.
	static void _override_scalar(
			YAML::Node& config, YAML::Node& local, const std::string& key
	);
	static void _override_sequence(
			YAML::Node& config, YAML::Node& local, const std::string& key
	);
	void _override_config(YAML::Node& config);

	void _init_env(YAML::Node& config);
	void _init_logger(YAML::Node& logger);
	void _init_allowed_hosts(YAML::Node& allowed_hosts);
	void _init_disallowed_user_agents(YAML::Node& agents);
	void _init_ignorable_404_urls(YAML::Node& urls);
	void _init_formats(YAML::Node& config);
	void _init_csrf(YAML::Node& config);
	void _init_secure(YAML::Node& config);
	void _init_apps(YAML::Node& apps);
	void _init_middleware(YAML::Node& middleware);

private:
	std::map<
		std::string,
		std::function<std::shared_ptr<middleware::IMiddleware>()>
	> _middleware;

	std::map<
		std::string,
		std::function<std::shared_ptr<render::lib::ILibrary>()>
	> _libraries;

	std::map<
		std::string,
		std::function<std::shared_ptr<apps::IAppConfig>()>
	> _apps;

	std::map<
		std::string,
		std::function<std::shared_ptr<render::ILoader>()>
	> _loaders;

	[[nodiscard]]
	std::shared_ptr<apps::IAppConfig> _get_app(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<middleware::IMiddleware> _get_middleware(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<render::lib::ILibrary> _get_library(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<render::ILoader> _get_loader(const std::string& full_name) const;
};

__CONF_END__
