//
// Created by Christian Cruz on 12/18/25.
//

#include "Application.h"

#include <cassert>
#include <iostream>

#include "ImGuiLayer.h"
#include "Layer.h"
#include "Source/Platform/Mac/GLFWMacWindow/ImGuiLayerGLFW.h"
#include "RenderCommand.h"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h"
#include "Source/Platform/Mac/Metal/MetalWindow/ImGuiLayerMetal.h"
using namespace std;

namespace Nebula {
    static Application* s_Application = nullptr;

    Application::Application(const ApplicationSpecification& specification) : m_appSpec(specification)
    {
        s_Application = this;
        WindowSpecification winSpec = m_appSpec.window_specification;
        m_window = Window::Create(m_appSpec.window_specification);
        m_window->SetApplication(this);
        m_window->SetEventCallback([this](iEvent& event)
        {
            RaiseEvent(event);
        });
        RenderCommand::Create(m_appSpec.window_specification.GetAPI());

        RenderCommand::Init();
        m_LayerStack = new LayerStack();
    }

    Application::~Application() {
        s_Application = nullptr;
        delete m_Renderer;
    }

    void Application::Run()
    {
        switch (m_appSpec.window_specification.GetWindowType()) {
            case WindowType::GLFW:
            case WindowType::Metal:
                while (!m_window->ShouldClose()) {
                    m_window->Update();
                }
                break;
            case WindowType::None:
                break;
        }
    }


    void Application::RaiseEvent(iEvent& event)
    {
        for (auto it = m_LayerStack->begin(); it != m_LayerStack->end(); ++it)
        {
            (*it)->OnEvent(event);

            if (event.Handled)
                break;
        }
    }


    void Application::OnFrame()
    {
        float dl = m_window->GetTime();

        for (Layer* layer : *m_LayerStack)
             layer->OnUpdate(dl);

        for (Layer* layer: *m_LayerStack)
            layer->OnRender();

    }

    void Application::OnImGuiFrame() {
        for (Layer* layer : *m_LayerStack)
            layer->OnImGuiRender();
    }

    void Application::InitImGui()
    {
        if (!m_window) return;

        Ref<ImGuiRenderer> backend = m_window->CreateImGuiRenderer();
        if (!backend) return;

        m_ImGuiLayer = new ImGuiLayer(std::move(backend));
        PushLayer(m_ImGuiLayer);
    }

    Application& Application::Get()
    {
        assert(s_Application);
        return *s_Application;
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack->PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack->PushOverlay(layer);
    }

    LayerStack& Application::GetLayerStack()
    {
        return *m_LayerStack;
    }
} // Nebula