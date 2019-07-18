cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

# package information
set(PACKAGE_NAME       "wasp")
set(PACKAGE_VERSION    "0.0.1-dev")
set(PACKAGE_STRING     "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TAR_NAME   "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUG_REPORT "https://github.com/YuriyLisovskiy/wasp/issues")

set(PACKAGE_TESTS "unit-tests")

set(CMAKE_CXX_FLAGS "-pthread")

project(${PACKAGE_NAME})

file(GLOB LIB_FILES
		"src/conf/*.h"
		"src/conf/*.cpp"
		"src/utils/*.h"
		"src/utils/*.cpp"
		"src/utils/datetime/*.h"
		"src/utils/datetime/*.cpp"
		"src/core/*.h"
		"src/core/*.cpp"
		"src/middleware/*.h"
		"src/middleware/*.cpp"
		"src/http/parsers/*.h"
		"src/http/parsers/*.cpp"
		"src/http/*.h"
		"src/http/*.cpp"
		"src/*.h"
		"src/*.cpp"
	)

file(GLOB TEST_FILES
		"tests/unittests/http/*.h"
		"tests/unittests/http/parsers/*.h"
		"tests/*.h"
	)

set(LIB_SRC ${LIB_FILES})
set(TEST_SRC ${TEST_FILES})

if(EXISTS "${PROJECT_SOURCE_DIR}/examples/http_server.cpp")
	add_executable(http_server_example ${LIB_SRC} "examples/http_server.cpp")
else()
    message("main.cpp does not exist")
endif()

add_executable(${PACKAGE_TESTS} ${LIB_SRC} ${TEST_SRC} "tests/unittests/main.cpp")
target_link_libraries(${PACKAGE_TESTS} gtest gtest_main rt pthread ${TEST_SRC})
