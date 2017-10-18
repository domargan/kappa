cmake_minimum_required(VERSION 3.8)
project(kappa)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic-errors -Wall -Wextra")

set(SOURCE_FILES main.cpp)
add_executable(kappa ${SOURCE_FILES})