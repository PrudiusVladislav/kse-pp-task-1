cmake_minimum_required(VERSION 3.28)
project(kse_pp_task_1 CXX)

set(CMAKE_CXX_STANDARD 17)


add_executable(kse_pp_task_1 main.cpp
        GapBuffer.h
        GapBuffer.cpp
        StringUtils.h
        StringUtils.cpp
        CommandHandler.h
        CommandHandler.cpp
        Commands.h
        Commands.cpp)
