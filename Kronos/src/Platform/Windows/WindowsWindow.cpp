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
        //WIN32 INIT
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
}