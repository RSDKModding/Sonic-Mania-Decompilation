cmake_minimum_required(VERSION 3.24.1)

project(SonicMania LANGUAGES C CXX)

add_executable(SonicMania
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
