cmake_minimum_required(VERSION 3.16)
project(alg_practice C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_library(list
        list.h)
add_executable(list-tests
        test_list.cpp)