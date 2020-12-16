#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__
#include "Core.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Log.hpp"

namespace Kronos {
	class Application {
    private:
		static Application* s_Instance;
        Window* m_Window;
		bool m_Running;
		LayerStack m_LayerStack;

		bool OnWindowDestroy(WindowDestroyEvent& event);
		bool OnMouseMove(MouseMovedEvent& event);
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		virtual bool OnWindowClose(WindowCloseEvent& event);

		static Application& GetApp() { return *s_Instance; }
		inline Window* GetWindow() { return m_Window; }
	};
	Application* CreateApplication();

}
#endif
