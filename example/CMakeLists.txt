set(BINARY "${LIBRARY_NAME}-example")

# Shared library.
set(WASP_LIB_DLL ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/lib${LIBRARY_NAME}.so)

# Setup example app sources.
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ./*/*.h ./*/*.cpp)
set(SOURCES ${SOURCES})

add_executable(${BINARY} ${LIB_SOURCES} ${SOURCES} "./main.cpp")
target_link_libraries(${BINARY} ${WASP_LIB_DLL})
target_link_libraries(${BINARY} stdc++fs)
