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


__CORE_BEGIN__

std::shared_ptr<ILogger> Logger::_instance = nullptr;

std::shared_ptr<ILogger> Logger::get_instance(const Config& cfg)
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

Logger::Logger(const Config& cfg)
{
	this->_config = cfg;
	if (cfg.streams.empty())
	{
		this->_config.streams.emplace_back(&std::cout);
	}
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

void Logger::print(const std::string& msg, Color colour, char end)
{
	if (this->_config.enable_print)
	{
		std::cout << this->get_colour(colour) << msg << this->_colors[Color::DEFAULT] << end;
	}
}

void Logger::print(const char* msg, Color colour, char end)
{
	this->print(std::string(msg), colour, end);
}

void Logger::info(const core::BaseException& exc)
{
	this->info(exc.what(), exc.line(), exc.function(), exc.file());
}

void Logger::debug(const core::BaseException& exc)
{
	this->debug(exc.what(), exc.line(), exc.function(), exc.file());
}

void Logger::warning(const core::BaseException& exc)
{
	this->warning(exc.what(), exc.line(), exc.function(), exc.file());
}

void Logger::error(const core::BaseException& exc)
{
	this->error(exc.what(), exc.line(), exc.function(), exc.file());
}

void Logger::fatal(const core::BaseException& exc)
{
	this->fatal(exc.what(), exc.line(), exc.function(), exc.file());
}

void Logger::log(const std::string& msg, int line, const char* function, const char* file, Logger::log_level_enum level)
{
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

	std::string result = "[" + dt::now().strftime("%F %T") +
		"] " + level_name + ":" + full_msg + "\n";
	this->write_to_stream(result, this->get_colour(colour));
}

void Logger::write_to_stream(const std::string& msg, const char* colour)
{
	const char* default_colour = this->_colors[Color::DEFAULT];
	for (auto& stream : this->_config.streams)
	{
		*stream << '\n' << colour << msg << default_colour << '\n';
	}

	this->flush();
}

void Logger::flush()
{
	for (auto& stream : this->_config.streams)
	{
		stream->flush();
	}
}

const char* Logger::get_colour(Color colour)
{
	if (this->_colors.find(colour) == this->_colors.end())
	{
		colour = Color::DEFAULT;
	}

	return this->_colors[colour];
}

void Logger::set_config(const Config& config)
{
	this->_config = config;
}

__CORE_END__
