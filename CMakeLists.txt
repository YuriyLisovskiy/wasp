cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(FRAMEWORK_NAME "xalwart")
set(LIBRARY_NAME "${FRAMEWORK_NAME}")
set(LIBRARY_VERSION "0.0.1")

project(${LIBRARY_NAME})

function(remove_dirs SRC_LIST)
    foreach(entry ${SRC_LIST})
        if (IS_DIRECTORY ${entry})
            list(REMOVE_ITEM SRC_LIST ${entry})
        endif()
    endforeach()
endfunction(remove_dirs)

function(list_exclude SRC_LIST EXPR)
    list(FILTER ${SRC_LIST} EXCLUDE REGEX ${EXPR})
endfunction(list_exclude)

function(print SRC_LIST)
    foreach(entry ${SRC_LIST})
        message(${entry})
    endforeach(entry)
endfunction(print)

set(LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${LIB_SRC_DIR}/*.h ${LIB_SRC_DIR}/*.cpp)
remove_dirs(SOURCES)

# Build shared library.
set(FULL_LIBRARY_NAME ${LIBRARY_NAME}-${LIBRARY_VERSION})
add_library(${FULL_LIBRARY_NAME} SHARED ${SOURCES})

# Install library.
install(
    TARGETS ${FULL_LIBRARY_NAME}
    LIBRARY DESTINATION /usr/local/lib
)

add_custom_command(
    TARGET ${FULL_LIBRARY_NAME}
    POST_BUILD
    COMMAND XW_ROOT_DIR=${PROJECT_SOURCE_DIR}/src XW_LIB_NAME=${LIBRARY_NAME} bash ${PROJECT_SOURCE_DIR}/scripts/install.sh
)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_subdirectory(tests)
endif()
