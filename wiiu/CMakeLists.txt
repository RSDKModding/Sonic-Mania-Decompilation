cmake_minimum_required(VERSION 3.18.4)

project(SonicMania LANGUAGES C CXX)

add_executable(SonicMania WIN32
	"../SonicMania/Game.c"
	"../SonicMania/Objects/All.c"
)

target_include_directories(SonicMania PRIVATE
	"../SonicMania"
	"../SonicMania/Objects"
)

target_compile_definitions(SonicMania PRIVATE
	GAME_MAIN=main
)

add_subdirectory("../dependencies/RSDKv5/wiiu" EXCLUDE_FROM_ALL "RSDKv5")
target_link_libraries(SonicMania PRIVATE RSDKv5)

if(WIIU)
	wut_create_rpx(SonicMania)
endif()
