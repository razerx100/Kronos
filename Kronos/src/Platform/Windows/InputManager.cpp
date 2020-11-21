#include"Kronos/InputManager.hpp"

namespace Kronos {
	std::pair<float, float> InputManager::m_MousePosition;

	bool InputManager::isKeyPressed(short keyCode) {
		return HIWORD(GetKeyState(keyCode));
	}
	bool InputManager::isKeyToggled(short keyCode) {
		return LOWORD(GetKeyState(keyCode));
	}
	bool InputManager::isMouseKeyPressed(short keyCode) {
		return HIWORD(GetKeyState(keyCode));
	}
}