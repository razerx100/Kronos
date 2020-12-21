#include<Kronos.hpp>
#include<ctime>
class ExampleLayer : public Kronos::Layer {
public:
	std::clock_t start;
	double duration;
	int num;

	ExampleLayer() : Layer("Hello"), num(0) {
		start = std::clock();
	}

	void OnUpdate() override {
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		if (duration > 2) {
			start = std::clock();
			Kronos::Application::GetApp().GetRenderer()->ChangeTriangleColor(num);
		}
	}
};
class KEditor : public Kronos::Application {
public:
	KEditor() {
		PushLayer(new ExampleLayer());
	}
	~KEditor() {

	}
};
Kronos::Application* Kronos::CreateApplication() {
	return new KEditor();
}