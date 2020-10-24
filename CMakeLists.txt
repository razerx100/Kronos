cmake_minimum_required(VERSION 3.16)

project(KRONOS)

set(PLATFORM "Linux" CACHE STRING "Platform Name")

if(PLATFORM STREQUAL "Linux")
    add_compile_definitions(KR_PLATFORM_LINUX)
elseif(PLATFORM STREQUAL "Windows")
    add_compile_definitions(KR_PLATFORM_WINDOWS)
endif()

add_subdirectory(Kronos)
add_subdirectory(Sandbox)
