file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
remove_dirs(SOURCES)

file(
    GLOB_RECURSE
    VENDOR_SOURCES
    LIST_DIRECTORIES true "../vendor/*.h" "../vendor/*.cpp"
)
remove_dirs(VENDOR_SOURCES)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-pthread")
endif()

# Build shared library
add_library(${LIBRARY_NAME} SHARED ${SOURCES} ${VENDOR_SOURCES})
if (WIN32)
    target_link_libraries(${LIBRARY_NAME} wsock32 Ws2_32)
    add_custom_command(
        TARGET ${LIBRARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/src/${LIBRARY_NAME}.dll
            ${CMAKE_BINARY_DIR}/example/${LIBRARY_NAME}.dll
    )
endif()

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/yaml)

add_custom_command(
    TARGET ${LIBRARY_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
        -DCMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}
        -DLIBRARY_NAME=${LIBRARY_NAME}
        -DDEPLOY_DIR=${PROJECT_SOURCE_DIR}/example
        -P ${PROJECT_SOURCE_DIR}/scripts/deploy_library.cmake
)
