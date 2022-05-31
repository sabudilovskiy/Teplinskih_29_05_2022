cmake_minimum_required(VERSION 3.20)
project(Teplinskih_29_05_2022)
include_directories(includes)
set(CMAKE_CXX_STANDARD 20)

add_executable(Teplinskih_29_05_2022 main.cpp includes/Thread_safe_queue.h includes/Thread_pool.h includes/Guard.h includes/Thread_safe_answer.h includes/Random.h)
