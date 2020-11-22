#include<Kronos.hpp>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("Hello") {}

	void OnImGuiRender() override {
		ImGui::Begin("Hello World");
		ImGui::Text("OKay");
		ImGui::End();
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