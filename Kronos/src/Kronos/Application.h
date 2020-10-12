#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include "Core.h"
namespace Kronos {
	class KRONOS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	Application* CreateApplication();
}
#endif
