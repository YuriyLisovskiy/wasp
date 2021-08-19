/**
 * http/errors.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Common errors for http parsers.
 */

#pragma once

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

enum class ParseFormError
{
	Nil = 0, MissingFormBody, NotMultipart, MissingBoundary,
	ReadingFailure, ReadingTimedOut, PostTooLarge, SaveFileFailure, MessageTooLarge, Eof
};

__HTTP_END__
