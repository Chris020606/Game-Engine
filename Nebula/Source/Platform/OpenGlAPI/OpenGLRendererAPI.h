//
// Created by Christian Cruz on 12/21/25.
//

#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H
#include <functional>

#include "Source/Renderer/RendererAPI.h"
#include <glad/glad.h>


namespace Nebula {

class OpenGLRendererAPI : public RendererAPI {
    public:
        void Init() override;
        void BeginFrame() override;
        void EndFrame() override;
        void DrawIndexed(uint32_t count) override;
        void Draw() override;

        void RenderViewport(uint32_t width, uint32_t height) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void SetDepthTest(bool enabled) override;
        void SetBlending(bool enabled) override;
        void SetCulling(bool enabled) override;


        using ImGuiRenderCallback = std::function<void()>;
        void SetImGuiRenderCallback(std::function<void()> cb) override;
    private:
        ImGuiRenderCallback m_ImGuiRenderCallback;

};

} // Nebula

#endif //OPENGLRENDERERAPI_H
