set(BINARY unittests)

set(WASP_LIB_DLL ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/libwasp.so)

function(add_tests SUB_DIR SUB_NAME)

    set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/unittests/${SUB_DIR})
    file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${TESTS_DIR}*.cpp)

    file(GLOB FILES_TO_REMOVE
        "${TESTS_DIR}*/main.cpp"
        "${TESTS_DIR}*/*/main.cpp"
        "${TESTS_DIR}*/*/*/main.cpp;"
    )

    if (FILES_TO_REMOVE)
        list(REMOVE_ITEM SOURCES ${FILES_TO_REMOVE})
    endif()

    add_executable(${BINARY}-${SUB_NAME} ${SOURCES})
    target_link_libraries(${BINARY}-${SUB_NAME} PUBLIC ${WASP_LIB_DLL} gtest stdc++fs)
endfunction()

function(add_sub_tests SUB_NAME)
    add_tests(${SUB_NAME}_tests/ ${SUB_NAME})
endfunction(add_sub_tests)

add_tests("" all)

add_sub_tests(collections)
add_sub_tests(conf)
add_sub_tests(core)
add_sub_tests(http)
add_sub_tests(render)
add_sub_tests(utils)
add_sub_tests(views)
