#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__
#include "Core.hpp"
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
	};
	Application* CreateApplication();
}
#endif
