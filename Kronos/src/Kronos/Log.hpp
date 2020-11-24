#ifndef __LOG_HPP__
#define __LOG_HPP__
#include"Core.hpp"
#include"imgui.h"
#include"Layer.hpp"
namespace Kronos {
	class Log {
        friend class LogLayer;
    private:
        static ImGuiTextBuffer     Buf;
        static ImGuiTextFilter     Filter;
        static ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        static bool                AutoScroll;  // Keep scrolling if already at the bottom.

		static void Init();
        static void Clear();
        static void Draw(const char* title, bool* p_open);

    public:
        static void AddLog(const char* fmt, ...) IM_FMTARGS(2);
	};

    class LogLayer : public Layer {
    public:
        LogLayer() : Layer("Log Window") { }

        void OnAttach() override;
        void OnImGuiRender() override;
    };
}

#define KR_TRACE(...) ::Kronos::Log::AddLog(__VA_ARGS__)

#endif

