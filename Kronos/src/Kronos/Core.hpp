#ifndef __CORE_HPP__
#define __CORE_HPP__
	#ifdef KR_PLATFORM_WINDOWS
		#include<Windows.h>
    //#elif KR_PLATFORM_LINUX
	#else
	#error NOT CORRECTLY DEFINED
	#endif

	#define BIT(x) (1 << x)

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#endif
