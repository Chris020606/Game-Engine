//
// Created by Christian Cruz on 12/22/25.
//

#ifndef IEVENT_H
#define IEVENT_H
#include <functional>
#include <iostream>

namespace Nebula {
    // Event categories for filtering
    enum class EventCategory {
        None = 0,
        Application = 1 << 0,
        Input = 1 << 1,
        Keyboard = 1 << 2,
        Mouse = 1 << 3,
        MouseButton = 1 << 4,
        Window = 1 << 5
    };
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        ToggleImGui
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) \
virtual int GetCategoryFlags() const override { return static_cast<int>(category); }


    class iEvent
    {
    public:
        bool Handled = false;

        virtual ~iEvent() = default;
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual std::string ToString() const { return GetName(); }
        virtual int GetCategoryFlags() const = 0;
        inline bool IsInCategory(EventCategory category) const {
            return GetCategoryFlags() & static_cast<int>(category);
        }
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(iEvent& event)
            : m_Event(event) { }

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType() && !m_Event.Handled)
            {
                m_Event.Handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }
    private:
        iEvent& m_Event;
    };

    class Input {
    public:
        static bool IsKeyPressed(int keycode)   { return s_Keys[keycode]; }
        static bool IsMouseButtonPressed(int b) { return s_Buttons[b];    }

        static void SetKeyState(int keycode, bool pressed)   { s_Keys[keycode]    = pressed; }
        static void SetMouseState(int button,  bool pressed) { s_Buttons[button]  = pressed; }

    private:
        static std::unordered_map<int, bool> s_Keys;
        static std::unordered_map<int, bool> s_Buttons;
    };

} // Nebula

#endif //IEVENT_H
