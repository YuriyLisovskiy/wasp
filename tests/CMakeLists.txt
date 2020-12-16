set(BINARY unittests)

set(LIB_DIRECTORY ${CMAKE_BINARY_DIR}/src)
if (WIN32)
    set(LIB_DLL ${LIB_DIRECTORY}/${LIBRARY_NAME}-${LIBRARY_VERSION}.lib)
else()
    set(LIB_DLL ${LIB_DIRECTORY}/lib${LIBRARY_NAME}-${LIBRARY_VERSION}.so)
endif()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-pthread")
endif()

function(add_tests SUB_DIR SUB_NAME)
    set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/unittests/${SUB_DIR})
    file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${TESTS_DIR}*.cpp)
    remove_dirs(SOURCES)
    list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp$")

    foreach(entry ${SOURCES})
        message(${entry})
    endforeach(entry)

#    file(GLOB FILES_TO_REMOVE
#        "${TESTS_DIR}*/main.cpp"
#        "${TESTS_DIR}*/*/main.cpp"
#        "${TESTS_DIR}*/*/*/main.cpp;"
#        "${TESTS_DIR}*/*/*/*/main.cpp;"
#    )

#    if (FILES_TO_REMOVE)
#        list(REMOVE_ITEM SOURCES ${FILES_TO_REMOVE})
#    endif()

    set(FULL_BIN ${BINARY}-${SUB_NAME})
    add_executable(${FULL_BIN} ${SOURCES} ${TESTS_DIR}/main.cpp)

    # Link dependencies.
    link_xw_library(${FULL_BIN} ${LIBRARY_NAME}.core 0.0.1)
    link_xw_library(${FULL_BIN} ${LIBRARY_NAME}.render 0.0.1)

    target_link_libraries(${FULL_BIN} PUBLIC ${LIB_DLL} gtest)
    if (UNIX)
        target_link_libraries(${FULL_BIN} PUBLIC stdc++fs)
    endif()

    target_include_directories(${FULL_BIN} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_include_directories(${FULL_BIN} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yaml)
endfunction()

function(add_sub_tests SUB_NAME)
    add_tests(${SUB_NAME}_tests/ ${SUB_NAME})
endfunction(add_sub_tests)

add_tests("" all)

#add_sub_tests(collections)
add_sub_tests(core)
add_sub_tests(http)
#add_sub_tests(render)
add_sub_tests(utils)
add_sub_tests(views)
