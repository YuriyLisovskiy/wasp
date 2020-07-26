/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/views/redirect_view.h>


class RedirectView : public xw::views::RedirectView
{
public:
	explicit RedirectView(xw::conf::Settings* settings);
};
