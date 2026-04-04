//
// Created by Christian Cruz on 12/20/25.
//

#ifndef WINDOWMAC_H
#define WINDOWMAC_H
#include <memory>

#include "../../../Core/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Source/Core/Events/iEvent.h"
#include "Source/Core/Events/InputEvents.h"
#include "Source/Core/Events/WindowEvent.h"


struct GLFWwindow;
struct ImVec2;

namespace Nebula {

      class WindowMac : public Window {
      public:
       explicit WindowMac(const WindowSpecification& spec);
       ~WindowMac() override;

       void Update() override;
       bool ShouldClose() const override;

       void RaiseEvent(iEvent& event);

       glm::vec2 GetFramebufferSize();
       glm::vec2 GetMousePos();
       GLFWwindow* getGLFWwindow() const { return m_Window; }

       float GetTime() override;

       void SetEventCallback(const EventCallbackFn& callback) override
       {
        m_Spec.EventCallback = callback;
       }

       void SetApplication(Application* app) override {
             m_App = app;
       };

       float GetWidth() const override { return m_Spec.width; }
       float GetHeight() const override { return m_Spec.height; }

       ImVec2 GetFramebufferSize() const override;

       Ref<ImGuiRenderer> CreateImGuiRenderer() override;

       void* GetNativeWindow() override { return m_Window; }
      private:
       GLFWwindow* m_Window = nullptr;
       WindowSpecification m_Spec;
       Application* m_App = nullptr;

      };

}

 // Nebula

#endif //WINDOWMAC_H
