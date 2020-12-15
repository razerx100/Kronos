cmake_minimum_required(VERSION 3.12)

project(KRONOS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(PLATFORM "Linux" CACHE STRING "Platform Name")

if(PLATFORM STREQUAL "Linux")
    add_compile_definitions(KR_PLATFORM_LINUX)
elseif(PLATFORM STREQUAL "Windows")
    add_compile_definitions(KR_PLATFORM_WINDOWS)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(KR_BUILD_DEBUG)
endif()

add_subdirectory(Kronos/vendor/imgui)
add_subdirectory(Kronos)
add_subdirectory(KApp)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT KApp)
