# Basic CMake that uses All.c
cmake_minimum_required(VERSION 3.7)

project(SonicMania)
set(GAME_NAME SonicMania CACHE STRING "The game directory to look into")

set(OUTPUT_NAME "Game" CACHE STRING "The name of the built library")

set(sources
    ${GAME_NAME}/Game.c
    ${GAME_NAME}/Objects/All.c
)

if (VITA)
    set(VITASDK_CMAKE_FILE "$ENV{VITASDK}/share/vita.cmake" CACHE PATH "VitaSDK CMake functions file")
    include("${VITASDK_CMAKE_FILE}" REQUIRED)

    add_executable(SonicMania
        ${sources}
    )
else()
    add_library(SonicMania SHARED
        ${sources}
    )
endif()

target_include_directories(SonicMania PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
)

if (VITA)
    vita_create_self(${OUTPUT_NAME}.suprx SonicMania
      GEN_EXPORTS export.yml
      UNSAFE
    )
else()
    set_target_properties(SonicMania PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
endif()

unset(GAME_NAME CACHE)
unset(OUTPUT_NAME CACHE)