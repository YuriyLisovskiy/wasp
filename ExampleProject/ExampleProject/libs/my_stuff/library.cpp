/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./library.h"

#include "./filters/my_filters.h"


using namespace xw::render;

MyStuff::MyStuff(xw::conf::Settings* settings) : lib::Library(settings)
{
}

std::shared_ptr<lib::Filters> MyStuff::get_filters()
{
	return std::make_shared<lib::Filters>(std::map<std::string, lib::Filter>{
		{"to_upper", toUpper},
		{"rm_digits", rmDigits},
		{"type", getType},
	});
}

std::string MyStuff::name()
{
	return "MyStuff";
}
