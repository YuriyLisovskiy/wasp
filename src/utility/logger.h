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
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/datetime/datetime.h"


__UTILITY_BEGIN__

class ILogger
{
public:
	virtual ~ILogger() = default;
	virtual void info(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void debug(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void warning(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void error(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void fatal(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void trace(const std::string& msg, int line = 0, const char* function = "", const char* file = "") = 0;
	virtual void trace(const char* msg, int line = 0, const char* function = "", const char* file = "") = 0;
};

class Logger : public ILogger
{
public:
	struct Config
	{
		bool log_info = true;
		bool log_debug = true;
		bool log_warning = true;
		bool log_error = true;
		bool log_fatal = true;
		bool log_trace = true;
		std::vector<std::ostream*> streams;
	};

	static ILogger* get_instance(const Logger::Config& cfg);

	void info(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void debug(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void warning(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void error(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void fatal(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void trace(const std::string& msg, int line = 0, const char* function = "", const char* file = "") override;
	void trace(const char* msg, int line = 0, const char* function = "", const char* file = "") override;

private:
	enum log_level_enum
	{
		ll_info, ll_debug, ll_warning, ll_error, ll_fatal, ll_trace
	};

	const char* DEFAULT = "\033[0m";
	const char* BLACK = "\033[30m";                 // Black
	const char* RED = "\033[31m";                   // Red
	const char* GREEN = "\033[32m";                 // Green
	const char* YELLOW = "\033[33m";                // Yellow
	const char* BLUE = "\033[34m";                  // Blue
	const char* MAGENTA = "\033[35m";               // Magenta
	const char* CYAN = "\033[36m";                  // Cyan
	const char* WHITE = "\033[37m";                 // White
	const char* BOLD_BLACK = "\033[1m\033[30m";     // Bold Black
	const char* BOLD_RED = "\033[1m\033[31m";       // Bold Red
	const char* BOLD_GREEN = "\033[1m\033[32m";     // Bold Green
	const char* BOLD_YELLOW = "\033[1m\033[33m";    // Bold Yellow
	const char* BOLD_BLUE = "\033[1m\033[34m";      // Bold Blue
	const char* BOLD_MAGENTA = "\033[1m\033[35m";   // Bold Magenta
	const char* BOLD_CYAN = "\033[1m\033[36m";      // Bold Cyan
	const char* BOLD_WHITE = "\033[1m\033[37m";     // Bold White

	Logger::Config _config;

	static ILogger* _instance;

	explicit Logger(const Logger::Config& cfg);
	~Logger() override;
	void log(const std::string& msg, int line, const char* function, const char* file, Logger::log_level_enum level);
	void write_to_stream(const std::string& msg, const char* colour);
	void flush();
};

__UTILITY_END__


#endif // WASP_UTILS_LOGGER_H
