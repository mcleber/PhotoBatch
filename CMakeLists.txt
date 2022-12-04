cmake_minimum_required(VERSION 3.20)

project(PhotoBatch)

add_executable(PhotoBatch "PhotoBatch.cpp"
	"ArgumentParser.cpp" 
	"ArgumentParser.h" 
	"Utils.cpp" 
	"Utils.h"
	"Mode.cpp"
    "Mode.h"
    "RenameMode.cpp"
    "RenameMode.h" 
	"ConvertMode.cpp"
    "ConvertMode.h"
	"ResizeMode.cpp"
	"ResizeMode.h"
	"ScaleMode.cpp"
    "ScaleMode.h"
    "STBDef.cpp"
	"ModeFactory.cpp"
    "ModeFactory.h"
)

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 20)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Desabilita o modo quiet do FetchContent" FORCE)

include(FetchContent)

#add_subdirectory(UnitTests)
add_subdirectory(External)

target_link_libraries(PhotoBatch PRIVATE stb)
