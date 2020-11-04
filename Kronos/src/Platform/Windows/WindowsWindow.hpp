#ifndef __WINDOWS_WINDOW_HPP__
#define __WINDOWS_WINDOW_HPP__
#include<Kronos/Window.hpp>
namespace Kronos{
    class WindowsWindow : public Window {
    private:
        void Init(const WindowProps& props);
        void Shutdown();
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        HWND m_Hwnd;

    private:
        class WindowData{
        public:
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFunc EventCallback;
        };

        WindowData m_Data;

    public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallback(const EventCallbackFunc& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enables) override;
        bool IsVSynced() const override;

        inline HWND GetHWND() { return m_Hwnd; }
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
#endif