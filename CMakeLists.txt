cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # thanks dank_meme01
include(cmake/CPM.cmake) # why do I need this, this didnt happen before

project(PrismMenu VERSION 1.0.0)

# Use GLOB_RECURSE instead of GLOB
# to recursively add all source files 
# under src/ 
file(GLOB SOURCES
	src/*.cpp
    src/Hacks/*.cpp
)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

set(IMGUI_VERSION "v1.90")
if (APPLE)
    message("mat please merge the pr")
    CPMAddPackage("gh:ninXout/gd-imgui-cocos#dd044a8")
else()
    CPMAddPackage("gh:matcool/gd-imgui-cocos#5bc72d6") # specify a commit!
endif()
target_link_libraries(${PROJECT_NAME} imgui-cocos)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME})
