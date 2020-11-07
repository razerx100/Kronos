#include "LayerStack.hpp"
namespace Kronos {
	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers)
			delete layer;
		for (Layer* layer : m_Overlays)
			delete layer;
	}
	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace_back(layer);
	}
	void LayerStack::PushOverLay(Layer* overlay) {
		m_Overlays.emplace_back(overlay);
	}
	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
	void LayerStack::for_each(std::function<void(Layer*)> fn) {
		std::vector<Layer*>::iterator ptr = m_Layers.begin();
		while (ptr != m_Layers.end())
			fn(*ptr++);
		ptr = m_Overlays.begin();
		while (ptr != m_Overlays.end())
			fn(*ptr++);
	}
	//Argument function should return the break_condition.
	//If returned value is true, for_each will exit.
	//Initial value of break_condition argument doesn't matter.
	void LayerStack::for_each(std::function<bool(Layer*)> fn, bool break_condition) {
		std::vector<Layer*>::iterator ptr = m_Layers.begin();
		while (ptr != m_Layers.end()) {
			break_condition = fn(*ptr++);
			if (break_condition)
				return;
		}
		ptr = m_Overlays.begin();
		while (ptr != m_Overlays.end()) {
			break_condition = fn(*ptr++);
			if (break_condition)
				return;
		}
	}
	void LayerStack::for_each_reverse(std::function<void(Layer*)> fn) {
		std::vector<Layer*>::iterator ptr = m_Overlays.end();
		while (ptr != m_Overlays.begin())
			fn(*(--ptr));
		ptr = m_Layers.end();
		while (ptr != m_Layers.begin())
			fn(*(--ptr));
	}
	//Argument function should return the break_condition.
	//If returned value is true, for_each_reverse will exit.
	//Initial value of break_condition argument doesn't matter.
	void LayerStack::for_each_reverse(std::function<bool(Layer*)> fn, bool break_condition) {
		std::vector<Layer*>::iterator ptr = m_Overlays.end();
		while (ptr != m_Overlays.begin()) {
			break_condition = fn(*(--ptr));
			if (break_condition)
				return;
		}
		ptr = m_Layers.end();
		while (ptr != m_Layers.begin()) {
			break_condition = fn(*(--ptr));
			if (break_condition)
				return;
		}
	}
}