cmake_minimum_required(VERSION 3.21)
project(CLRS4)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

FILE(GLOB_RECURSE CLRS4_inc *.h)

add_executable(CLRS4 main.cpp ${CLRS4_inc})

target_include_directories(CLRS4 PRIVATE ${CMAKE_SOURCE_DIR})

if(MSVC)
  target_compile_options(CLRS4 PRIVATE /W4 /WX)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(/Ox)
  endif()
else()
  target_compile_options(CLRS4 PRIVATE -Wall -Wextra -Wpedantic -Werror)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
  endif()
endif()





