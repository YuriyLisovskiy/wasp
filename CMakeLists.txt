cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

# Package information.
set(PACKAGE_NAME "wasp")
set(PACKAGE_VERSION "0.0.1-dev")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TAR_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUG_REPORT "https://github.com/YuriyLisovskiy/wasp/issues")

project(${PACKAGE_NAME})


# Test names by packages.
set(PACKAGE_MAIN_TESTS "unit-tests")
set(PACKAGE_MAIN_UNIT_TESTS "unit-tests")
set(PACKAGE_COLLECTIONS_UNIT_TESTS "unit-tests-collections")
set(PACKAGE_CONF_UNIT_TESTS "unit-tests-conf")
set(PACKAGE_CORE_UNIT_TESTS "unit-tests-core")
set(PACKAGE_CORE_DATETIME_UNIT_TESTS "unit-tests-core-datetime")
set(PACKAGE_CORE_OBJECT_UNIT_TESTS "unit-tests-core-object")
set(PACKAGE_HTTP_UNIT_TESTS "unit-tests-http")
set(PACKAGE_UTILS_UNIT_TESTS "unit-tests-utils")
set(PACKAGE_VIEWS_UNIT_TESTS "unit-tests-views")

set(CMAKE_CXX_FLAGS "-pthread")


file(GLOB LIB_FILES
	"src/apps/*.h"
	"src/apps/*.cpp"
	"src/conf/*.h"
	"src/conf/*.cpp"
	"src/collections/*.h"
	"src/collections/*.cpp"
	"src/core/datetime/*.h"
	"src/core/datetime/*.cpp"
	"src/core/datetime/predefined/*.h"
	"src/core/datetime/predefined/*.cpp"
	"src/core/files/*.h"
	"src/core/files/*.cpp"
	"src/core/flags/*.h"
	"src/core/flags/*.cpp"
	"src/core/parsers/*.h"
	"src/core/parsers/*.cpp"
	"src/core/signing/*.h"
	"src/core/signing/*.cpp"
	"src/core/net/*.h"
	"src/core/net/*.cpp"
	"src/core/string/*.h"
	"src/core/string/*.cpp"
	"src/core/object/*.h"
	"src/core/object/*.cpp"
	"src/core/management/*.h"
	"src/core/management/*.cpp"
	"src/core/management/commands/*.h"
	"src/core/management/commands/*.cpp"
	"src/core/*.h"
	"src/core/*.cpp"
	"src/middleware/*.h"
	"src/middleware/*.cpp"
	"src/http/*.h"
	"src/http/*.cpp"
	"src/views/*.h"
	"src/views/*.cpp"
	"src/urls/resolvers/*.h"
	"src/urls/resolvers/*.cpp"
	"src/urls/*.h"
	"src/urls/*.cpp"
	"src/utils/*.h"
	"src/utils/*.cpp"
	"src/utils/crypto/*.h"
	"src/utils/crypto/*.cpp"
	"src/*.h"
	"src/*.cpp"
)
set(LIB_SRC ${LIB_FILES})


# Collections package tests setup.
file(GLOB COLLECTIONS_TEST_SRC
	"tests/unittests/collections_tests/*.h"
)
add_executable(${PACKAGE_COLLECTIONS_UNIT_TESTS} ${LIB_SRC} ${COLLECTIONS_TEST_SRC} "tests/unittests/collections_tests/main.cpp")
target_link_libraries(${PACKAGE_COLLECTIONS_UNIT_TESTS} gtest gtest_main rt pthread ${COLLECTIONS_TEST_SRC})


# Conf packages tests setup.
file(GLOB CONF_TEST_SRC
	"tests/unittests/conf_tests/*.h"
)
add_executable(${PACKAGE_CONF_UNIT_TESTS} ${LIB_SRC} ${CONF_TEST_SRC} "tests/unittests/conf_tests/main.cpp")
target_link_libraries(${PACKAGE_CONF_UNIT_TESTS} gtest gtest_main rt pthread ${CONF_TEST_SRC})


# Core packages tests setup.
file(GLOB CORE_TEST_SRC
	"tests/unittests/core_tests/datetime_tests/*.h"
	"tests/unittests/core_tests/exceptions_tests/*.h"
	"tests/unittests/core_tests/files_tests/*.h"
	"tests/unittests/core_tests/flags_tests/*.h"
	"tests/unittests/core_tests/object_tests/*.h"
	"tests/unittests/core_tests/parsers_tests/*.h"
	"tests/unittests/core_tests/string_tests/*.h"
	"tests/unittests/core_tests/*.h"
)
add_executable(${PACKAGE_CORE_UNIT_TESTS} ${LIB_SRC} ${CORE_TEST_SRC} "tests/unittests/core_tests/main.cpp")
target_link_libraries(${PACKAGE_CORE_UNIT_TESTS} gtest gtest_main rt pthread ${CORE_TEST_SRC})


# Core/datetime packages tests setup.
file(GLOB CORE_DATETIME_TEST_SRC
	"tests/unittests/core_tests/datetime_tests/*.h"
)
add_executable(
	${PACKAGE_CORE_DATETIME_UNIT_TESTS}
	${LIB_SRC} ${CORE_DATETIME_TEST_SRC}
	"tests/unittests/core_tests/datetime_tests/main.cpp"
)
target_link_libraries(${PACKAGE_CORE_DATETIME_UNIT_TESTS} gtest gtest_main rt pthread ${CORE_DATETIME_TEST_SRC})


# Core/datetime packages tests setup.
file(GLOB CORE_OBJECT_TEST_SRC
	"tests/unittests/core_tests/object_tests/*.h"
)
add_executable(
	${PACKAGE_CORE_OBJECT_UNIT_TESTS}
	${LIB_SRC} ${CORE_OBJECT_TEST_SRC}
	"tests/unittests/core_tests/object_tests/main.cpp"
)
target_link_libraries(${PACKAGE_CORE_OBJECT_UNIT_TESTS} gtest gtest_main rt pthread ${CORE_OBJECT_TEST_SRC})


# Http package tests setup.
file(GLOB HTTP_TEST_SRC
	"tests/unittests/http_tests/parsers_tests/*.h"
	"tests/unittests/http_tests/*.h"
)
add_executable(${PACKAGE_HTTP_UNIT_TESTS} ${LIB_SRC} ${HTTP_TEST_SRC} "tests/unittests/http_tests/main.cpp")
target_link_libraries(${PACKAGE_HTTP_UNIT_TESTS} gtest gtest_main rt pthread ${HTTP_TEST_SRC})


# Utils package tests setup.
file(GLOB UTILS_TEST_SRC
	"tests/unittests/utils_tests/crypto_tests/*.h"
	"tests/unittests/utils_tests/*.h"
)
add_executable(${PACKAGE_UTILS_UNIT_TESTS} ${LIB_SRC} ${UTILS_TEST_SRC} "tests/unittests/utils_tests/main.cpp")
target_link_libraries(${PACKAGE_UTILS_UNIT_TESTS} gtest gtest_main rt pthread ${UTILS_TEST_SRC})


# Views package tests setup.
file(GLOB VIEWS_TEST_SRC
	"tests/unittests/views_tests/*.h"
)
add_executable(${PACKAGE_VIEWS_UNIT_TESTS} ${LIB_SRC} ${VIEWS_TEST_SRC} "tests/unittests/views_tests/main.cpp")
target_link_libraries(${PACKAGE_VIEWS_UNIT_TESTS} gtest gtest_main rt pthread ${VIEWS_TEST_SRC})


# Setup all test sourses.
file(GLOB OTHER_TEST_SRC
	"tests/unittests/*.h"
	"tests/*.h"
)
set(TEST_SRC
	${OTHER_TEST_SRC}
	${COLLECTIONS_TEST_SRC}
	${CORE_TEST_SRC}
	${HTTP_TEST_SRC}
	${UTILITY_TEST_SRC}
	${VIEWS_TEST_SRC}
)
add_executable(${PACKAGE_MAIN_UNIT_TESTS} ${LIB_SRC} ${TEST_SRC} "tests/unittests/main.cpp")
target_link_libraries(${PACKAGE_MAIN_UNIT_TESTS} gtest gtest_main rt pthread ${TEST_SRC})


# Setup example program.
file(GLOB EXAMPLE_SRC
	"example/*.h"
	"example/*/*.h"
	"example/*/*.cpp"
)
if(EXISTS "${PROJECT_SOURCE_DIR}/example/main.cpp")
	add_executable(example ${LIB_SRC} ${EXAMPLE_SRC} "example/main.cpp")
else()
    message("main.cpp of example web server does not exist")
endif()
