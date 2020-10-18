cmake_minimum_required(VERSION 3.12)

project(KRONOS)

add_compile_definitions(KR_PLATFORM_LINUX)

add_subdirectory(Kronos)
add_subdirectory(Sandbox)
