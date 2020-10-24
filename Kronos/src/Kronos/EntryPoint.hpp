#ifndef __ENTRY_POINT_HPP__
#define __ENTRY_POINT_HPP__
	#ifdef KR_PLATFORM_WINDOWS
	extern Kronos::Application* Kronos::CreateApplication();
	int main(int argc, char** argv) {
        Kronos::Log::Init();
		auto app = Kronos::CreateApplication();
		app->Run();
		delete app;
	}
    #elif KR_PLATFORM_LINUX
	extern Kronos::Application* Kronos::CreateApplication();
	int main(int argc, char** argv) {
        Kronos::Log::Init();
		auto app = Kronos::CreateApplication();
		app->Run();
		delete app;
	}
	#endif
#endif