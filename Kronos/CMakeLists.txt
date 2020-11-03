include_directories(vendor/spdlog/include src)

if(PLATFORM STREQUAL "Windows")
    file(GLOB_RECURSE KR_SRC src/Kronos/*.hpp src/Kronos/*.cpp Kronos.hpp src/Platform/Windows/*.hpp src/Platform/Windows/*.cpp)
elseif(PLATFORM STREQUAL "Linux")
    # CHANGE THE PLATFORM DIRECTORY TO LINUX WHEN LINUX IMPLEMENTATION IS DONE
    file(GLOB_RECURSE KR_SRC src/Kronos/*.hpp src/Kronos/*.cpp Kronos.hpp src/Platform/Windows/*.hpp src/Platform/Windows/*.cpp)
endif()

add_library(Kronos SHARED ${KR_SRC})

if(PLATFORM STREQUAL "Windows")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_definitions(KR_ENABLE_ASSERTS)
    endif()
 	add_compile_definitions(KR_BUILD_DLL)
    set_target_properties(Kronos PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")
elseif(PLATFORM STREQUAL "Linux")
    set_target_properties(Kronos PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")
endif()
