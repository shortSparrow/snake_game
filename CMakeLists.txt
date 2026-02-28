cmake_minimum_required(VERSION 4.1)
project(snake_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(snake_game main.cpp
        ansi_codes.h
        game_settings.h)
