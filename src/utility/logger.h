/*
 * Copyright (c) 2019 Yuriy Lisovskiy
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

#ifndef WASP_UTILS_LOGGER_H
#define WASP_UTILS_LOGGER_H

// C++ libraries.
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"
#include "../core/datetime/datetime.h"


__UTILITY_BEGIN__

class ILogger
{
public:
	enum Color
	{
		DEFAULT,
		BLACK,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		BOLD_BLACK,
		BOLD_RED,
		BOLD_GREEN,
		BOLD_YELLOW,
		BOLD_BLUE,
		BOLD_MAGENTA,
		BOLD_CYAN,
		BOLD_WHITE,
	};

	virtual ~ILogger() = default;
	virtual void info(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void debug(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void warning(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void error(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void fatal(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void print(const std::string& msg, Color colour = Color::DEFAULT, char end = '\n') = 0;
	virtual void print(const char* msg, Color colour = Color::DEFAULT, char end = '\n') = 0;

	virtual void info(const core::BaseException& exc) = 0;
	virtual void debug(const core::BaseException& exc) = 0;
	virtual void warning(const core::BaseException& exc) = 0;
	virtual void error(const core::BaseException& exc) = 0;
	virtual void fatal(const core::BaseException& exc) = 0;
};

class Logger : public ILogger
{
public:
	struct Config
	{
		bool enable_info_log = true;
		bool enable_debug_log = true;
		bool enable_warning_log = true;
		bool enable_error_log = true;
		bool enable_fatal_log = true;
		std::vector<std::ostream*> streams;
	};

	static ILogger* get_instance(const Logger::Config& cfg);
	static void reset_instance();

	void info(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void debug(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void warning(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void error(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void fatal(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void print(const std::string& msg, Color colour = Color::DEFAULT, char end = '\n') override;
	void print(const char* msg, Color colour = Color::DEFAULT, char end = '\n') override;

	void info(const core::BaseException& exc) override;
	void debug(const core::BaseException& exc) override;
	void warning(const core::BaseException& exc) override;
	void error(const core::BaseException& exc) override;
	void fatal(const core::BaseException& exc) override;

private:
	std::map<Color, const char*> _colors = {
		{DEFAULT, "\033[0m"},
		{BLACK, "\033[30m"},
		{RED, "\033[31m"},
		{GREEN, "\033[32m"},
		{YELLOW, "\033[33m"},
		{BLUE, "\033[34m"},
		{MAGENTA, "\033[35m"},
		{CYAN, "\033[36m"},
		{WHITE, "\033[37m"},
		{BOLD_BLACK, "\033[1m\033[30m"},
		{BOLD_RED, "\033[1m\033[31m"},
		{BOLD_GREEN, "\033[1m\033[32m"},
		{BOLD_YELLOW, "\033[1m\033[33m"},
		{BOLD_BLUE, "\033[1m\033[34m"},
		{BOLD_MAGENTA, "\033[1m\033[35m"},
		{BOLD_CYAN, "\033[1m\033[36m"},
		{BOLD_WHITE, "\033[1m\033[37m"},
	};

	enum log_level_enum
	{
		ll_info, ll_debug, ll_warning, ll_error, ll_fatal
	};

	Logger::Config _config;

	static ILogger* _instance;

	explicit Logger(const Logger::Config& cfg);
	~Logger() override = default;
	void log(const std::string& msg, int line, const char* function, const char* file, Logger::log_level_enum level);
	void write_to_stream(const std::string& msg, const char* colour);
	void flush();
	const char* get_colour(Color colour);
};

__UTILITY_END__


#endif // WASP_UTILS_LOGGER_H
