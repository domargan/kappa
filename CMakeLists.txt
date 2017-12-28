cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(kappa VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic-errors -Wall -Wextra")

set(Boost_INCLUDE_DIR /opt/boost/include)
set(Boost_LIBRARY_DIR /opt/boost/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE kappa_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE kappa_HEADERS "${PROJECT_SOURCE_DIR}/*.h")

set (kappa_INCLUDE_DIRS "")
foreach (_headerFile ${kappa_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND kappa_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES kappa_INCLUDE_DIRS)

add_executable (kappa ${kappa_SOURCES})
target_include_directories(kappa PRIVATE ${kappa_INCLUDE_DIRS})
target_link_libraries(kappa ${Boost_LIBRARIES})