# Basic CMake that uses All.c
cmake_minimum_required(VERSION 3.7)

project(SonicMania)
set(GAME_NAME SonicMania CACHE STRING "The game directory to look into")

set(OUTPUT_NAME "Game" CACHE STRING "The name of the built library")

add_library(SonicMania SHARED
    ${GAME_NAME}/Game.c
    ${GAME_NAME}/Objects/All.c
)

target_include_directories(SonicMania PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
)

set_target_properties(SonicMania PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

unset(GAME_NAME CACHE)
unset(OUTPUT_NAME CACHE)