#include <gtest/gtest.h>

#include "http/parsers/request_parser_tests.h"
#include "http/cookie_tests.h"

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
