cmake_minimum_required(VERSION 3.12)

project(KRONOS)

set(PLATFORM "LINUX" CACHE STRING "Platform Name")

if(PLATFORM STREQUAL "LINUX")
    add_compile_definitions(KR_PLATFORM_LINUX)
elseif(PLATFORM STREQUAL "WINDOWS")
    add_compile_definitions(KR_PLATFORM_WINDOWS)
endif()

add_subdirectory(Kronos)
add_subdirectory(Sandbox)
