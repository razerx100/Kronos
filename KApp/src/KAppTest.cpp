#include<Kronos.hpp>
#include<ctime>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("Hello") {
	}

	void OnUpdate() override {

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