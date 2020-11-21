#include"WindowsWindow.hpp"
#include"Kronos/Events/ApplicationEvent.hpp"
#include"Kronos/Events/KeyEvent.hpp"
#include"Kronos/Events/MouseEvent.hpp"
#include"Platform/Windows/resources/resource.hpp"

namespace Kronos {
    Window* Window::Create(const WindowProps& props){
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
        : wc{} {
        Init(props);
        ZeroMemory(&msg, sizeof(msg));
    }

    WindowsWindow::~WindowsWindow(){
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    }

    void WindowsWindow::Init(const WindowProps& props){
        m_Data.Title = props.Title;
        m_Data.Height = props.Height;
        m_Data.Width = props.Width;

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_CLASSDC;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(0);
        wc.lpszClassName = props.Title.c_str();
        wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APP_ICON));
        wc.hIconSm = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 16, 16, 0);

        RegisterClassEx(&wc);

        m_Hwnd = CreateWindowEx(
            0,
            props.Title.c_str(),
            props.Title.c_str(),
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT, CW_USEDEFAULT,
            props.Width,
            props.Height,
            NULL, NULL, wc.hInstance, this
        );
    }

    void WindowsWindow::Show() {
        ::ShowWindow(m_Hwnd, SW_SHOWNORMAL);
        ::UpdateWindow(m_Hwnd);
    }

    void WindowsWindow::Close() {
        ::DestroyWindow(m_Hwnd);
    }

    void WindowsWindow::OnUpdate(){
        while (::PeekMessage(&msg, m_Hwnd, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessageA(&msg);
		}
    }

    LRESULT WindowsWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, WindowData data) {
        switch (uMsg)
        {
        //Application events
        case WM_CLOSE:
        {
            WindowCloseEvent event;
            data.EventCallback(event);
        }
        return 0;
        case WM_DESTROY:
        {
            WindowDestroyEvent event;
            data.EventCallback(event);
        }
        return 0;
        case WM_SIZE:
        {
            data.Width = LOWORD(lParam);
            data.Height = HIWORD(lParam);
            WindowResizeEvent event(data.Width, data.Height, static_cast<UINT>(wParam));
            data.EventCallback(event);
        }
        return 0;
        case WM_PAINT:
        {

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_Hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(m_Hwnd, &ps);
        }
        return 0;
        //Keyboard key events
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            KeyPressedEvent event(wParam, LOWORD(lParam));
            data.EventCallback(event);
        }
        return 0;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            KeyReleasedEvent event(wParam);
            data.EventCallback(event);
        }
        return 0;
        case WM_CHAR:
        {
            KeyTypedEvent event(wParam);
            data.EventCallback(event);
        }
        return 0;
        //Mouse Events
        case WM_MOUSEWHEEL:
        {
            float yOffset = GET_WHEEL_DELTA_WPARAM(wParam) / static_cast<float>(WHEEL_DELTA);
            MouseScrolledEvent event(0, yOffset);
            data.EventCallback(event);
        }
        return 0;
        case WM_MOUSEHWHEEL:
        {
            float xOffset = GET_WHEEL_DELTA_WPARAM(wParam) / static_cast<float>(WHEEL_DELTA);
            MouseScrolledEvent event(xOffset, 0);
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
        case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
        {
            int button = 0;
            if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) { button = 0; }
            if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) { button = 1; }
            if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) { button = 2; }
            if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
        }
        return 0;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            int button = 0;
            if (uMsg == WM_LBUTTONUP) { button = 0; }
            if (uMsg == WM_RBUTTONUP) { button = 1; }
            if (uMsg == WM_MBUTTONUP) { button = 2; }
            if (uMsg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
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
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<WindowsWindow*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

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
}