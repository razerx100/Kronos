#include "Application.hpp"
#include"KeyCodes.hpp"
#include"InputManager.hpp"

namespace Kronos {
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(true), m_Renderer(nullptr) {
		s_Instance = this;
		WindowProps prop = WindowProps("KEditor");
		m_Renderer = Renderer::Create(prop.Width, prop.Height);
        m_Window = Window::Create(prop);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->Show();
	}
	Application::~Application() {
        delete m_Window;
		delete m_Renderer;
	}
	void Application::Run() {
		while(m_Running){
			m_LayerStack.for_each([](Layer* layer) { layer->OnUpdate(); });

            m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowDestroyEvent>(BIND_EVENT_FN(Application::OnWindowDestroy));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMove));

		m_LayerStack.for_each_reverse([&event](Layer* layer) {
				layer->OnEvent(event);
				return event.handled;
			}, true);
	}
	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverLay(overlay);
		overlay->OnAttach();
	}
	bool Application::OnWindowDestroy(WindowDestroyEvent& event) {
		m_Running = false;
		return true;
	}
	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Window->Close();
		return false;
	}
	bool Application::OnMouseMove(MouseMovedEvent& event) {
		InputManager::setMousePos(event.GetX(), event.GetY());
		return false;
	}
}
