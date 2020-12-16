#ifndef __LOG_HPP__
#define __LOG_HPP__
#include"Core.hpp"
#include<memory>
#include<spdlog/spdlog.h>
#include<spdlog/sinks/stdout_color_sinks.h>
#include<spdlog/fmt/ostr.h>

namespace Kronos {
	class Log {
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
    public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

#define KR_CORE_TRACE(...) ::Kronos::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KR_CORE_WARN(...)  ::Kronos::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KR_CORE_INFO(...)  ::Kronos::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KR_CORE_ERROR(...) ::Kronos::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KR_CORE_FATAL(...) ::Kronos::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define KR_TRACE(...) ::Kronos::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KR_WARN(...)  ::Kronos::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KR_INFO(...)  ::Kronos::Log::GetClientLogger()->info(__VA_ARGS__)
#define KR_ERROR(...) ::Kronos::Log::GetClientLogger()->error(__VA_ARGS__)
#define KR_FATAL(...) ::Kronos::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif

