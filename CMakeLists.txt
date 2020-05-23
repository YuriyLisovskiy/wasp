cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

project(XalwartFramework)

set(LIBRARY_NAME "xalwart")

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(tests)
