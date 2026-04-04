//
// Created by Christian Cruz on 12/21/25.
//

#include "OpenGLRendererAPI.h"

#include <cstdio>

#include "Source/Platform/Mac/GLFWMacWindow/ImGuiLayerGLFW.h"


namespace Nebula {

    void OpenGLRendererAPI::Init() {

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGLRendererAPI::BeginFrame() {
        //auto& app = Application::Get();
        //ImVec2 fb = app.GetWindow()->GetFramebufferSize();

        //glViewport(0, 0, (GLsizei)fb.x, (GLsizei)fb.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(uint32_t count) {
        glDrawElements(GL_TRIANGLES, int(count), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::EndFrame() {

    }

    void OpenGLRendererAPI::Draw() {
        BeginFrame();
        // Render ImGui
        if (m_ImGuiRenderCallback) { // this is not being called
            std::printf("OpenGL RendererAPI::Draw()\n");
            m_ImGuiRenderCallback();
        }
        EndFrame();
    }

    void OpenGLRendererAPI::RenderViewport(uint32_t width, uint32_t height) {

    }

    void OpenGLRendererAPI::SetImGuiRenderCallback(std::function<void()> cb)
    {
        m_ImGuiRenderCallback = cb;
        std::printf("SetImGuiRenderCallback on OpenGLRendererAPI: %p\n", this);
    }


} // Nebula