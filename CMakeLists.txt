cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

# package information
set(PACKAGE_NAME "wasp")
set(PACKAGE_VERSION "0.0.1-dev")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TAR_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUG_REPORT "https://github.com/YuriyLisovskiy/wasp/issues")

set(PACKAGE_TESTS "unit-tests-all")
set(CORE_FILES_PACKAGE_TESTS "unit-tests-core-files")
set(HTTP_FILES_PACKAGE_TESTS "unit-tests-http")

set(CMAKE_CXX_FLAGS "-pthread")

project(${PACKAGE_NAME})

file(GLOB LIB_FILES
		"src/conf/*.h"
		"src/conf/*.cpp"
		"src/utility/*.h"
		"src/utility/*.cpp"
		"src/utility/datetime/*.h"
		"src/utility/datetime/*.cpp"
		"src/collections/*.h"
		"src/collections/*.cpp"
		"src/core/files/*.h"
		"src/core/files/*.cpp"
		"src/core/sockets/*.h"
		"src/core/sockets/*.cpp"
		"src/core/*.h"
		"src/core/*.cpp"
		"src/middleware/*.h"
		"src/middleware/*.cpp"
		"src/http/parsers/*.h"
		"src/http/parsers/*.cpp"
		"src/http/*.h"
		"src/http/*.cpp"
		"src/views/*.h"
		"src/views/*.cpp"
		"src/*.h"
		"src/*.cpp"
	)

file(GLOB TEST_CORE_FILES
		"tests/unittests/core_tests/files_tests/*.h"
)

file(GLOB TEST_HTTP_FILES
		"tests/unittests/http_tests/parsers_tests/*.h"
		"tests/unittests/http_tests/*.h"
)

file(GLOB TEST_FILES
		"tests/unittests/*.h"
		TEST_HTTP_FILES
		"tests/unittests/utility_tests/*.h"
		TEST_CORE_FILES
		"tests/unittests/core_tests/*.h"
		"tests/unittests/core_tests/exceptions_tests/*.h"
		"tests/unittests/collections_tests/*.h"
		"tests/*.h"
	)

set(LIB_SRC ${LIB_FILES})
set(TEST_SRC ${TEST_CORE_FILES} ${TEST_FILES})

set(CORE_FILES_TEST_SRC ${TEST_CORE_FILES})
set(HTTP_FILES_TEST_SRC ${TEST_HTTP_FILES})

file(GLOB EXAMPLE_FILES
		"example/*.h"
	)
set(EXAMPLE_SRC ${EXAMPLE_FILES})

if(EXISTS "${PROJECT_SOURCE_DIR}/example/http_server.cpp")
	add_executable(http_server_example ${LIB_SRC} ${EXAMPLE_SRC} "example/http_server.cpp")
else()
    message("main.cpp does not exist")
endif()

add_executable(${PACKAGE_TESTS} ${LIB_SRC} ${TEST_SRC} "tests/unittests/main.cpp")
target_link_libraries(${PACKAGE_TESTS} gtest gtest_main rt pthread ${TEST_SRC})

add_executable(${CORE_FILES_PACKAGE_TESTS} ${LIB_SRC} ${CORE_FILES_TEST_SRC} "tests/unittests/core_tests/files_tests/main.cpp")
target_link_libraries(${CORE_FILES_PACKAGE_TESTS} gtest gtest_main rt pthread ${CORE_FILES_TEST_SRC})

add_executable(${HTTP_FILES_PACKAGE_TESTS} ${LIB_SRC} ${HTTP_FILES_TEST_SRC} "tests/unittests/http_tests/main.cpp")
target_link_libraries(${HTTP_FILES_PACKAGE_TESTS} gtest gtest_main rt pthread ${HTTP_FILES_TEST_SRC})
