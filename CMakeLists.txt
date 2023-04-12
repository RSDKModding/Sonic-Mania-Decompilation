cmake_minimum_required(VERSION 3.13)
project(SonicMania)

set(RETRO_REVISION 3 CACHE STRING "What revision to compile for. Defaults to v5U = 3")
option(RETRO_MOD_LOADER "Enables or disables the mod loader." ON)

option(WITH_RSDK "Whether or not to build with RSDKv5. Defalts to true" ON)

if(NOT DEFINED GAME_STATIC)
    if(WIN32)
        set(GAME_STATIC OFF)
    elseif(ANDROID)
        set(GAME_STATIC ON) # just to keep the libraries uncluttered
    elseif(CMAKE_SYSTEM_NAME STREQUAL NintendoSwitch)
        set(GAME_STATIC ON)
    endif()
endif()

option(GAME_STATIC "Whether or not to build the game as a static library." $<IF:WITH_RSDK,ON,OFF>)

option(MANIA_PREPLUS "Whether or not to build Mania pre-plus. Defaults to false" OFF)

set(GAME_NAME "SonicMania" CACHE STRING "The game directory to look into")
set(GAME_OUTPUT_NAME "Game" CACHE STRING "The name of the built library")

# mmmmeh
set(GAME_SOURCES
    ${GAME_NAME}/Game.c
    ${GAME_NAME}/Objects/All.c
)

if(GAME_STATIC)
    add_library(${GAME_NAME} STATIC ${GAME_SOURCES})
else()
    add_library(${GAME_NAME} SHARED ${GAME_SOURCES})
endif()

target_include_directories(${GAME_NAME} PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
)

target_compile_definitions(${GAME_NAME} PRIVATE
    RETRO_REVISION=${RETRO_REVISION}
    RETRO_USE_MOD_LOADER=$<BOOL:${RETRO_MOD_LOADER}>
    MANIA_PREPLUS=$<BOOL:${MANIA_PREPLUS}>
)

if(WIN32)
    target_compile_definitions(${GAME_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

set_target_properties(${GAME_NAME} PROPERTIES OUTPUT_NAME ${GAME_OUTPUT_NAME})

message(NOTICE "static " ${GAME_STATIC})

if(WITH_RSDK)
    set(RSDK_PATH dependencies/RSDKv5 CACHE STRING "The path to look for RSDKv5 if using WITH_RSDK.")

    add_subdirectory(${RSDK_PATH})

    if(GAME_STATIC)
        target_include_directories(RetroEngine PRIVATE ${GAME_NAME}/)
        target_link_libraries(RetroEngine ${GAME_NAME})
    endif()
endif()