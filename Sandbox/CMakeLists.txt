include_directories(../Kronos/vendor/spdlog/include ../Kronos/src)

add_executable(Sandbox src/Sandboxapp.cpp)

target_link_libraries(Sandbox Kronos)

set_target_properties(Sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")