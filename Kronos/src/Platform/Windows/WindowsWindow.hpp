#ifndef __WINDOWS_WINDOW_HPP__
#define __WINDOWS_WINDOW_HPP__
#include<Kronos/Window.hpp>
#include"Windows/stdafx.hpp"
#include"Windows/DirectX12/Dx12Renderer.hpp"
namespace Kronos{
    class WindowsWindow : public Window {
    private:
        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFunc EventCallback;
        };

        WindowData m_Data;

    private:
        void Init(const WindowProps& props);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, WindowsWindow *window);

        static HWND s_Hwnd;
        MSG m_msg;
        WNDCLASSEX m_wc;
        DxRenderer* m_renderer;

    public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        inline void SetEventCallback(const EventCallbackFunc& callback) override { m_Data.EventCallback = callback; }
        inline static HWND GetHWND() { return s_Hwnd; }

        void OnUpdate() override;
        void Show() override;
        void Close() override;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
#endif