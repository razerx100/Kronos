#ifndef __APPLICATION_EVENT_HPP__
#define __APPLICATION_EVENT_HPP__
#include"Event.hpp"
namespace Kronos{
    class KRONOS_API WindowResizeEvent : public Event {
    private:
        unsigned int m_width, m_height, m_state;
    public:
        WindowResizeEvent(unsigned int width, unsigned int height, unsigned int state)
            : m_width(width), m_height(height), m_state(state) {}

        inline unsigned int GetWidth() const { return m_width; }
        inline unsigned int GetHeight() const { return m_height; }
        inline unsigned int GetState() const { return m_state; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height << ", " << m_state << ".";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class KRONOS_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class KRONOS_API WindowMovedEvent : public Event {
    public:
        WindowMovedEvent() {}

        EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class KRONOS_API WindowDestroyEvent : public Event {
    public:
        WindowDestroyEvent() {}

        EVENT_CLASS_TYPE(WindowDestroy)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class KRONOS_API DeviceChangedEvent : public Event {
    private:
        unsigned int m_EventData;
    public:
        DeviceChangedEvent(unsigned int eventData) : m_EventData(eventData) {}

        inline unsigned int GetEventData() const { return m_EventData; }

        std::string ToString() const {
            std::stringstream ss;
            ss << "Device changed event: " << m_EventData << ".";
            return ss.str();
        }
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
        EVENT_CLASS_TYPE(DeviceChanged)
    };

    class KRONOS_API AppTickEvent : public Event {
    public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class KRONOS_API AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class KRONOS_API AppRenderEvent : public Event {
    public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}
#endif