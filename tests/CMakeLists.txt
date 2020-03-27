set(BINARY ${CMAKE_PROJECT_NAME}-tests)

# All tests.
file(GLOB DUPLICATE_MAINS
    "${PROJECT_SOURCE_DIR}/tests/unittests/*/main.cpp"
    "${PROJECT_SOURCE_DIR}/tests/unittests/*/*/main.cpp"
    "${PROJECT_SOURCE_DIR}/tests/unittests/*/*/*/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/collections_tests/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/conf_tests/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/core_tests/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/http_tests/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/render_tests/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/utils_tests/main.cpp"
#    "${PROJECT_SOURCE_DIR}/tests/unittests/views_tests/main.cpp"
)
# set(DUPLICATE_MAINS )

file(GLOB_RECURSE UNITTEST_SOURCES LIST_DIRECTORIES true *.h *.cpp EXCEPT ${DUPLICATE_MAINS})
set(SOURCES ${UNITTEST_SOURCES})

list(LENGTH UNITTEST_SOURCES L)
message(${L})
list(REMOVE_ITEM UNITTEST_SOURCES ${DUPLICATE_MAINS})
list(LENGTH UNITTEST_SOURCES L)
message(${L})

add_executable(${BINARY} ${UNITTEST_SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})

# Link shared library.
set(WASP_LIB_DLL ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/libwasp.so)
target_link_libraries(${BINARY} PUBLIC ${WASP_LIB_DLL} gtest)
