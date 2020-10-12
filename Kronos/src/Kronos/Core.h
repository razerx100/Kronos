#ifndef __CORE_H__
#define __CORE_H__
	#ifdef KR_PLATFORM_WINDOWS
		#ifdef KR_BUILD_DLL
		#define KRONOS_API __declspec(dllexport)
		#else
		#define KRONOS_API __declspec(dllimport)
		#endif
	#else
	#error ONLY WINDOWS ATM
	#endif
#endif
