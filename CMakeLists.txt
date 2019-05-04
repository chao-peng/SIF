cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_VERSION "1.0.0")
project(Sif VERSION ${PROJECT_VERSION})

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

add_subdirectory(libSif)
add_subdirectory(libUtils)
add_subdirectory(sif)

