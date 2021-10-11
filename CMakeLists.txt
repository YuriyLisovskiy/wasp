cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(FRAMEWORK_NAME "xalwart")
set(LIBRARY_NAME "${FRAMEWORK_NAME}")
set(LIBRARY_VERSION "0.0.0")

project(${LIBRARY_NAME})

set(ROOT_DIR /usr/local)
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(LIB_DIR ${ROOT_DIR}/lib)

include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})

if (APPLE)
    set(XW_OPENSSL_DIR "openssl@1.1/1.1.1l" CACHE STRING "OpenSSL version to use for compiling lib.")

    set(XW_OPENSSL_OPT_HOMEBREW_INCLUDE /opt/homebrew/Cellar/${XW_OPENSSL_DIR}/include)
    if (EXISTS ${XW_OPENSSL_OPT_HOMEBREW_INCLUDE})
        include_directories(${XW_OPENSSL_OPT_HOMEBREW_INCLUDE})
    endif()

    set(XW_OPENSSL_OPT_HOMEBREW_LIB /opt/homebrew/Cellar/${XW_OPENSSL_DIR}/lib)
    if (EXISTS ${XW_OPENSSL_OPT_HOMEBREW_LIB})
        link_directories(${XW_OPENSSL_OPT_HOMEBREW_LIB})
    endif()

    set(XW_OPENSSL_USR_LOCAL_INCLUDE /usr/local/Cellar/${XW_OPENSSL_DIR}/include)
    if (EXISTS ${XW_OPENSSL_USR_LOCAL_INCLUDE})
        include_directories(${XW_OPENSSL_USR_LOCAL_INCLUDE})
    endif()

    set(XW_OPENSSL_USR_LOCAL_LIB /usr/local/Cellar/${XW_OPENSSL_DIR}/lib)
    if (EXISTS ${XW_OPENSSL_USR_LOCAL_LIB})
        link_directories(${XW_OPENSSL_USR_LOCAL_LIB})
    endif()
endif()

option(XW_BUILD_LIB "Build the library." ON)
if (${XW_BUILD_LIB})
    add_subdirectory(src)
endif()

option(XW_BUILD_TESTS "Build the library." OFF)
if (${XW_BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
