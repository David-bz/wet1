cmake_minimum_required(VERSION 3.7)
project(Ex3Wet1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        list_mtm1.h
        list_mtm1.c main.c)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors -Werror -DNDEBUG")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(Ex3Wet1 ${SOURCE_FILES} list_mtm1.c main.c)