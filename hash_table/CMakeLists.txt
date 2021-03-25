cmake_minimum_required(VERSION 3.16)
project(alg_practice C)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(hash
        hash.h)
add_executable(hash_table
        hash_test.cpp)
