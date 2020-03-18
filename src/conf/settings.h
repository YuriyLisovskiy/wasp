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
 * conf/settings.h
 *
 * Purpose: entire Wasp application's settings.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>
#include <vector>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/logger.h"
#include "../apps/interfaces.h"
#include "../middleware/interfaces.h"
#include "../core/management/base.h"
#include "../core/regex.h"
#include "../render/backends/interfaces.h"


__URLS_BEGIN__
class UrlPattern;
__URLS_END__

__APPS_BEGIN__
class IAppConfig;
__APPS_END__

__BACKENDS_BEGIN__
class IBackend;
__BACKENDS_END__


__CONF_BEGIN__

struct Settings
{
	bool DEBUG;

	core::ILogger* LOGGER;

	/// Build paths inside the project like this: wasp::path::join(BASE_DIR, ...)
	std::string BASE_DIR;

	/// Hosts/domain names that are valid for this site.
	///
	/// For example: "127.0.0.1" - matches localhost, "*" - matches all
	std::vector<std::string> ALLOWED_HOSTS;

	/// Local time zone for this installation. All choices can be found here:
	/// https://en.wikipedia.org/wiki/List_of_tz_zones_by_name (although not all
	/// systems may support all possibilities). When USE_TZ is true, this is
	/// interpreted as the default user time zone.
	std::string TIME_ZONE;

	/// If you set this to True, Django will use timezone-aware datetimes.
	bool USE_TZ;

	/// Default charset to use for all HttpResponse objects, if a MIME type isn't
	/// manually specified. It's used to construct the Content-Type header.
	std::string DEFAULT_CHARSET;

	/// Root application where Wasp will load urlpatterns.
	///
	/// ROOT_APP is the first installed app by default, it can
	/// be overridden in project settings.
	std::shared_ptr<apps::IAppConfig> ROOT_APP;

	/// Vector of patterns which will be loaded from ROOT_APP.
	/// To change this setting, setup ROOT_APP in your project
	/// settings.
	std::vector<urls::UrlPattern> ROOT_URLCONF;

	/// List of AppConfig-derived objects representing apps.
	/// Order is required. The first app is interpreted as
	/// main application configuration.
	std::vector<std::shared_ptr<apps::IAppConfig>> INSTALLED_APPS;

	/// List of commands to run from command line.
	std::map<std::string, core::BaseCommand*> COMMANDS;

	/// Backend for rendering templates.
	render::backends::IBackend* TEMPLATES_BACKEND;

	/// Whether to append trailing slashes to URLs.
	bool APPEND_SLASH;

	/// List of regular expression objects representing User-Agent strings
	/// that are not allowed to visit any page, systemwide. Use this for bad
	/// robots/crawlers.
	///
	/// Here are a few examples:
	///     DISALLOWED_USER_AGENTS = {
	///         wasp::core::rgx::Regex(R"(NaverBot.*)"),
	///         wasp::core::rgx::Regex(R"(EmailSiphon.*)"),
	///         wasp::core::rgx::Regex(R"(SiteSucker.*)"),
	///         wasp::core::rgx::Regex(R"(sohu-search.*)")
	///     };
	std::vector<core::rgx::Regex> DISALLOWED_USER_AGENTS;

	/// List of compiled regular expression objects representing URLs that need not
	/// be reported by BrokenLinkEmailsMiddleware.
	///
	/// Here are a few examples:
	///    IGNORABLE_404_URLS = {
	///        wasp::core::rgx::Regex(R"(/apple-touch-icon.*\.png)"),
	///        wasp::core::rgx::Regex(R"(/favicon.ico)"),
	///        wasp::core::rgx::Regex(R"(/robots.txt)"),
	///        wasp::core::rgx::Regex(R"(/phpmyadmin/)"),
	///        wasp::core::rgx::Regex(R"(/apple-touch-icon.*\.png)")
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
	std::pair<std::string, std::string>* SECURE_PROXY_SSL_HEADER;

	/// List of middleware to use. Order is important; in the request phase, these
	/// middleware will be applied in the order given, and in the response
	/// phase the middleware will be applied in reverse order.
	std::vector<middleware::IMiddleware*> MIDDLEWARE;

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

	Settings();
	virtual ~Settings();
	virtual void init() = 0;
	virtual void override();
	void prepare();

	template <typename _T, typename = std::enable_if<std::is_base_of<apps::IAppConfig, _T>::value>>
	std::shared_ptr<apps::IAppConfig> app()
	{
		return std::make_shared<_T>(this);
	}

	template <typename _T, typename = std::enable_if<std::is_base_of<middleware::IMiddleware, _T>::value>>
	middleware::IMiddleware* middleware()
	{
		return new _T(this);
	}

	template <typename _CommandT, typename = std::enable_if<std::is_base_of<core::BaseCommand, _CommandT>::value>>
	std::pair<std::string, core::BaseCommand*> command()
	{
		auto* command = new _CommandT(this);
		return std::pair<std::string, core::BaseCommand*>{command->name(), command};
	}
};

__CONF_END__
