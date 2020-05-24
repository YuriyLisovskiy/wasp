file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${SOURCES})

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-pthread")
endif()

# Build shared library.
add_library(${LIBRARY_NAME} SHARED ${SOURCES})
if (WIN32)
    target_link_libraries(${LIBRARY_NAME} wsock32 Ws2_32)
    add_custom_command(
        TARGET ${LIBRARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/src/${LIBRARY_NAME}.dll
        ${CMAKE_BINARY_DIR}/example/${LIBRARY_NAME}.dll
    )
endif()
