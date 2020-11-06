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
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
            m_Window->OnUpdate();
        }
	}
	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.handled)
				break;
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverLay(overlay);
	}
}
