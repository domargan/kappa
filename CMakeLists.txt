cmake_minimum_required(VERSION 3.8 FATAL_ERROR)#
set(CMAKE_VERBOSE_MAKEFILE ON)

project(kappa VERSION 0.1 LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -ansi -pedantic-errors -Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DPRINT_TEST")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(Boost_INCLUDE_DIR /opt/boost/include)
set(Boost_LIBRARY_DIR /opt/boost/lib)
find_package(Boost COMPONENTS system filesystem iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE kappa_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE kappa_HEADERS "${PROJECT_SOURCE_DIR}/*.h")

set(kappa_INCLUDE_DIRS "")
foreach (_headerFile ${kappa_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND kappa_INCLUDE_DIRS ${_dir})
endforeach ()
list(REMOVE_DUPLICATES kappa_INCLUDE_DIRS)

add_executable(kappa ${kappa_SOURCES})
target_include_directories(kappa PRIVATE ${kappa_INCLUDE_DIRS})
target_link_libraries(kappa ${Boost_LIBRARIES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

list(REMOVE_ITEM kappa_SOURCES ${PROJECT_SOURCE_DIR}/init.cpp)
add_executable(tests ${kappa_SOURCES} ${CMAKE_SOURCE_DIR}/test/tests_main.cpp)
target_include_directories(tests PRIVATE ${kappa_INCLUDE_DIRS})
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
