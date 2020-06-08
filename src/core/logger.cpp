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
 * An implementation of core/logger.h
 */

#include "./logger.h"

// C++ libraries.
#include <iostream>
#include <cstring>

// Framework modules.
#include "./datetime.h"
#include "./path.h"


__CORE_INTERNAL_BEGIN__

bool LoggerStream::is_file()
{
	return false;
}

bool LoggerStream::is_console()
{
	return false;
}

void LoggerConsoleStream::write(const std::string& text)
{
	std::cout << text;
}

void LoggerConsoleStream::flush()
{
	std::cout.flush();
}

bool LoggerConsoleStream::is_console()
{
	return true;
}

LoggerFileStream::LoggerFileStream(const std::string& fp)
{
	this->_file = std::make_shared<File>(fp, "a");
	this->_file->open();
}

void LoggerFileStream::write(const std::string& text)
{
	if (this->_file->is_open())
	{
		this->_file->write_str(text);
	}
}

bool LoggerFileStream::is_file()
{
	return true;
}

void LoggerFileStream::flush()
{
	this->_file->flush();
}

bool LoggerFileStream::is_valid()
{
	return this->_file && this->_file->is_open();
}

__CORE_INTERNAL_END__


__CORE_BEGIN__

void LoggerConfig::add_console_stream()
{
	if (!this->_has_cout)
	{
		this->streams.push_back(std::make_shared<internal::LoggerConsoleStream>());
		this->_has_cout = true;
	}
}

void LoggerConfig::add_file_stream(const std::string& fp)
{
	auto fs = std::make_shared<internal::LoggerFileStream>(fp);
	if (fs->is_valid())
	{
		this->streams.push_back(fs);
	}
}

std::shared_ptr<ILogger> Logger::_instance = nullptr;

std::shared_ptr<ILogger> Logger::get_instance(const LoggerConfig& cfg)
{
	if (Logger::_instance == nullptr)
	{
		Logger::_instance = std::shared_ptr<Logger>(new Logger(cfg));
	}
	else
	{
		Logger::_instance->set_config(cfg);
	}

	return Logger::_instance;
}

Logger::Logger(const LoggerConfig& cfg)
{
	this->_config = cfg;
	if (cfg.streams.empty())
	{
		this->_config.add_console_stream();
	}

	this->_thread_pool = std::make_shared<internal::ThreadPool>(1);
}

void Logger::info(const std::string& msg, int line, const char* function, const char* file)
{
	this->_log(msg, line, function, file, Logger::log_level_enum::ll_info);
}

void Logger::debug(const std::string& msg, int line, const char* function, const char* file)
{
	this->_log(msg, line, function, file, Logger::log_level_enum::ll_debug);
}

void Logger::warning(const std::string& msg, int line, const char* function, const char* file)
{
	this->_log(msg, line, function, file, Logger::log_level_enum::ll_warning);
}

void Logger::error(const std::string& msg, int line, const char* function, const char* file)
{
	this->_log(msg, line, function, file, Logger::log_level_enum::ll_error);
}

void Logger::fatal(const std::string& msg, int line, const char* function, const char* file)
{
	this->_log(msg, line, function, file, Logger::log_level_enum::ll_fatal);
}

void Logger::set_config(const LoggerConfig& config)
{
	this->_config = config;
}

void Logger::print(const std::string& msg, Color colour, char end)
{
	if (this->_config.enable_print)
	{
		this->_set_colour(colour);
		std::cout << msg << end;
		this->_set_colour(Color::DEFAULT);
	}
}

void Logger::print(const char* msg, Color colour, char end)
{
	this->print(std::string(msg), colour, end);
}

void Logger::info(const core::BaseException& exc)
{
	this->info(exc.get_message(), exc.line(), exc.function(), exc.file());
}

void Logger::debug(const core::BaseException& exc)
{
	this->debug(exc.get_message(), exc.line(), exc.function(), exc.file());
}

void Logger::warning(const core::BaseException& exc)
{
	this->warning(exc.get_message(), exc.line(), exc.function(), exc.file());
}

void Logger::error(const core::BaseException& exc)
{
	this->error(exc.get_message(), exc.line(), exc.function(), exc.file());
}

void Logger::fatal(const core::BaseException& exc)
{
	this->fatal(exc.get_message(), exc.line(), exc.function(), exc.file());
}

void Logger::_log(
	const std::string& msg, int line, const char* function,
	const char* file, Logger::log_level_enum level
)
{
	this->_thread_pool->push([this, level, line, file, function, msg]() -> void {
		std::string level_name;
		bool is_enabled;
		Color colour;
		switch (level)
		{
			case Logger::log_level_enum::ll_warning:
				level_name = "[warning]";
				is_enabled = this->_config.enable_warning;
				colour = Color::YELLOW;
				break;
			case Logger::log_level_enum::ll_error:
				level_name = "[error]";
				is_enabled = this->_config.enable_error;
				colour = Color::RED;
				break;
			case Logger::log_level_enum::ll_debug:
				level_name = "[debug]";
				is_enabled = this->_config.enable_debug;
				colour = Color::MAGENTA;
				break;
			case Logger::log_level_enum::ll_fatal:
				level_name = "[fatal]";
				is_enabled = this->_config.enable_fatal;
				colour = Color::BOLD_RED;
				break;
			default:
				level_name = "[info]";
				is_enabled = this->_config.enable_info;
				colour = Color::CYAN;
				break;
		}

		if (!is_enabled)
		{
			return;
		}

		std::string full_msg;
		if (line != 0 && std::strlen(file) > 0 && std::strlen(function) > 0)
		{
			full_msg = "\n\tFile \"" + std::string(file) + "\", line "
			           + std::to_string(line) + ", in "
			           + std::string(function) + "\n" + msg;
		}
		else
		{
			full_msg = " " + msg;
		}

		std::string result = "[" + dt::Datetime::now().strftime("%F %T") +
		                     "] " + level_name + ":" + full_msg;
		this->_write_to_stream(result, colour);
	});
}

void Logger::_write_to_stream(const std::string& msg, Color colour)
{
	for (auto& stream : this->_config.streams)
	{
		auto is_console = stream->is_console();
		if (is_console)
		{
			this->_set_colour(colour);
		}

		stream->write(msg + "\n");
		if (is_console)
		{
			this->_set_colour(Color::DEFAULT);
		}

		stream->flush();
	}
}

void Logger::_set_colour(Color colour)
{
#if defined(__unix__) || defined(__linux__)
	if (this->_colors.find(colour) == this->_colors.end())
	{
		colour = Color::DEFAULT;
	}

	std::cout << this->_colors[colour];
#endif
}

__CORE_END__
