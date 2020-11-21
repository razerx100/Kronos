#include"InputManager.hpp"

namespace Kronos {
	std::pair<float, float> InputManager::m_MousePosition;
	InputManager::Key InputManager::m_keys[0xFF];

	bool InputManager::isKeyPressed(short keyCode) {
		return m_keys[keyCode].repeatCount;
	}
	int InputManager::getKeyRepeat(short keyCode) {
		return m_keys[keyCode].repeatCount;
	}
	bool InputManager::isMouseKeyPressed(short keyCode) {
		return true;
	}
}