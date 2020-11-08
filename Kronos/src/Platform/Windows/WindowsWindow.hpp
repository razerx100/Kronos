#ifndef __WINDOWS_WINDOW_HPP__
#define __WINDOWS_WINDOW_HPP__
#include<Kronos/Window.hpp>
namespace Kronos{
    class WindowsWindow : public Window {
    private:
        class WindowData{
        public:
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFunc EventCallback;
        };

        WindowData m_Data;

    private:
        void Init(const WindowProps& props);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, WindowData data);
        HWND m_Hwnd;

    public:
        WindowsWindow(const WindowProps& props);

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        inline void SetEventCallback(const EventCallbackFunc& callback) override { m_Data.EventCallback = callback; }

        void Show() override;
        void SetVSync(bool enables) override;
        bool IsVSynced() const override;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
#endif