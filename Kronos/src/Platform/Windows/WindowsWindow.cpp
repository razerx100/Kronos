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

    void WindowsWindow::OnUpdate(){
        while (::PeekMessage(&msg, m_Hwnd, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessageA(&msg);
		}
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
        }
        return 0;
        case WM_SIZE:
        {
            data.Width = LOWORD(lParam);
            data.Height = HIWORD(lParam);
            WindowResizeEvent event(data.Width, data.Height, (UINT)wParam);
            data.EventCallback(event);
        }
        return 0;
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case ID_FILE_EXIT:
                DestroyWindow(m_Hwnd);
                break;
            default:
                break;
            }
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