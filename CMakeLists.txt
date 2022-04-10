cmake_minimum_required(VERSION 3.22)
project(CLRS4)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE CLRS4_inc *.h)

add_executable(CLRS4 main.cpp ${CLRS4_inc})
