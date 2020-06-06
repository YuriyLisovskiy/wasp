set(BINARY example)

set(LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/example/lib)
if (WIN32)
    set(DLL ${LIB_DIRECTORY}/${LIBRARY_NAME}.lib)
else()
    set(DLL ${LIB_DIRECTORY}/lib${LIBRARY_NAME}.so)
endif()

file(
    GLOB_RECURSE SOURCES
    LIST_DIRECTORIES true
    */*.h */*.cpp
)
filter_list(SOURCES "^.*/example/(include|lib)/?.*")
remove_dirs(SOURCES)

add_executable(${BINARY} ${SOURCES} "./main.cpp")

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/example/include)
target_include_directories(${BINARY} PUBLIC ${INCLUDE_DIR}/${LIBRARY_NAME}/vendor/yaml)
target_include_directories(${BINARY} PUBLIC ${INCLUDE_DIR})

target_link_libraries(${BINARY} ${DLL})

if (UNIX)
    target_link_libraries(${BINARY} stdc++fs)
endif()
