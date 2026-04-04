//
// Created by Christian Cruz on 1/14/26.
//

#ifndef METALFRAMEBUFFER_H
#define METALFRAMEBUFFER_H

#include "Metal/Metal.hpp"
#include "Source/Renderer/Framebuffers.h"

#ifdef __OBJC__
#import <Metal/Metal.h>
#else
// Forward declarations for C++
typedef void MTLTextureDescriptor;
typedef void MTLTexture;
typedef void MTLRenderPassDescriptor;
#endif

namespace MTL {
    class Device;
}

#include "MetalRendererAPI.h"

namespace Nebula {

    class MetalFramebuffer : public Framebuffer {
    public:
        explicit MetalFramebuffer(const FramebufferSpecification& spec);
        ~MetalFramebuffer() override;

        // Framebuffer interface
        void Bind()   override;
        void Unbind() override;
        void Resize(uint32_t width, uint32_t height) override;
        void* GetColorAttachmentRendererID() const override;
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

        // Metal-specific accessors
        MTL::Texture*              GetColorTexture()          const { return m_ColorTexture; }
        MTL::Texture*              GetDepthTexture()          const { return m_DepthTexture; }
        MTL::RenderPassDescriptor* GetRenderPassDescriptor()  const { return m_RenderPassDescriptor; }

        // Render into this framebuffer using the current command buffer
        void BeginRenderPass(MTL::CommandBuffer* commandBuffer, MTL::RenderCommandEncoder*& outEncoder) const;

        void SetClearColor(float r, float g, float b, float a) override;;

    private:
        void Invalidate();
        void Release();

        FramebufferSpecification   m_Specification;
        MTL::Device*               m_Device               = nullptr;
        MTL::Texture*              m_ColorTexture         = nullptr;
        MTL::Texture*              m_DepthTexture         = nullptr;
        MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
    };

} // Nebula

#endif // METALFRAMEBUFFER_H