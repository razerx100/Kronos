#include<Kronos.hpp>
class ExampleLayer : public Kronos::Layer {
public:
	ExampleLayer() : Layer("Hello") {}

	void OnUpdate() override {
		if (Kronos::InputManager::isKeyPressed(Kronos::KeyCode::Tab))
			KR_TRACE("Tab is pressed");
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