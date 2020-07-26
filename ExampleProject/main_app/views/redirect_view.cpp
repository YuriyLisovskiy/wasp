/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./redirect_view.h"


RedirectView::RedirectView(xw::conf::Settings* settings)
	: xw::views::RedirectView(settings, "/index", false, false)
{
}
