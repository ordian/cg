cmake_minimum_required(VERSION 2.8.4)
project(task03)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Ofast")
set(SOURCE_FILES main.cpp)

add_executable(task03 ${SOURCE_FILES})
