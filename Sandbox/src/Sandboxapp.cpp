#include<Kronos.hpp>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("KR") {}

	void OnUpdate() override {
		if (Kronos::InputManager::isKeyPressed(Kronos::KeyCode::Backspace))
			KR_CLIENT_INFO("Backspace pressed {0}", Kronos::InputManager::getKeyRepeat(Kronos::KeyCode::Backspace));
	}

	void OnEvent(Kronos::Event& event) override {}
};
class Sandbox : public Kronos::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Kronos::ImGuiLayer());
	}
	~Sandbox() {

	}
};
Kronos::Application* Kronos::CreateApplication() {
	return new Sandbox();
}