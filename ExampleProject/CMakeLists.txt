cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

project(ExampleProject)
set(BINARY application)

# Load and filter project sources.
file(
    GLOB_RECURSE SOURCES
    LIST_DIRECTORIES true
    */*.h */*.cpp
)
list(
	FILTER SOURCES
	EXCLUDE REGEX "^.*/(include|lib$|media|static|templates|cmake-build-*)/?.*"
)
foreach(entry ${SOURCES})
    if (IS_DIRECTORY ${entry})
        list(REMOVE_ITEM SOURCES ${entry})
    else()
        message(${entry})
    endif()
endforeach()

add_executable(${BINARY} ${SOURCES} "./main.cpp")

# Setup and link 'xalwart' framework.
set(XALWART_FRAMEWORK "xalwart")
set(LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
if (WIN32)
    set(DLL ${LIB_DIRECTORY}/xalwart/${XALWART_FRAMEWORK}-0.0.1.lib)
else()
    set(DLL ${LIB_DIRECTORY}/xalwart/lib${XALWART_FRAMEWORK}-0.0.1.so)
endif()
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${BINARY} PUBLIC ${INCLUDE_DIR}/${XALWART_FRAMEWORK}/vendor/yaml)
target_include_directories(${BINARY} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${BINARY} ${DLL})

# Required for <experimental/filesystem> on Linux which is used by 'xalwart'
if (UNIX)
    target_link_libraries(${BINARY} stdc++fs)
endif()
