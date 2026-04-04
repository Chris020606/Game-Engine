//
// Created by Christian Cruz on 12/18/25.
//

#ifndef WINDOW_H
#define WINDOW_H

#pragma once
#include "Events/iEvent.h"

#include <memory>
#include <string>
#include <functional>

#include "Core.h"
#include "imgui.h"


namespace Nebula {
    class Application;
}

namespace Nebula {
    class ImGuiRenderer;

    enum class WindowType {
        GLFW,
        Metal,
        None
    };

    enum class API {
        OpenGL = 1,
        Metal = 2,
        None = 0
    };

    struct WindowSpecification {
        float width = 1200.0f;
        float height = 800.0f;
        std::string title = "Nebula";
        bool IsResizable = true;
        bool VSync = true;
        mutable bool IsImGuiEnabled = true;

        using EventCallbackFn = std::function<void(iEvent&)>;
        EventCallbackFn EventCallback;

         WindowType windowType = WindowType::None;
         API api = API::Metal;
         API GetAPI() const { return api; }
         WindowType GetWindowType() const { return windowType; }
    };

    class Window {
    public:
        virtual ~Window() = default;
        virtual void Update() = 0;
        virtual bool ShouldClose() const = 0;
        virtual float GetTime() = 0;

        static std::unique_ptr<Window> Create(const WindowSpecification& spec);
        using EventCallbackFn = std::function<void(iEvent&)>;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetApplication(Application* app) = 0;
        virtual void* GetNativeWindow() = 0;

        virtual float GetWidth() const = 0;
        virtual float GetHeight() const = 0;

        virtual ImVec2 GetFramebufferSize() const = 0;

        virtual Ref<ImGuiRenderer> CreateImGuiRenderer() = 0;
    };

} // Nebula

#endif //WINDOW_H
