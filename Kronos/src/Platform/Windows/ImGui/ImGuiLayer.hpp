#ifndef __IMGUI_LAYER_HPP__
#define __IMGUI_LAYER_HPP__
#include<Kronos/Layer.hpp>

namespace Kronos {
	class ImGuiLayer : public Layer {
	private:
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	};
}
#endif