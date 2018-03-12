cmake_minimum_required(VERSION 3.9)

project(vfs)

set(CMAKE_CXX_STANDARD 17)

include_directories(deps include)

add_subdirectory(src)
add_subdirectory(test)

add_executable(main-cpp main.cpp)
set_target_properties(main-cpp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(main-cpp vfs-uv)