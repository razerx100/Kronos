#ifndef __CORE_HPP__
#define __CORE_HPP__
	#ifdef KR_PLATFORM_WINDOWS
		#ifdef KR_BUILD_DLL
		#define KRONOS_API __declspec(dllexport)
		#else
		#define KRONOS_API __declspec(dllimport)
		#endif
    #elif KR_PLATFORM_LINUX
    #define KRONOS_API
	#else
	#error ONLY WINDOWS ATM
	#endif
#endif
