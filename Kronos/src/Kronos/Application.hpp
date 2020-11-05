#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__
#include "Core.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Window.hpp"
namespace Kronos {
	class KRONOS_API Application
	{
    private:
        Window* m_Window;
        bool m_Running;
	public:
		Application();
		virtual ~Application();

		inline Window* GetWindow() { return m_Window; }
		void Run();
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
	Application* CreateApplication();

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
}
#endif
