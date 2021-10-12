set(CMAKE_CXX_FLAGS "-pthread")

set(BINARY unittests)

function(add_tests SUB_DIR SUB_NAME)
    set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/${SUB_DIR})
    file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${TESTS_DIR}*.cpp)
    foreach(entry ${SOURCES})
        if (IS_DIRECTORY ${entry})
            list(REMOVE_ITEM SOURCES ${entry})
        endif()
    endforeach()
    list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp$")
    set(FULL_BIN ${BINARY}-${SUB_NAME})
    add_executable(${FULL_BIN} ${SOURCES} ${TESTS_DIR}/main.cpp)
    target_include_directories(${FULL_BIN} PUBLIC ${INCLUDE_DIR})
    if (NOT APPLE)
        target_link_libraries(${LIBRARY_NAME} PUBLIC stdc++fs)
    endif()
    target_link_libraries(${FULL_BIN} PUBLIC
        crypto
        ssl
        gtest
        ${FRAMEWORK_NAME}.base
        ${FRAMEWORK_NAME}.orm
        ${LIBRARY_NAME}
    )
endfunction()

function(add_sub_tests SUB_NAME)
    add_tests(${SUB_NAME}/ ${SUB_NAME})
endfunction(add_sub_tests)

add_tests("" all)
add_sub_tests(commands)
add_sub_tests(conf)
add_sub_tests(controllers)
add_sub_tests(http)
add_sub_tests(utility)
