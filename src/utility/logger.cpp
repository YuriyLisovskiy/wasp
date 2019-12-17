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

#include "./logger.h"


__UTILITY_BEGIN__

ILogger* Logger::_instance = nullptr;

Logger::~Logger()
{
	delete Logger::_instance;
	Logger::_instance = nullptr;
}

ILogger* Logger::get_instance()
{
	if (Logger::_instance == nullptr)
	{
		Logger::_instance = new Logger();
	}

	return Logger::_instance;
}

void Logger::info(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_info);
}

void Logger::debug(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_debug);
}

void Logger::warning(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_warning);
}

void Logger::error(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_error);
}

void Logger::fatal(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_fatal);
}

void Logger::trace(const std::string& msg, int line, const char* function, const char* file)
{
	this->write_to_stream(
		"Traceback (exception):\n  File \"" + std::string(file) + "\", line " +
		std::to_string(line) + ", in " + std::string(function) + "\n" + msg + "\n",
		Logger::log_level_enum::ll_error
	);
}

void Logger::trace(const char* msg, int line, const char* function, const char* file)
{
	this->trace(std::string(msg), line, function, file);
}

void Logger::log(const std::string& msg, int line, const char* function, const char* file, Logger::log_level_enum level)
{
	std::string level_name;
	switch (level)
	{
		case Logger::log_level_enum::ll_warning:
			level_name = "[warning]";
			break;
		case Logger::log_level_enum::ll_error:
			level_name = "[error]";
			break;
		case Logger::log_level_enum::ll_debug:
			level_name = "[debug]";
			break;
		case Logger::log_level_enum::ll_fatal:
			level_name = "[fatal]";
			break;
		default:
			level_name = "[info]";
			break;
	}
	std::locale::global(std::locale("uk_UA.utf8"));
	std::time_t t = std::time(nullptr);
	char now[100];
	std::strftime(now, sizeof(now), "%F %T", std::localtime(&t));

	std::string full_msg;
	if (line == 0 && std::strlen(file) > 0 && std::strlen(function) > 0)
	{
		full_msg = "\tFile \"" + std::string(file) + "\", line " + std::to_string(line) + ", in " + std::string(function) + "\n" + msg;
	}
	else
	{
		full_msg = msg;
	}

	this->write_to_stream("[" + std::string(now) + "] [" + "thread id" + "] " + level_name + ":\n" + full_msg + "\n", level);
}

void Logger::write_to_stream(const std::string& msg, Logger::log_level_enum level)
{
	// TODO: add multiple streams support
	std::cout << '\n' << msg;
	std::cout.flush();
}

__UTILITY_END__
