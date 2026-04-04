//
// Created by Christian Cruz on 1/8/26.
//

#include "ImGuiLayer.h"
#include <imgui.h>

#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "Application.h"
#include "Source/Platform/Mac/Metal/MetalWindow/ImGuiLayerMetal.h"

namespace Nebula {
    ImGuiLayer::ImGuiLayer(Ref<ImGuiRenderer> backend)
       : m_Backend(std::move(backend)) {
    }

    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::OnAttach() {
        // Initialize ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Configure ImGui
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGuiStyle& style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }


        ImGuizmo::SetOrthographic(false);
        // Initialize backend renderer
        auto* window = Application::Get().GetWindow();
        m_Backend->Init(window->GetNativeWindow());
    }

    void ImGuiLayer::OnDetach() {
        // Shutdown backend and destroy context
        m_Backend->Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin()
    {
        if (!ImGui::GetCurrentContext())
            return;

        IM_ASSERT(ImGui::GetCurrentContext() && "ImGui context does not exist!");


        ImGuiIO& io = ImGui::GetIO();
        ImVec2 size = m_Backend->GetDisplaySize();
        io.DisplaySize = size;

        m_Backend->NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        // if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        //     ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        }
    }

    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiLayer::RenderToGPU() {
        m_Backend->RenderDrawData();
    }

    void ImGuiLayer::OnImGuiRender() {
    }

    void ImGuiLayer::BeginDockspace() {

    }

    void ImGuiLayer::EndDockspace() {

    }

}
