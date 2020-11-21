#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__
#include<Kronos/Core.hpp>
#include<Kronos/Application.hpp>
#include<utility>
namespace Kronos {
	class KRONOS_API InputManager {
	private:
		static std::pair<float, float> m_MousePosition;
	public:
		static inline void setMousePos(float x, float y) { m_MousePosition = { x, y }; }
		static bool isKeyPressed(short keyCode);
		static bool isKeyToggled(short keyCode);
		static bool isMouseKeyPressed(short keyCode);
		static inline std::pair<float, float> getMousePosition() { return m_MousePosition; }
	};
}
#endif