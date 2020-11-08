#include "Application.hpp"
namespace Kronos {
	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Running(true) {
		s_Instance = this;
        m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->Show();
	}
	Application::~Application() {
        delete m_Window;
	}
	void Application::Run() {
		while(m_Running){
			m_LayerStack.for_each([](Layer* layer) { layer->OnUpdate(); });
            m_Window->OnUpdate();
        }
	}
	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		m_LayerStack.for_each_reverse([&event](Layer* layer) {
				layer->OnEvent(event);
				return event.handled;
			}, true);
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
