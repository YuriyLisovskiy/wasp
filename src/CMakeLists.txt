cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

set(BINARY "wasp")

project(${BINARY})


file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${SOURCES})

# Build shared library.
add_library(${BINARY} SHARED ${SOURCES})
