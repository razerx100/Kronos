#include"WindowsWindow.hpp"
namespace Kronos{
    Window* Window::Create(const WindowProps& props){
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props){
        Init(props);
    }

    WindowsWindow::~WindowsWindow(){

    }

    void WindowsWindow::Init(const WindowProps& props){
        WNDCLASS wc = {};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(0);
        wc.lpszClassName = props.Title.c_str();

        RegisterClass(&wc);

        m_Hwnd = CreateWindowEx(
            0,
            props.Title.c_str(),
            props.Title.c_str(),
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT, CW_USEDEFAULT,
            props.Width,
            props.Height,
            NULL, NULL, GetModuleHandle(0), NULL
        );
    }

    void WindowsWindow::Shutdown(){

    }

    void WindowsWindow::OnUpdate(){

    }

    void WindowsWindow::SetVSync(bool enabled){
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSynced() const {
        return m_Data.VSync;
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(
        HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam
    ) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    HWND GetWinHandle(Window* win) {
        WindowsWindow* ptr = reinterpret_cast<WindowsWindow*>(win);
        return ptr->GetHWND();
    }
}