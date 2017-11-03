cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(kappa VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic-errors -Wall -Wextra")

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR})