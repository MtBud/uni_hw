cmake_minimum_required(VERSION 3.0)
project(recenze)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        ./transmission/transmission.c
)

add_executable( my_project ${SOURCE_FILES})