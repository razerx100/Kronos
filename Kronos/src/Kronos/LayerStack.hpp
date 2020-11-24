#ifndef __LAYER_STACK_HPP__
#define __LAYER_STACK_HPP__
#include <vector>
#include "Layer.hpp"
namespace Kronos {
	class LayerStack {
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*> m_Overlays;
	public:
		LayerStack() {}
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		void for_each(std::function<void(Layer*)> fn);
		void for_each(std::function<bool(Layer*)> fn, bool break_condition);
		void for_each_reverse(std::function<void(Layer*)> fn);
		void for_each_reverse(std::function<bool(Layer*)> fn, bool break_condition);
	};
}
#endif