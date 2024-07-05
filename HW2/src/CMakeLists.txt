cmake_minimum_required(VERSION 3.28)
project(src)


set(CMAKE_CXX_STANDARD 23)

add_executable(src main.cpp quat.hpp)
add_subdirectory(../../CPPLAB1/src/Google_Tests)
