cmake_minimum_required(VERSION 4.0)
project(A2 C)

set(CMAKE_C_STANDARD 11)
find_package(Threads REQUIRED)

add_executable(A2Q1 a2q1.c)
target_link_libraries(A2Q1 Threads::Threads)

add_executable(A2Q2 a2q2.c)
target_link_libraries(A2Q2 Threads::Threads)