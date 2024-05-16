cmake_minimum_required(VERSION 3.28)
project(kse_pp_task_1 C)

set(CMAKE_C_STANDARD 17)

add_executable(kse_pp_task_1 main.c
        gap_buffer.h
        gap_buffer.c)
