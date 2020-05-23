set(BINARY "${LIBRARY_NAME}-example")

# Shared library.
if (WIN32)
    set(LIB_DLL ${CMAKE_BINARY_DIR}/src/${LIBRARY_NAME}.lib)
else()
    set(LIB_DLL ${CMAKE_BINARY_DIR}/src/lib${LIBRARY_NAME}.so)
endif()

# Setup example app sources.
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ./*/*.h ./*/*.cpp)
set(SOURCES ${SOURCES})

add_executable(${BINARY} ${LIB_SOURCES} ${SOURCES} "./main.cpp")

target_link_libraries(${BINARY} ${LIB_DLL})

if (UNIX)
    target_link_libraries(${BINARY} stdc++fs)
endif()
