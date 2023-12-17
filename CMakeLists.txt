cmake_minimum_required(VERSION 3.26)
project(ced_echo C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(lib/ced)

include_directories(lib/ced/src)

add_executable(ced_echo main.c)
target_link_libraries(ced_echo PRIVATE ced)