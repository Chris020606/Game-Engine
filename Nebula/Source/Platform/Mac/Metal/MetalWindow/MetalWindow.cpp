//
// Created by Christian Cruz on 3/21/26.
//

#include "MetalWindow.h"

#include <utility>

#include "glfw_adapter.h"
#include "../MetalAPI/MetalRendererAPI.h"

#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Events/iEvent.h"
#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Events/InputEvents.h"
#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Events/WindowEvent.h"
#include "Application.h"
#include "ImGuiLayerMetal.h"
#include "Render.h"

namespace Nebula {
    MetalWindow::MetalWindow(WindowSpecification spec)
        : m_WindowSpec(std::move(spec))
    {
        glfwInit();
        if (!glfwInit()) {
            return;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_GLFWwindow =  glfwCreateWindow(m_WindowSpec.width, m_WindowSpec.height, m_WindowSpec.title.c_str(), nullptr, nullptr);

        m_Device = MTL::CreateSystemDefaultDevice();
        MetalRendererAPI::SetDevice(m_Device);

        m_MetalLayer = CA::MetalLayer::layer()->retain();
        m_MetalLayer->setDevice(m_Device);
        m_MetalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);

        m_NSWindow = get_ns_window(m_GLFWwindow, m_MetalLayer)->retain();

        glfwSwapInterval(m_WindowSpec.VSync ? 1 : 0);

        glfwSetWindowUserPointer(m_GLFWwindow, this);

        SetCallBacks();

    }

    MetalWindow::~MetalWindow() {
        m_MetalLayer->release();
        m_NSWindow->release();
        glfwTerminate();
    }

    void MetalWindow::Update() {
        glfwPollEvents();

        if (!m_ImGuiInitialized && m_App) {
            m_App->InitImGui();
            m_ImGuiInitialized = true;
        }

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_GLFWwindow, &fbWidth, &fbHeight);
        m_MetalLayer->setDrawableSize({ (CGFloat)fbWidth, (CGFloat)fbHeight });

        CA::MetalDrawable* drawable = m_MetalLayer->nextDrawable();
        if (!drawable) { printf("ERROR: no drawable\n"); return; }

        auto* renderer = dynamic_cast<MetalRendererAPI*>(Renderer::GetRendererAPI());
        if (!renderer) { printf("ERROR: no renderer\n"); return; }

        renderer->SetCurrentDrawable(drawable);
        renderer->SetDrawableSize(
            static_cast<uint32_t>(fbWidth),
            static_cast<uint32_t>(fbHeight)
        );

        renderer->BeginFrame();

        if (m_App) {
            m_App->OnFrame();

            if (m_App->GetImGuiLayer()) {
                renderer->BeginImGuiPass();
                m_App->GetImGuiLayer()->Begin();
                m_App->OnImGuiFrame();
                m_App->GetImGuiLayer()->End();
                m_App->GetImGuiLayer()->RenderToGPU();
            } else {
                printf("ERROR: no ImGui layer\n");
            }
        }
        renderer->EndFrame();
    }

    bool MetalWindow::ShouldClose() const {
        return glfwWindowShouldClose(m_GLFWwindow);
    }

    float MetalWindow::GetTime() {
        return glfwGetTime();
    }

    void MetalWindow::SetEventCallback(const EventCallbackFn &callback) {
        m_WindowSpec.EventCallback = callback;
    }

    void MetalWindow::RaiseEvent(iEvent& event) const
    {
        if (m_WindowSpec.EventCallback)
            m_WindowSpec.EventCallback(event);
    }

    void MetalWindow::SetApplication(Application *app) {
        m_App = app;
    }

    float MetalWindow::GetHeight() const {
        return m_WindowSpec.height;
    }

    float MetalWindow::GetWidth() const {
        return m_WindowSpec.width;
    }

    void MetalWindow::SetCallBacks() {
        glfwSetWindowCloseCallback(m_GLFWwindow, [](GLFWwindow* handle)
        {
            MetalWindow& window = *((MetalWindow*)glfwGetWindowUserPointer(handle));

            WindowClosedEvent event;
            window.RaiseEvent(event);
        });

        glfwSetWindowSizeCallback(m_GLFWwindow, [](GLFWwindow* handle, int width, int height)
        {
            MetalWindow& window = *static_cast<MetalWindow *>(glfwGetWindowUserPointer(handle));

            WindowResizeEvent event((uint32_t)width, (uint32_t)height);
            window.RaiseEvent(event);
        });

        glfwSetKeyCallback(m_GLFWwindow, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
        {
            MetalWindow& window = *((MetalWindow*)glfwGetWindowUserPointer(handle));

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

        glfwSetMouseButtonCallback(m_GLFWwindow, [](GLFWwindow* handle, int button, int action, int mods)
        {
            MetalWindow& window = *((MetalWindow*)glfwGetWindowUserPointer(handle));

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

        glfwSetScrollCallback(m_GLFWwindow, [](GLFWwindow* handle, double xOffset, double yOffset)
        {
            MetalWindow& window = *((MetalWindow*)glfwGetWindowUserPointer(handle));

            MouseScrolledEvent event(xOffset, yOffset);
            window.RaiseEvent(event);
        });

        glfwSetCursorPosCallback(m_GLFWwindow, [](GLFWwindow* handle, double x, double y)
        {
            MetalWindow& window = *((MetalWindow*)glfwGetWindowUserPointer(handle));

            MouseMovedEvent event(x, y);;
            window.RaiseEvent(event);
        });
    }

    Ref<ImGuiRenderer> MetalWindow::CreateImGuiRenderer() {
        return std::make_shared<ImGuiLayerMetal>();
    }
} // Nebula