if(PLATFORM STREQUAL "Windows")
    include_directories(vendor/spdlog/include vendor/imgui vendor/DirectXMath/Inc src)
    file(GLOB_RECURSE KR_SRC src/Kronos/*.hpp src/Kronos/*.cpp Kronos.hpp src/Platform/Windows/*.hpp src/Platform/Windows/*.cpp)
elseif(PLATFORM STREQUAL "Linux")
    include_directories(vendor/spdlog/include vendor/imgui vendor/DirectXMath/Inc src src/Platform/Linux)
    # CHANGE THE PLATFORM DIRECTORY TO LINUX WHEN LINUX IMPLEMENTATION IS DONE
    file(GLOB_RECURSE KR_SRC src/Kronos/*.hpp src/Kronos/*.cpp Kronos.hpp src/Platform/Windows/*.hpp src/Platform/Windows/*.cpp src/Platform/Linux/sal.h)
endif()

add_library(Kronos SHARED ${KR_SRC})
target_link_libraries(Kronos ImGui)

if(PLATFORM STREQUAL "Windows")
 	add_compile_definitions(KR_BUILD_DLL)
    set_target_properties(Kronos PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}")
elseif(PLATFORM STREQUAL "Linux")
    set_target_properties(Kronos PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")
endif()
