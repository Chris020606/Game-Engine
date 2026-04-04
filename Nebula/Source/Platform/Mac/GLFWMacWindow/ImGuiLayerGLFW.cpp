//
// Created by Christian Cruz on 1/8/26.
//

#include "ImGuiLayerGLFW.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace Nebula {

// Constructor
ImGuiOpenGLRenderer::ImGuiOpenGLRenderer()
{
}

ImGuiOpenGLRenderer::~ImGuiOpenGLRenderer()
{
    Shutdown();
}

void ImGuiOpenGLRenderer::Init(void *window)
{
    if (!window) {
        std::printf("ERROR: GLFW window is null\n");
        return;
    }

    m_Window = static_cast<GLFWwindow*>(window);

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

}

bool ImGuiOpenGLRenderer::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    return true;
}

void ImGuiOpenGLRenderer::RenderDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
}

void ImGuiOpenGLRenderer::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

ImVec2 ImGuiOpenGLRenderer::GetDisplaySize() const
{
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return ImVec2((float)width, (float)height);
}


} // Nebula