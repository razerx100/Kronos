#include<Kronos.hpp>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("KR") {}

	void OnUpdate() override {
		if (Kronos::InputManager::isKeyPressed(Kronos::KeyCode::BraceStartUS) && Kronos::InputManager::isKeyPressed(Kronos::KeyCode::Shift))
			KR_CLIENT_INFO("{");
		else if (Kronos::InputManager::isKeyPressed(Kronos::KeyCode::BraceStartUS))
			KR_CLIENT_INFO("[");
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