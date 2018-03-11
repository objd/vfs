cmake_minimum_required(VERSION 3.9)
project(vfs)

set(CMAKE_CXX_STANDARD 11)

include_directories(include src test)

add_subdirectory(src)
add_subdirectory(test)