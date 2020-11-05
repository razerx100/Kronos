#ifndef __CORE_HPP__
#define __CORE_HPP__
	#ifdef KR_PLATFORM_WINDOWS
		#include<Windows.h>
		#ifdef KR_BUILD_DLL
		#define KRONOS_API __declspec(dllexport)
		#else
		#define KRONOS_API __declspec(dllimport)
		#endif
    #elif KR_PLATFORM_LINUX
    #define KRONOS_API
	#else
	#error NOT CORRECTLY DEFINED
	#endif

    #ifdef KR_ENABLE_ASSERTS
        #define KR_ASSERT(x, ...) {if(!x) { KR_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
        #define KR_CORE_ASSERT(x, ...) {if(!x) { KR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
    #else
        #define KR_ASSERT(x, ...)
        #define KR_CORE_ASSERT(x, ...)
    #endif

	#define BIT(x) (1 << x)
#endif
