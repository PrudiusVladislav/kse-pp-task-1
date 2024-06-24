cmake_minimum_required(VERSION 3.28)
project(kse_pp_task_1 CXX)

set(CMAKE_CXX_STANDARD 17)


add_executable(kse_pp_task_1 main.cpp
        core/GapBuffer.h
        core/GapBuffer.cpp
        utils/StringUtils.h
        utils/StringUtils.cpp
        core/CommandHandler.h
        core/CommandHandler.cpp
        commands/Commands.h
        commands/Commands.cpp
        utils/CaesarCipher.h
        utils/CaesarCipher.cpp)
