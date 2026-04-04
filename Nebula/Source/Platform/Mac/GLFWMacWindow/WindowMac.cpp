//
// Created by Christian Cruz on 12/20/25.
//

#include "WindowMac.h"
#include <iostream>

#include "Application.h"
#include "imgui.h"

namespace Nebula {

    WindowMac::WindowMac(const WindowSpecification& spec)
        : m_Spec(spec)
    {
        if (!glfwInit()) {
            std::cerr << "Failed to init GLFW\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(
            m_Spec.width,
            m_Spec.height,
            m_Spec.title.c_str(),
            nullptr,
            nullptr
        );

        if (!m_Window) {
            std::cerr << "Failed to create window\n";
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to init GLAD\n";
        }

        glfwSwapInterval(m_Spec.VSync ? 1 : 0);


        glfwSetWindowUserPointer(m_Window, this);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* handle)
        {
            WindowMac& window = *((WindowMac*)glfwGetWindowUserPointer(handle));

            WindowClosedEvent event;
            window.RaiseEvent(event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* handle, int width, int height)
        {
            WindowMac& window = *((WindowMac*)glfwGetWindowUserPointer(handle));

            WindowResizeEvent event((uint32_t)width, (uint32_t)height);
            window.RaiseEvent(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
        {
            WindowMac& window = *((WindowMac*)glfwGetWindowUserPointer(handle));

            switch (action)
            {
                case GLFW_PRESS:
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, action == GLFW_REPEAT);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    window.RaiseEvent(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* handle, int button, int action, int mods)
        {
            WindowMac& window = *((WindowMac*)glfwGetWindowUserPointer(handle));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    window.RaiseEvent(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* handle, double xOffset, double yOffset)
        {
            WindowMac& window = *((WindowMac*)glfwGetWindowUserPointer(handle));

            MouseScrolledEvent event(xOffset, yOffset);
            window.RaiseEvent(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* handle, double x, double y)
        {
            WindowMac& window = *((WindowMac*)glfwGetWindowUserPointer(handle));

            MouseMovedEvent event(x, y);
            window.RaiseEvent(event);
        });

        int fbW, fbH, winW, winH;
        glfwGetFramebufferSize(m_Window, &fbW, &fbH);
        glfwGetWindowSize(m_Window, &winW, &winH);

        printf("Framebuffer: %d x %d | Window: %d x %d\n", fbW, fbH, winW, winH);

    }

    WindowMac::~WindowMac() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void WindowMac::Update() {
        glfwMakeContextCurrent(m_Window);
        auto* renderer = Renderer::GetRendererAPI();

        if (renderer) {
            renderer->Draw();
        }

        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    bool WindowMac::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    glm::vec2 WindowMac::GetFramebufferSize() {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return { width, height };
    }

    glm::vec2 WindowMac::GetMousePos()
    {
        double x, y;
        glfwGetCursorPos(m_Window, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    float WindowMac::GetTime()
    {
        return (float)glfwGetTime();
    }

    void WindowMac::RaiseEvent(iEvent& event)
    {
        if (m_Spec.EventCallback)
            m_Spec.EventCallback(event);
    }

    ImVec2 WindowMac::GetFramebufferSize() const {
        int w, h;
        glfwGetFramebufferSize(m_Window, &w, &h);
        return { (float)w, (float)h };
    }

    Ref<ImGuiRenderer> WindowMac::CreateImGuiRenderer() {
    }
} // Nebula