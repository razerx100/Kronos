include_directories(vendor/imgui vendor/DirectXMath/Inc src src/Platform)

if(PLATFORM STREQUAL "Windows")
    file(GLOB_RECURSE KR_SRC src/Kronos/*.hpp src/Kronos/*.cpp Kronos.hpp src/Platform/Windows/*.hpp src/Platform/Windows/*.cpp)
elseif(PLATFORM STREQUAL "Linux")
    # CHANGE THE PLATFORM DIRECTORY TO LINUX WHEN LINUX IMPLEMENTATION IS DONE
    file(GLOB_RECURSE KR_SRC src/Kronos/*.hpp src/Kronos/*.cpp Kronos.hpp src/Platform/Windows/*.hpp src/Platform/Windows/*.cpp)
endif()

add_library(Kronos ${KR_SRC})
target_link_libraries(Kronos ImGui)

