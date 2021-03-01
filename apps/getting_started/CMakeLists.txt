cmake_minimum_required(VERSION 3.10)

add_executable(window window.cpp)
target_link_libraries(window ivory pthread)