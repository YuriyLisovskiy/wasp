cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

project(XalwartFramework)

set(LIBRARY_NAME "xalwart")

function(remove_dirs SRC_LIST)
    foreach(entry ${SRC_LIST})
        if (IS_DIRECTORY ${entry})
            list(REMOVE_ITEM SRC_LIST ${entry})
        endif()
    endforeach()
endfunction(remove_dirs)

function(filter_list SRC_LIST EXPR)
    list(FILTER ${SRC_LIST} EXCLUDE REGEX ${EXPR})
endfunction(filter_list)

function(print SRC_LIST)
    foreach(entry ${SRC_LIST})
        message(${entry})
    endforeach(entry)
endfunction(print)

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(tests)
