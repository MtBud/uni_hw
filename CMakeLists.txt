cmake_minimum_required(VERSION 3.0)
project(AAG)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        ./AAG/determinization/determinization.cpp
)

add_executable( my_project ${SOURCE_FILES})