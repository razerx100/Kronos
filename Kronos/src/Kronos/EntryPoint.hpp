#ifndef __ENTRY_POINT_HPP__
#define __ENTRY_POINT_HPP__
	#ifdef KR_PLATFORM_WINDOWS
		extern Kronos::Application* Kronos::CreateApplication();
		#ifdef KR_BUILD_DEBUG
		int main(int, char**) {
			Kronos::Log::Init();
			auto app = Kronos::CreateApplication();
			app->Run();
			delete app;
			return 0;
		}
		#else
		int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			PWSTR pCmdLine, int nCmdShow) {
			auto app = Kronos::CreateApplication();
			app->Run();
			delete app;
			return 0;
		}
		#endif
    #elif KR_PLATFORM_LINUX
	extern Kronos::Application* Kronos::CreateApplication();
	int main(int, char**) {
		Kronos::Log::Init();
		auto app = Kronos::CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
	#endif
#endif