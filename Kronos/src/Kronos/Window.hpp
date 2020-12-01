#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__
#include"Core.hpp"
#include"Events/Event.hpp"
namespace Kronos{
    struct WindowProps{
        std::string Title;
        unsigned int Width, Height;

        WindowProps(const std::string& title = "Kronos Engine",
                    unsigned int width = 1980,
                    unsigned int height = 1080
        ) : Title(title), Width(width), Height(height) {}
    };

    class Window {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned GetWidth() const = 0;
        virtual unsigned GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;

        virtual void Show() {}
        virtual void Close() {}

        static Window* Create(const WindowProps& props = WindowProps());
    };
}
#endif