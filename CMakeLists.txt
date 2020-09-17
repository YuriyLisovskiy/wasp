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

set(XW_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/${LIBRARY_NAME})

function(link_xw_library TARGET LIB_NAME LIB_VER)
    if (WIN32)
        set(DLL ${XW_LIB_DIR}/${LIB_NAME}-${LIB_VER}.lib)
    else()
        set(DLL ${XW_LIB_DIR}/lib${LIB_NAME}-${LIB_VER}.so)
    endif()
    target_link_libraries(${TARGET} PUBLIC ${DLL})
endfunction(link_xw_library)

add_subdirectory(src)
add_subdirectory(tests)
