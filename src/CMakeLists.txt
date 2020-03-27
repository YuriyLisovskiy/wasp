set(LIBRARY_NAME "wasp")

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${SOURCES})

# Build shared library.
add_library(${LIBRARY_NAME} SHARED ${SOURCES})
