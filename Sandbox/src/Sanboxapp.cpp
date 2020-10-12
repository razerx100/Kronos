#include<Kronos.h>
class Sandbox : public Kronos::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};
Kronos::Application* Kronos::CreateApplication() {
	return new Sandbox();
}