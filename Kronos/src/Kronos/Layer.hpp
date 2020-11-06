#ifndef __LAYER_HPP__
#define __LAYER_HPP__
#include "Core.hpp"
#include "Events/Event.hpp"
namespace Kronos {
	class KRONOS_API Layer {
	protected:
		std::string m_DebugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}
#endif