cmake_minimum_required(VERSION 3.24)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 20)

project(Isacc++)
add_library(Isaac++ INTERFACE)
target_include_directories(Isaac++ INTERFACE ${CMAKE_CURRENT_LIST_DIR})
