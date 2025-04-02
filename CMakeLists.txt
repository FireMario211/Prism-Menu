cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # thanks dank_meme01
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")
    add_definitions(-DGLES_SILENCE_DEPRECATION)
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
include(cmake/CPM.cmake) # why do I need this, this didnt happen before

project(PrismMenu VERSION 1.0.0)

# Use GLOB_RECURSE instead of GLOB
# to recursively add all source files 
# under src/ 
file(GLOB SOURCES
	src/*.cpp
    src/Hacks/*.cpp
    src/Misc/*.cpp
    src/UI/*.cpp
    includes/gdr/*.hpp
)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (DEFINED ENV{LOCAL})
    message("Local Dev Build")
    add_definitions(-DDEV_BUILD)
endif()

if (DEFINED ENV{NO_IMGUI} OR APPLE OR ANDROID)
    message("Not compiling with ImGui")
    add_definitions(-DNO_IMGUI)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

#if (APPLE)
#    target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/FPSFix.mm)
#    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/FPSFix.mm PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
#endif()

#set(IMGUI_VERSION "v1.90")
if (NOT APPLE AND NOT DEFINED ENV{NO_IMGUI})
    CPMAddPackage("gh:matcool/gd-imgui-cocos#fbd4103") # specify a commit!
    target_link_libraries(${PROJECT_NAME} imgui-cocos)
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME})
