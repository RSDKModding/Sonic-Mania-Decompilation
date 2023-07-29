cmake_minimum_required(VERSION 3.13)
project(SonicMania)

option(WITH_RSDK "Whether or not to build with RSDKv5. Defaults to true" ON)

if(NOT DEFINED GAME_STATIC)
    if(WIN32)
        set(GAME_STATIC OFF)
    elseif(UNIX)
        set(GAME_STATIC OFF)
    else()
        set(GAME_STATIC ON)
    endif()
endif()

option(GAME_STATIC "Whether or not to build the game as a static library." $<BOOL:$<IF:WITH_RSDK,ON,OFF>>)

option(MANIA_FIRST_RELEASE "Whether or not to build Mania's first release. Defaults to false" OFF)
if(MANIA_FIRST_RELEASE)
    set(MANIA_PREPLUS ON)
else()
    option(MANIA_PREPLUS "Whether or not to build Mania pre-plus. Defaults to false" OFF)
endif()


if(NOT MANIA_PREPLUS)
    set(GAME_VERSION 6 CACHE STRING "The game version to use. Defaults to 6 == Last Steam release")
else()
    set(GAME_VERSION 3 CACHE STRING "The game version to use. Defaults to 3 == Last pre-plus release")
endif()

set(GAME_NAME "SonicMania" CACHE STRING "The game directory to look into")
set(GAME_OUTPUT_NAME "Game" CACHE STRING "The name of the built library")

option(GAME_INCLUDE_EDITOR "Whether or not to include editor functions. Defaults to true" ON)

option(GAME_INCREMENTAL_BUILD "Whether or not to build all objects separately (for quicker dev-->build iterations). Defaults to false." OFF)

if(GAME_INCREMENTAL_BUILD)
    include(${GAME_NAME}/Objects.cmake)
    set(GAME_SOURCES
        ${GAME_NAME}/Game.c
        ${GENERATED_SOURCES}
    )
else()
    set(GAME_SOURCES
        ${GAME_NAME}/Game.c
        ${GAME_NAME}/Objects/All.c
    )
endif()

if(GAME_STATIC)
    add_library(${GAME_NAME} STATIC ${GAME_SOURCES})
else()
    add_library(${GAME_NAME} SHARED ${GAME_SOURCES})
endif()

target_include_directories(${GAME_NAME} PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
)

if(WIN32)
    target_compile_definitions(${GAME_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

set_target_properties(${GAME_NAME} PROPERTIES OUTPUT_NAME ${GAME_OUTPUT_NAME})

if(WITH_RSDK)
    set(RSDK_PATH dependencies/RSDKv5 CACHE STRING "The path to look for RSDKv5 if using WITH_RSDK.")

    add_subdirectory(${RSDK_PATH})

    if(GAME_STATIC)
        target_include_directories(RetroEngine PRIVATE ${GAME_NAME}/)
        target_link_libraries(RetroEngine ${GAME_NAME})
    endif()
else()
    set(RETRO_REVISION 3 CACHE STRING "What revision to compile for. Defaults to v5U = 3")

    option(RETRO_MOD_LOADER "Enables or disables the mod loader." ON)
    set(RETRO_MOD_LOADER_VER 2 CACHE STRING "Sets the mod loader version. Defaults to latest")

    target_compile_definitions(${GAME_NAME} PRIVATE
        RETRO_REVISION=${RETRO_REVISION}

        RETRO_USE_MOD_LOADER=$<BOOL:${RETRO_MOD_LOADER}>
        RETRO_MOD_LOADER_VER=${RETRO_MOD_LOADER_VER}

        GAME_INCLUDE_EDITOR=$<BOOL:${GAME_INCLUDE_EDITOR}>

        MANIA_PREPLUS=$<BOOL:${MANIA_PREPLUS}>
        MANIA_FIRST_RELEASE=$<BOOL:${MANIA_FIRST_RELEASE}>
        GAME_VERSION=${GAME_VERSION}
    )
endif()