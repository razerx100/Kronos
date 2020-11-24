#ifndef __EVENT_HPP__
#define __EVENT_HPP__
#include<string>
#include<ostream>
#include<functional>
#include<sstream>
#include"Kronos/Core.hpp"
namespace Kronos {
	enum class EventType {
		None = 0,
		WindowClose, WindowDestroy, WindowResize, WindowFocus, WindowLostFocus,
		WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		DeviceChanged
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput		 = BIT(1),
		EventCategoryKeyboard	 = BIT(2),
		EventCategoryMouse		 = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

    #define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::type; }\
                                    virtual EventType GetEventType() const override { return GetStaticType(); }\
                                    virtual const char* GetName() const override { return #type; }

    #define EVENT_CLASS_CATEGORY(category) virtual int GetCategory() const override { return category; }

    class Event {
		friend class EventDispatcher;
	public:
		bool handled = false;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategory() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategory() & category;
		}
	};

	class EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    private:
        Event& m_event;
    public:
        EventDispatcher(Event& event) : m_event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func){
            if(m_event.GetEventType() == T::GetStaticType()){
                m_event.handled = func(*(T*)&m_event);
                return true;
            }
            return false;
        }
	};

    inline std::ostream& operator<<(std::ostream& out, const Event& e){
        return out << e.ToString();
    }
}
#endif