include_directories(vendor/spdlog/include src)

if(PLATFORM STREQUAL "Windows")
	add_compile_definitions(KR_BUILD_DLL)
endif()

file(GLOB_RECURSE KR_SRC src/*.hpp src/*.cpp)

add_library(Kronos SHARED ${KR_SRC})

if(PLATFORM STREQUAL "Windows")
    target_precompile_headers(Kronos PRIVATE "Windows.h" <iostream> <memory> <utility> <algorithm> <functional> <string> <sstream> <vector>)
elseif(PLATFORM STREQUAL "Linux")
    target_precompile_headers(Kronos PRIVATE <iostream> <memory> <utility> <algorithm> <functional> <string> <sstream> <vector>)
endif()

if(PLATFORM STREQUAL "Windows")
	set_target_properties(Kronos PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")
endif()

set_target_properties(Kronos PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")