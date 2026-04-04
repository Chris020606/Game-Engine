//
// Created by Christian Cruz on 2/12/26.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "RendererAPI.h"
#include "Window.h"
#include "Render.h"
#include <memory>

#include "Framebuffers.h"

namespace Nebula {

class RenderCommand {
public:
    static void Create(const API api) {
        s_RendererAPI = RendererAPI::Create(static_cast<RendererAPI::API>(api));
        RendererAPI::SetInstance(s_RendererAPI.get());
        Renderer::SetRendererAPI(s_RendererAPI);
    }

    static void Init() {
        s_RendererAPI->Init();
    }

    static void Shutdown() {
        s_RendererAPI->Shutdown();
    }

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    static void Clear() {
        s_RendererAPI->Clear();
    }

    static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

    static void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
        s_RendererAPI->DrawArrays(vertexArray, vertexCount);
    }

    static void SetDepthTest(bool enabled) {
        s_RendererAPI->SetDepthTest(enabled);
    }

    static void SetBlending(bool enabled) {
        s_RendererAPI->SetBlending(enabled);
    }

    static void RenderToFramebuffer(const Ref<Framebuffer>& fb) {
        s_RendererAPI->RenderToFramebuffer(fb.get());
    }

    static void BeginRenderPass() {
        s_RendererAPI->BeginRenderPass();
    }

private:
    static Ref<RendererAPI> s_RendererAPI;
};
} // Nebula

#endif //RENDERCOMMAND_H
