#ifndef __ENTRY_POINT_HPP__
#define __ENTRY_POINT_HPP__
#include<thread>
	#ifdef KR_PLATFORM_WINDOWS
	extern Kronos::Application* Kronos::CreateApplication();
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		PWSTR pCmdLine, int nCmdShow) {
        Kronos::Log::Init();
		auto app = Kronos::CreateApplication();
		auto run = [&app]() { app->Run(); };
		std::thread th1(run);

		MSG msg = { };
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		th1.join();
		delete app;
		return 0;
	}
    #elif KR_PLATFORM_LINUX
	extern Kronos::Application* Kronos::CreateApplication();
	int main(int argc, char** argv) {
        Kronos::Log::Init();
		auto app = Kronos::CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
	#endif
#endif