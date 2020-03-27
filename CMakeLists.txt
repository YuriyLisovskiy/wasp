cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

# Package information.
set(PACKAGE_NAME "wasp")
set(PACKAGE_VERSION "0.0.1-dev")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TAR_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUG_REPORT "https://github.com/YuriyLisovskiy/wasp/issues")

project(${PACKAGE_NAME})

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Test names by packages.
set(PACKAGE_MAIN_TESTS "unit-tests")
set(PACKAGE_MAIN_UNIT_TESTS "unit-tests")
set(PACKAGE_COLLECTIONS_UNIT_TESTS "unit-tests-collections")
set(PACKAGE_CONF_UNIT_TESTS "unit-tests-conf")
set(PACKAGE_CORE_UNIT_TESTS "unit-tests-core")
set(PACKAGE_CORE_DATETIME_UNIT_TESTS "unit-tests-core-datetime")
set(PACKAGE_CORE_OBJECT_UNIT_TESTS "unit-tests-core-object")
set(PACKAGE_HTTP_UNIT_TESTS "unit-tests-http")
set(PACKAGE_RENDER_UNIT_TESTS "unit-tests-render")
set(PACKAGE_UTILS_UNIT_TESTS "unit-tests-utils")
set(PACKAGE_VIEWS_UNIT_TESTS "unit-tests-views")

set(CMAKE_CXX_FLAGS "-pthread")


file(GLOB LIB_SRC
	"src/*/*/*/*.h"
	"src/*/*/*/*.cpp"
	"src/*/*/*.h"
	"src/*/*/*.cpp"
	"src/*/*.h"
	"src/*/*.cpp"
	"src/*.h"
	"src/*.cpp"
)

# Build shared library.
# add_library(${PACKAGE_NAME} SHARED ${LIB_SRC})


# Perform library testing.

# Collections package tests setup.
file(GLOB COLLECTIONS_TEST_SRC
	"tests/unittests/collections_tests/*.h"
	"tests/unittests/collections_tests/*.h"
)
add_executable(${PACKAGE_COLLECTIONS_UNIT_TESTS} ${LIB_SRC} ${COLLECTIONS_TEST_SRC} "tests/unittests/collections_tests/main.cpp")
target_link_libraries(${PACKAGE_COLLECTIONS_UNIT_TESTS} gtest gtest_main rt pthread ${COLLECTIONS_TEST_SRC})
target_link_libraries(${PACKAGE_COLLECTIONS_UNIT_TESTS} stdc++fs)


# Conf packages tests setup.
file(GLOB CONF_TEST_SRC
	"tests/unittests/conf_tests/*.h"
)
add_executable(${PACKAGE_CONF_UNIT_TESTS} ${LIB_SRC} ${CONF_TEST_SRC} "tests/unittests/conf_tests/main.cpp")
target_link_libraries(${PACKAGE_CONF_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${CONF_TEST_SRC})
target_link_libraries(${PACKAGE_CONF_UNIT_TESTS} stdc++fs)


# Core packages tests setup.
file(GLOB CORE_TEST_SRC
	"tests/unittests/core_tests/*/*.h"
	"tests/unittests/core_tests/*.h"
)
add_executable(${PACKAGE_CORE_UNIT_TESTS} ${LIB_SRC} ${CORE_TEST_SRC} "tests/unittests/core_tests/main.cpp")
target_link_libraries(${PACKAGE_CORE_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${CORE_TEST_SRC})
target_link_libraries(${PACKAGE_CORE_UNIT_TESTS} stdc++fs)


# Core/datetime packages tests setup.
file(GLOB CORE_DATETIME_TEST_SRC
	"tests/unittests/core_tests/datetime_tests/*.h"
)
add_executable(
	${PACKAGE_CORE_DATETIME_UNIT_TESTS}
	${LIB_SRC} ${CORE_DATETIME_TEST_SRC}
	"tests/unittests/core_tests/datetime_tests/main.cpp"
)
target_link_libraries(${PACKAGE_CORE_DATETIME_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${CORE_DATETIME_TEST_SRC})
target_link_libraries(${PACKAGE_CORE_DATETIME_UNIT_TESTS} stdc++fs)


# Core/datetime packages tests setup.
file(GLOB CORE_OBJECT_TEST_SRC
	"tests/unittests/core_tests/object_tests/*.h"
)
add_executable(
	${PACKAGE_CORE_OBJECT_UNIT_TESTS}
	${LIB_SRC} ${CORE_OBJECT_TEST_SRC}
	"tests/unittests/core_tests/object_tests/main.cpp"
)
target_link_libraries(${PACKAGE_CORE_OBJECT_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${CORE_OBJECT_TEST_SRC})
target_link_libraries(${PACKAGE_CORE_OBJECT_UNIT_TESTS} stdc++fs)


# Http package tests setup.
file(GLOB HTTP_TEST_SRC_
	"tests/unittests/http_tests/*/*.h"
	"tests/unittests/http_tests/*.h"
)
set(HTTP_TEST_SRC ${HTTP_TEST_SRC_})

list(REMOVE_ITEM HTTP_TEST_SRC tests/unittests/http_tests/main.cpp)

message(${HTTP_TEST_SRC})

add_executable(${PACKAGE_HTTP_UNIT_TESTS} ${LIB_SRC} ${HTTP_TEST_SRC} "tests/unittests/http_tests/main.cpp")
target_link_libraries(${PACKAGE_HTTP_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${HTTP_TEST_SRC})
target_link_libraries(${PACKAGE_HTTP_UNIT_TESTS} stdc++fs)


# Render package tests setup.
file(GLOB RENDER_TEST_SRC
	"tests/unittests/render_tests/*/*.h"
	"tests/unittests/render_tests/*.h"
)
add_executable(${PACKAGE_RENDER_UNIT_TESTS} ${LIB_SRC} ${RENDER_TEST_SRC} "tests/unittests/render_tests/main.cpp")
target_link_libraries(${PACKAGE_RENDER_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${RENDER_TEST_SRC})
target_link_libraries(${PACKAGE_RENDER_UNIT_TESTS} stdc++fs)


# Utils package tests setup.
file(GLOB UTILS_TEST_SRC
	"tests/unittests/utils_tests/*/*.h"
	"tests/unittests/utils_tests/*.h"
)
add_executable(${PACKAGE_UTILS_UNIT_TESTS} ${LIB_SRC} ${UTILS_TEST_SRC} "tests/unittests/utils_tests/main.cpp")
target_link_libraries(${PACKAGE_UTILS_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${UTILS_TEST_SRC})
target_link_libraries(${PACKAGE_UTILS_UNIT_TESTS} stdc++fs)


# Views package tests setup.
file(GLOB VIEWS_TEST_SRC
	"tests/unittests/views_tests/*.h"
)
add_executable(${PACKAGE_VIEWS_UNIT_TESTS} ${LIB_SRC} ${VIEWS_TEST_SRC} "tests/unittests/views_tests/main.cpp")
target_link_libraries(${PACKAGE_VIEWS_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${VIEWS_TEST_SRC})
target_link_libraries(${PACKAGE_VIEWS_UNIT_TESTS} stdc++fs)


# Setup all test sourses.
file(GLOB OTHER_TEST_SRC
	"tests/unittests/*.h"
	"tests/*.h"
)
set(TEST_SRC
	${OTHER_TEST_SRC}
	${COLLECTIONS_TEST_SRC}
	${CONF_TEST_SRC}
	${CORE_TEST_SRC}
	${HTTP_TEST_SRC}
	${RENDER_TEST_SRC}
	${UTILS_TEST_SRC}
	${VIEWS_TEST_SRC}
)
add_executable(${PACKAGE_MAIN_UNIT_TESTS} ${LIB_SRC} ${TEST_SRC} "tests/unittests/main.cpp")
target_link_libraries(${PACKAGE_MAIN_UNIT_TESTS} gtest gtest_main rt pthread -std=c++1z ${TEST_SRC})
target_link_libraries(${PACKAGE_MAIN_UNIT_TESTS} stdc++fs)


# Setup example program.
file(GLOB EXAMPLE_SRC
	"example/*.h"
	"example/*/*.h"
	"example/*/*.cpp"
	"example/*/*/*.h"
	"example/*/*/*.cpp"
)

if(EXISTS "${PROJECT_SOURCE_DIR}/example/main.cpp")
	add_executable(example ${LIB_SRC} ${EXAMPLE_SRC} "example/main.cpp")
	# target_link_libraries(example ${PROJECT_SOURCE_DIR}/bin/lib${PACKAGE_NAME}.so)
	target_link_libraries(example stdc++fs)
else()
	message("main.cpp of example web server does not exist")
endif()
