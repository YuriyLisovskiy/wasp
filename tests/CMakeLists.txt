set(BINARY ${CMAKE_PROJECT_NAME}-tests)

# All tests.
file(GLOB DUPLICATE_MAINS
    "${PROJECT_SOURCE_DIR}/tests/unittests/*/main.cpp"
    "${PROJECT_SOURCE_DIR}/tests/unittests/*/*/main.cpp"
    "${PROJECT_SOURCE_DIR}/tests/unittests/*/*/*/main.cpp"
)

file(GLOB_RECURSE UNITTEST_SOURCES LIST_DIRECTORIES true *.h *.cpp EXCEPT ${DUPLICATE_MAINS})
set(SOURCES ${UNITTEST_SOURCES})

list(REMOVE_ITEM UNITTEST_SOURCES ${DUPLICATE_MAINS})

add_executable(${BINARY} ${UNITTEST_SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})

# Link shared library.

message(${PROJECT_SOURCE_DIR})

set(WASP_LIB_DLL ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/libwasp.so)
target_link_libraries(${BINARY} PUBLIC ${WASP_LIB_DLL} gtest stdc++fs)
