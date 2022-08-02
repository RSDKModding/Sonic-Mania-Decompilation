# Basic CMake that uses All.c

cmake_minimum_required(VERSION 3.7)

set(GAME_NAME SonicMania)

add_library(Game SHARED
    ${GAME_NAME}/Game.c
    ${GAME_NAME}/Objects/All.c
)

target_include_directories(Game PRIVATE
    ${GAME_NAME}
    ${GAME_NAME}/Objects
)