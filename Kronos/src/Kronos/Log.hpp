#ifndef __LOG_HPP__
#define __LOG_HPP__
#include"Core.hpp"
#include"imgui.h"
namespace Kronos {
	class Log {
    private:
        static ImGuiTextBuffer     Buf;
        static ImGuiTextFilter     Filter;
        static ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        static bool                AutoScroll;  // Keep scrolling if already at the bottom.
        static void Clear();

    public:
		static void Init();
        static void Draw(const char* title, bool* p_open);

        static void AddLog(const char* fmt, ...) IM_FMTARGS(2);
	};
}

#define KR_TRACE(...) ::Kronos::Log::AddLog(__VA_ARGS__)

#endif

