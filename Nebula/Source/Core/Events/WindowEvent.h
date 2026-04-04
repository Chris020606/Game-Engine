//
// Created by Christian Cruz on 12/26/25.
//

#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H
#pragma once


#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Events/iEvent.h"
#include <sstream>
#include <string>



namespace Nebula {
    class WindowClosedEvent : public iEvent
    {
    public:
        WindowClosedEvent() = default;

        EVENT_CLASS_TYPE(WindowClose);
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class WindowResizeEvent : public iEvent {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_EventWidth(width), m_EventHeight(height) {}

        [[nodiscard]] inline uint32_t GetWidth()  const { return m_EventWidth;  }
        [[nodiscard]] inline uint32_t GetHeight() const { return m_EventHeight; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_EventWidth << ", " << m_EventHeight;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategory::Application)

    private:
        uint32_t m_EventWidth, m_EventHeight;
    };

    class WindowFocusEvent : public iEvent {
    public:
        WindowFocusEvent() = default;

        EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class WindowLostFocusEvent : public iEvent {
    public:
        WindowLostFocusEvent() = default;

        EVENT_CLASS_TYPE(WindowLostFocus)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class WindowMovedEvent : public iEvent {
    public:
        WindowMovedEvent(int x, int y) : m_X(x), m_Y(y) {}

        inline int GetX() const { return m_X; }
        inline int GetY() const { return m_Y; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowMovedEvent: " << m_X << ", " << m_Y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(EventCategory::Application)

    private:
        int m_X, m_Y;
    };

    class ToggleImGuiEvent : public Nebula::iEvent {
    public:
        EVENT_CLASS_TYPE(ToggleImGui)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
    };
}

#endif //WINDOWEVENT_H
