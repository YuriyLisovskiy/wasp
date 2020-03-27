cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(BINARY "example")

project(${BINARY})

# Setup lib sources.
# file(GLOB_RECURSE LIB_SOURCES LIST_DIRECTORIES true ../src/*.h ../src/*.cpp)
# set(LIB_SOURCES ${LIB_SOURCES})

# Shared library.
set(WASP_LIB_DLL ${PROJECT_SOURCE_DIR}/../cmake-build-debug/src/libwasp.so)

# Setup example app sources.
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ./*/*.h ./*/*.cpp)
set(SOURCES ${SOURCES})

add_executable(${BINARY} ${LIB_SOURCES} ${SOURCES} "./main.cpp")
target_link_libraries(${BINARY} ${WASP_LIB_DLL})
target_link_libraries(${BINARY} stdc++fs)

# Add binary.
# if(EXISTS "./main.cpp")
    # add_executable(${BINARY} ${LIB_SOURCES} ${SOURCES} "./main.cpp")
    # target_link_libraries(${EXECUTABLE_NAME} ../cmake-build-debug/lib${PACKAGE_NAME}.so)
    # target_link_libraries(${BINARY} stdc++fs)
# else()
    # message("ERROR: 'main.cpp' of example web server does not exist")
# endif()
