#include "Application.hpp"
namespace Kronos {
	Application::Application() : m_Running(true) {
        m_Window = Window::Create();
	}
	Application::~Application() {
        delete m_Window;
	}
	void Application::Run() {
		while(m_Running){
            m_Window->OnUpdate();
        }
	}
}
