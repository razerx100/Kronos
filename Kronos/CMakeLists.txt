include_directories(vendor/spdlog/include)

file(GLOB_RECURSE KR_SRC src/*.hpp src/*.cpp)

add_library(Kronos SHARED ${KR_SRC})

set_target_properties(Kronos PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")