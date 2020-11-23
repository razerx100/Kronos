#ifndef __CORE_HPP__
#define __CORE_HPP__
	#ifdef KR_PLATFORM_WINDOWS
		#include<Windows.h>
		//#ifdef KR_BUILD_DLL
		//#define KRONOS_API __declspec(dllexport)
		//#else
		//#define KRONOS_API __declspec(dllimport)
		//#endif
		#define KRONOS_API
    #elif KR_PLATFORM_LINUX
    #define KRONOS_API
	#else
	#error NOT CORRECTLY DEFINED
	#endif

	#define BIT(x) (1 << x)

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#endif
