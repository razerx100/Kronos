#include<Kronos.hpp>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("KR") {}

	void OnUpdate() override {
		KR_CLIENT_INFO("Layer Update");
	}

	void OnEvent(Kronos::Event& event) override {
		KR_CLIENT_TRACE("{0}", event);
	}
};
class Sandbox : public Kronos::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};
Kronos::Application* Kronos::CreateApplication() {
	return new Sandbox();
}