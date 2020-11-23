#include<Kronos.hpp>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("Hello") {}

	void OnImGuiRender() override {
		static bool show = true;

		if (show) {
			ImGui::Begin("Hello World", &show);
			ImGui::Text("OKay");
			ImGui::End();
		}
	}

	void OnUpdate() override {
		if (Kronos::InputManager::isKeyPressed(Kronos::KeyCode::Tab))
			KR_TRACE("Tab is pressed %d.\n", 5);
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