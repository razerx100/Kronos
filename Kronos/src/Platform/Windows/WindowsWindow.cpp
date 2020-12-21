#include"WindowsWindow.hpp"
#include"Kronos/Events/ApplicationEvent.hpp"
#include"Kronos/Events/KeyEvent.hpp"
#include"Kronos/Events/MouseEvent.hpp"
#include"Windows/resources/resource.hpp"
#include"Kronos/Log.hpp"
#include"DirectX12/Dx12Renderer.hpp"
#include"Kronos/Application.hpp"

namespace Kronos {
    Window* Window::Create(const WindowProps& props){
        return new WindowsWindow(props);
    }

    HWND WindowsWindow::s_Hwnd = nullptr;

    WindowsWindow::WindowsWindow(const WindowProps& props)
        : m_wc{} {
        Init(props);
        ZeroMemory(&m_msg, sizeof(m_msg));
    }

    WindowsWindow::~WindowsWindow(){
        ::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
    }

    void WindowsWindow::Init(const WindowProps& props){
        m_Data.Title = props.Title;
        m_Data.Height = props.Height;
        m_Data.Width = props.Width;

        m_wc.cbSize = sizeof(WNDCLASSEX);
        m_wc.style = CS_HREDRAW | CS_VREDRAW;
        m_wc.lpfnWndProc = WindowProc;
        m_wc.hInstance = GetModuleHandle(0);
        m_wc.lpszClassName = props.Title.c_str();
        m_wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APP_ICON));
        m_wc.hIconSm = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 16, 16, 0);

        RegisterClassEx(&m_wc);

        RECT windowRect = {0, 0, static_cast<LONG>(m_Data.Width), static_cast<LONG>(m_Data.Height)};
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        s_Hwnd = CreateWindowEx(
            0,
            props.Title.c_str(),
            props.Title.c_str(),
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT, CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL, NULL, m_wc.hInstance, this
        );

        Application::GetApp().GetRenderer()->OnInit();
        //renderer->OnInit();
    }

    void WindowsWindow::Show() {
        ::ShowWindow(s_Hwnd, SW_SHOWNORMAL);
        ::UpdateWindow(s_Hwnd);
    }

    void WindowsWindow::Close() {
        Application::GetApp().GetRenderer()->OnDestroy();
        //renderer->OnDestroy();
        ::DestroyWindow(s_Hwnd);
    }

    void WindowsWindow::OnUpdate(){
        while (::PeekMessage(&m_msg, NULL, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&m_msg);
			::DispatchMessageA(&m_msg);
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
            PostQuitMessage(0);
        }
        return 0;
        case WM_MOVE:
        {
            WindowMovedEvent event;
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
            HDC hdc = BeginPaint(s_Hwnd, &ps);
            Application::GetApp().GetRenderer()->OnRender();
            //renderer->OnRender();
            EndPaint(s_Hwnd, &ps);
        }
        return 0;
        //Keyboard key events
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            KeyPressedEvent event(wParam);
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
        {
            MouseButtonPressedEvent event(0);
            data.EventCallback(event);
        }
        return 0;
        case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
        {
            MouseButtonPressedEvent event(1);
            data.EventCallback(event);
        }
        return 0;
        case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
        {
            MouseButtonPressedEvent event(2);
            data.EventCallback(event);
        }
        return 0;
        case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
        {
            int button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
        }
        return 0;
        case WM_LBUTTONUP:
        {
            MouseButtonReleasedEvent event(0);
            data.EventCallback(event);
        }
        return 0;
        case WM_RBUTTONUP:
        {
            MouseButtonReleasedEvent event(1);
            data.EventCallback(event);
        }
        return 0;
        case WM_MBUTTONUP:
        {
            MouseButtonReleasedEvent event(2);
            data.EventCallback(event);
        }
        return 0;
        case WM_XBUTTONUP:
        {
            int button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
        }
        return 0;
        default:
            return DefWindowProc(s_Hwnd, uMsg, wParam, lParam);
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

            pThis->s_Hwnd = hwnd;
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