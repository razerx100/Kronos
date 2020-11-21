#ifndef __KEY_EVENT_HPP__
#define __KEY_EVENT_HPP__
#include"Event.hpp"
namespace Kronos{
    class KRONOS_API KeyEvent : public Event{
    protected:
        int m_keycode;
        KeyEvent(int keycode) : m_keycode(keycode) {}
    public:
        inline int GetKeyCode() const { return m_keycode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class KRONOS_API KeyPressedEvent : public KeyEvent{
    public:
        KeyPressedEvent(int keycode) : KeyEvent(keycode) {}

        std::string ToString() const override{
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keycode << ".";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    };

    class KRONOS_API KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
        std::string ToString() const override{
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keycode << ".";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KRONOS_API KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(int keycode) : KeyEvent(keycode) {}
        std::string ToString() const override{
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_keycode << ".";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
#endif