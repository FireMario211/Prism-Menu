cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # thanks dank_meme01

project(PrismMenu VERSION 1.0.0)

# Use GLOB_RECURSE instead of GLOB
# to recursively add all source files 
# under src/ 
file(GLOB SOURCES
	src/*.cpp
)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# imgui stuff 
set(IMGUI_VERSION "v1.90")
CPMAddPackage("gh:matcool/gd-imgui-cocos#4eb5777") # specify a commit!

if (FALSE) # in favor of resources
    # embedding files
    CPMAddPackage("gh:batterycenter/embed@1.2.6")

    # hacks
    b_embed(${PROJECT_NAME} resources/hacks/global.json)
    b_embed(${PROJECT_NAME} resources/hacks/player.json)
    b_embed(${PROJECT_NAME} resources/hacks/creator.json)
    b_embed(${PROJECT_NAME} resources/hacks/bypass.json)
    b_embed(${PROJECT_NAME} resources/hacks/misc.json)
    b_embed(${PROJECT_NAME} resources/hacks/settings.json)

    # langs
    b_embed(${PROJECT_NAME} resources/langs/spanish.json)
    #b_embed(${PROJECT_NAME} resources/langs/russian.json)

    b_embed(${PROJECT_NAME} resources/themes.json)
    # embedding files
endif()
target_link_libraries(${PROJECT_NAME} imgui-cocos)

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME})
