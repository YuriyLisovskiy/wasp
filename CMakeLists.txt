cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

project(XalwartFramework)

set(LIBRARY_NAME "xalwart")
set(LIBRARY_VERSION "0.0.1")

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

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(tests)
