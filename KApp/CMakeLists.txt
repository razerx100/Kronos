include_directories(../Kronos/src ../Kronos/vendor/spdlog/include ../Kronos/vendor/DirectXMath/Inc ../Kronos/src/Platform)

if(PLATFORM STREQUAL "Windows")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_executable(KApp src/KAppTest.cpp resources/KRONOS.ico resources/Kronos.rc)
	else()
		add_executable(KApp WIN32 src/KAppTest.cpp resources/KRONOS.ico resources/Kronos.rc)
	endif()
	set_target_properties(KApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}")
else()
	add_executable(KApp src/KAppTest.cpp)
	set_target_properties(KApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PLATFORM}/${CMAKE_BUILD_TYPE}")
endif()
target_link_libraries(KApp Kronos)
