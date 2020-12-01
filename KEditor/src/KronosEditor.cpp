#include<Kronos.hpp>
class LogLayer : public Kronos::Layer {
public:
	LogLayer() : Layer("Log Window") { }

	void OnAttach() override {
		Kronos::Log::Init();
	}

	void OnImGuiRender() override {
		static bool show = true;
		if (show)
			Kronos::Log::Draw("Log", &show);
	}
};
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
		PushOverlay(new LogLayer());
		PushLayer(new ExampleLayer());
	}
	~KEditor() {

	}
};
Kronos::Application* Kronos::CreateApplication() {
	return new KEditor();
}