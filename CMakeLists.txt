cmake_minimum_required(VERSION 3.5)

project(back-key LANGUAGES C)

find_package(Threads REQUIRED)

add_executable(back-key main.c)

set(CMAKE_CURRENT_BINARY_DIR ON)

target_link_libraries(back-key Threads::Threads)
