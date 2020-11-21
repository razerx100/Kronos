#ifndef __INPUT_MANAGER_HPP__
#define __INPUT_MANAGER_HPP__
#include<Kronos/Core.hpp>
#include<Kronos/Application.hpp>
#include<utility>
namespace Kronos {
	class KRONOS_API InputManager {
		friend class Application;
	private:
		class Key {
		public:
			int repeatCount;

			Key() : repeatCount(0) {}

			Key& operator++() {
				this->repeatCount++;
				return *this;
			}

			Key& operator=(int value) {
				this->repeatCount = 0;
				return *this;
			}
		};
	private:
		static std::pair<float, float> m_MousePosition;
		static Key m_keys[0xFF];
	public:
		static bool isKeyPressed(short keyCode);
		static int getKeyRepeat(short keyCode);
		static bool isMouseKeyPressed(short keyCode);
		static inline std::pair<float, float> getMousePosition() { return m_MousePosition; }
	};
}
#endif