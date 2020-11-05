#include "Application.hpp"
namespace Kronos {

	Application::Application() : m_Running(true) {
        m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	Application::~Application() {
        delete m_Window;
	}
	void Application::Run() {
		while(m_Running){
            m_Window->OnUpdate();
        }
	}
	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	}
	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
}
