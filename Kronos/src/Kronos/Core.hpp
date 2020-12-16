#ifndef __CORE_HPP__
#define __CORE_HPP__
	//#ifdef KR_PLATFORM_WINDOWS
    //#elif KR_PLATFORM_LINUX
	//#else
	//#error NOT CORRECTLY DEFINED
	//#endif

	#ifdef KR_BUILD_DEBUG
		#define KR_ASSERT(x, ...) {if(!x) { KR_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
		#define KR_CORE_ASSERT(x, ...) {if(!x) { KR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#else
		#define KR_ASSERT(x, ...)
		#define KR_CORE_ASSERT(x, ...)
	#endif

	#define BIT(x) (1 << x)

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#endif
