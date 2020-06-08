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
 * core/logger.h
 */

#pragma once

// C++ libraries.
#include <map>
#include <vector>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./exceptions.h"
#include "./files/file.h"


__CORE_INTERNAL_BEGIN__

class LoggerStream
{
public:
	virtual ~LoggerStream() = default;
	virtual void write(const std::string& text) = 0;
	virtual void flush() = 0;
	virtual bool is_file();
	virtual bool is_console();
};

class LoggerConsoleStream : public LoggerStream
{
public:
	void write(const std::string& text) override;
	void flush() override;
	bool is_console() override;
};

class LoggerFileStream : public LoggerStream
{
private:
	std::shared_ptr<File> _file;

public:
	explicit LoggerFileStream(const std::string& fp);
	void write(const std::string& text) override;
	bool is_file() override;
	void flush() override;
	bool is_valid();
};

__CORE_INTERNAL_END__


__CORE_BEGIN__

class LoggerConfig
{
private:
	bool _has_cout = false;

public:
	bool enable_info = true;
	bool enable_debug = true;
	bool enable_warning = true;
	bool enable_error = true;
	bool enable_fatal = true;
	bool enable_print = true;
	std::vector<std::shared_ptr<internal::LoggerStream>> streams;

	void add_console_stream();
	void add_file_stream(const std::string& fp);
};

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

	virtual void set_config(const LoggerConfig& config) = 0;
};

class Logger : public ILogger
{
public:
	static std::shared_ptr<ILogger> get_instance(const LoggerConfig& cfg);
//	static void reset_instance();

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

	void set_config(const LoggerConfig& config) override;

private:
#if defined(__unix__) || defined(__linux__)
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
#endif

	enum log_level_enum
	{
		ll_info, ll_debug, ll_warning, ll_error, ll_fatal
	};

	LoggerConfig _config;

	static std::shared_ptr<ILogger> _instance;

	explicit Logger(const LoggerConfig& cfg);
	void _log(
		const std::string& msg, int line, const char* function,
		const char* file, Logger::log_level_enum level
	);
	void _write_to_stream(const std::string& msg, Color colour);
	void _set_colour(Color colour);
};

__CORE_END__
