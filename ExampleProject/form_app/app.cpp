/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./app.h"

#include "./views/form_view.h"


FormAppConfig::FormAppConfig(xw::conf::Settings* settings)
	: AppConfig(__FILE__, settings)
{
}

void FormAppConfig::urlpatterns()
{
	this->url<FormView>(
		R"(profile/<user_id>([0-9]*)/name/<user_name>([A-Za-z]+)/?)",
		"profile"
	);
}
