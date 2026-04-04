//
// Created by Christian Cruz on 12/21/25.
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H
#include <cstdint>
#include <functional>
#include <glm/glm.hpp>
#include <utility>
#include "VertexArray.h"
#include "Mesh.h"
#include "Texture.h"
#include "Core.h"

namespace Nebula {
    class Framebuffer;

    class RendererAPI {
    public:
        enum class API {
            None = 0,
            OpenGL,
            Metal
        };

        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void BeginRenderPass() = 0;
        virtual void EndFrame() = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void Draw() = 0;
        virtual void DrawIndexed(uint32_t count) = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;
        virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
        virtual void DrawLines(int startPosX, int startPosY, int endPosX, int endPosY) = 0;
        virtual void DrawMesh(Ref<Mesh> &mesh) = 0;
        virtual void RenderToFramebuffer( Framebuffer* framebuffer ) = 0;

        virtual void RenderViewport(uint32_t width, uint32_t height) = 0;
        void* GetViewportTexture();

        using ImGuiRenderCallback = std::function<void()>;

        virtual void SetImGuiRenderCallback(ImGuiRenderCallback callback) { m_ImGuiRenderCallback = std::move(callback);}

        virtual void SetDepthTest(bool enabled) = 0;
        virtual void SetBlending(bool enabled) = 0;
        virtual void SetCulling(bool enabled) = 0;

        static API GetAPI() { return s_API; };
        static void SetAPI(API api) { s_API = api; }
        virtual class MetalRendererAPI* AsMetal() { return nullptr; }
        static RendererAPI* Get() { return s_Instance; }
        static void SetInstance(RendererAPI* instance) { s_Instance = instance; }

        static Ref<RendererAPI> Create(const RendererAPI::API api);

        private:
        static API s_API;
        static RendererAPI* s_Instance;
        ImGuiRenderCallback m_ImGuiRenderCallback;
    };

}


#endif //RENDERERAPI_H
