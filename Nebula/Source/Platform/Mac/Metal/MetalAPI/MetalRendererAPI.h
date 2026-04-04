//
// Created by Christian Cruz on 12/27/25.
//

#ifndef METALRENDERERAPI_H
#define METALRENDERERAPI_H

/// Metal Renderer API
/// This file is responsable for almost all metal api implementation like draw commands
/// Also settings such as SetViewport, ClearColor, SetDepth, SetCulling
/// Uses MSAA for better quality

// Forward declarations
namespace CA {
    class MetalLayer;
    class MetalDrawable;
}
namespace MTL {
    class Device;
    class CommandQueue;
    class CommandBuffer;
    class RenderCommandEncoder;
    class RenderPassDescriptor;
    class DepthStencilState;
    class Texture;
    class Buffer;
    class RenderPipelineState;
    class ComputePipelineState;
    class Library;
    class SamplerState;
    class SamplerDescriptor;
}
namespace NS {
    class AutoreleasePool;
}
namespace MTK {
    class View;
}

#include "Source/Renderer/RendererAPI.h"
#include <glm/glm.hpp>
#include <functional>
#include <unordered_map>

namespace Nebula {

    class Framebuffer;

    class MetalRendererAPI : public RendererAPI {
    public:
        MetalRendererAPI() = default;
        ~MetalRendererAPI() override;

        void Init() override; // Constructor
        void Shutdown() override; // Releases all the memory

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override; // Set Viewports t
        void SetClearColor(const glm::vec4& color) override; // Clear with a color parameter
        void Clear() override; // Clears the screen

        void Draw() override; // This was useful but don't need it
        void DrawIndexed(uint32_t count) override; // This version don't use it
        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override; // Draw vertex array using index count
        void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override; // Draw vertex array using vertecis
        void DrawMesh(Ref<Mesh> &mesh) override; // Call DrawIndexed internally
        void DrawLines(int startPosX, int startPosY, int endPosX, int endPosY) override; // Draw lines

        void SetDepthTest(bool enabled) override; // Set Depth Test
        void SetBlending(bool enabled) override; // Set Blending(Need implementation)
        void SetCulling(bool enabled) override; // Set Culling

        void BeginFrame() override; // Begins each frame
        void EndFrame() override; // End each frame
        void BeginImGuiPass();
        void BeginRenderPass() override;
        static void EndCurrentEncoder();

        void RenderToFramebuffer(Framebuffer* framebuffer) override; // It renders to a framebuffer
        void RenderViewport(uint32_t width, uint32_t height) override {};

        // Setters
        static void SetDevice(MTL::Device* device); // Set the Device
        void SetImGuiRenderCallback(ImGuiRenderCallback callback) override { m_ImGuiRenderCallback = callback; }
        void SetCurrentDrawable(CA::MetalDrawable* drawable) { m_CurrentDrawable = drawable; }
        void SetDrawableSize(uint32_t width, uint32_t height) { m_DrawableWidth  = width; m_DrawableHeight = height; }

        // Getters
        static MTL::Device* GetDevice() { return s_Device;}
        // MTL::CommandBuffer* GetCommandBuffer() const { return _pCommandBuffer; }
        static MTL::CommandBuffer* GetCommandBuffer() { return s_CommandBuffer; }
        static MTL::RenderCommandEncoder* GetEncoder() { return s_Encoder;}
        void* GetRenderPassDescriptorRaw() const { return _pMainPassDescriptor; }
        CA::MetalDrawable* GetCurrentDrawable() { return m_CurrentDrawable; }
        static uint32_t GetCurrentSampleCount() { return s_CurrentSampleCount; }
        MetalRendererAPI* AsMetal() override { return this; }

        using ImGuiRenderCallback = std::function<void()>;
        void CreateMSAATextures(uint32_t width, uint32_t height);
        MTL::SamplerState* GetDefaultSampler();


    private:

        uint32_t m_DrawableWidth  = 0;
        uint32_t m_DrawableHeight = 0;
        CA::MetalDrawable* m_CurrentDrawable = nullptr;

        static MTL::Device* s_Device;
        MTL::Device* _pDevice = nullptr;
        MTL::CommandQueue* _pCommandQueue = nullptr;
        MTL::CommandBuffer* _pCommandBuffer = nullptr;
        static MTL::CommandBuffer* s_CommandBuffer;
        MTL::RenderCommandEncoder* _pEncoder = nullptr;
        static MTL::RenderCommandEncoder* s_Encoder;

        MTL::RenderPassDescriptor* _pMainPassDescriptor = nullptr;

        MTL::DepthStencilState* _pDepthStateEnabled = nullptr;
        MTL::DepthStencilState* _pDepthStateDisabled = nullptr;
        MTL::DepthStencilState* _pDepthStateFramebuffer = nullptr;

        MTL::Texture* m_MSAATexture = nullptr;
        static const uint32_t k_SampleCount = 4;
        static uint32_t s_CurrentSampleCount;
        MTL::Texture* m_DepthTexture = nullptr;
        void CreateDepthTexture(uint32_t width, uint32_t height);

        ImGuiRenderCallback m_ImGuiRenderCallback;
        friend class ImGuiMetalLayer;
        glm::vec4 m_ClearColor = { 0.0f, 0.1f, 0.1f, 1.0f };

        MTL::SamplerState* m_DefaultSampler = nullptr;

    };
} // Nebula

#endif //METALRENDERERAPI_H
