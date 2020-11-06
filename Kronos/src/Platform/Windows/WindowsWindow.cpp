#include"WindowsWindow.hpp"
#include"Kronos/Events/ApplicationEvent.hpp"
#include"Kronos/Events/KeyEvent.hpp"
#include"Kronos/Events/MouseEvent.hpp"
namespace Kronos{
    Window* Window::Create(const WindowProps& props){
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props){
        Init(props);
    }

    void WindowsWindow::Init(const WindowProps& props){
        m_Data.Title = props.Title;
        m_Data.Height = props.Height;
        m_Data.Width = props.Width;

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
            NULL, NULL, GetModuleHandle(0), this
        );
    }

    void WindowsWindow::OnUpdate(){

    }

    void WindowsWindow::SetVSync(bool enabled){
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSynced() const {
        return m_Data.VSync;
    }

    LRESULT WindowsWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, WindowData data) {
        switch (uMsg)
        {
        case WM_DESTROY:
        {
            WindowCloseEvent event;
            data.EventCallback(event);
            PostQuitMessage(0);
        }
        return 0;
        case WM_SIZE:
        {
            data.Width = LOWORD(lParam);
            data.Height = HIWORD(lParam);
            WindowResizeEvent event(data.Width, data.Height);
            data.EventCallback(event);
        }
        return 0;
        //Keyboard key events
        case WM_KEYDOWN:
        {
            KeyPressedEvent event((UINT)wParam, LOWORD(lParam));
            data.EventCallback(event);
        }
        return 0;
        case WM_KEYUP:
        {
            KeyReleasedEvent event((UINT)wParam);
            data.EventCallback(event);
        }
        return 0;
        //Mouse Events
        case WM_MOUSEWHEEL:
        {
            MouseScrolledEvent event(0, HIWORD(wParam));
            data.EventCallback(event);
        }
        return 0;
        case WM_MOUSEHWHEEL:
        {
            MouseScrolledEvent event(HIWORD(wParam), 0);
            data.EventCallback(event);
        }
        return 0;
        case WM_MOUSEMOVE:
        {
            POINTS pt = MAKEPOINTS(lParam);
            MouseMovedEvent event(pt.x, pt.y);
            data.EventCallback(event);
        }
        return 0;
        //Mouse Button Events
        case WM_LBUTTONDOWN:
        {
            MouseButtonPressedEvent event((UINT)MK_LBUTTON);
            data.EventCallback(event);
        }
        return 0;
        case WM_RBUTTONDOWN:
        {
            MouseButtonPressedEvent event((UINT)MK_RBUTTON);
            data.EventCallback(event);
        }
        return 0;
        case WM_MBUTTONDOWN:
        {
            MouseButtonPressedEvent event((UINT)MK_MBUTTON);
            data.EventCallback(event);
        }
        return 0;
        case WM_LBUTTONUP:
        {
            MouseButtonReleasedEvent event((UINT)MK_LBUTTON);
            data.EventCallback(event);
        }
        return 0;
        case WM_RBUTTONUP:
        {
            MouseButtonReleasedEvent event((UINT)MK_RBUTTON);
            data.EventCallback(event);
        }
        return 0;
        case WM_MBUTTONUP:
        {
            MouseButtonReleasedEvent event((UINT)MK_MBUTTON);
            data.EventCallback(event);
        }
        return 0;
        //Other events
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_Hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(m_Hwnd, &ps);
        }
        return 0;
        default:
            return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
        }
        return true;
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(
        HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam
    ) {
        WindowsWindow* pThis = NULL;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (WindowsWindow*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_Hwnd = hwnd;
        }
        else {
            pThis = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }
        if (pThis) {
            return pThis->HandleMessage(uMsg, wParam, lParam, pThis->m_Data);
        }
        else {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    HWND GetWinHandle(Window* win) {
        WindowsWindow* ptr = reinterpret_cast<WindowsWindow*>(win);
        return ptr->GetHWND();
    }
}