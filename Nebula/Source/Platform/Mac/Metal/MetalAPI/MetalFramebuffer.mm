#import "MetalFramebuffer.h"
#import <Metal/Metal.h>
#import <Metal/Metal.hpp>


namespace Nebula {

    MetalFramebuffer::MetalFramebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        m_Device = MetalRendererAPI::GetDevice();
        if (m_Device) m_Device->retain();
        Invalidate();
    }

    MetalFramebuffer::~MetalFramebuffer() {
        Release();
        if (m_Device) {
            m_Device->release();
            m_Device = nullptr;
        }
    }

    void MetalFramebuffer::Invalidate() {
        if (!m_Device) {
            printf("[MetalFramebuffer] ERROR: No device!\n");
            return;
        }

        Release();

        // ---- Color texture ----
        MTL::TextureDescriptor* colorDesc = MTL::TextureDescriptor::texture2DDescriptor(
            MTL::PixelFormatBGRA8Unorm_sRGB,
            m_Specification.Width,
            m_Specification.Height,
            false
        );
        colorDesc->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
        colorDesc->setStorageMode(MTL::StorageModePrivate);
        m_ColorTexture = m_Device->newTexture(colorDesc);
        colorDesc->release();

        // ---- Depth texture ----
        if (m_Specification.HasDepth) {
            MTL::TextureDescriptor* depthDesc = MTL::TextureDescriptor::texture2DDescriptor(
                MTL::PixelFormatDepth32Float,
                m_Specification.Width,
                m_Specification.Height,
                false
            );
            depthDesc->setUsage(MTL::TextureUsageRenderTarget);
            depthDesc->setStorageMode(MTL::StorageModeMemoryless);
            m_DepthTexture = m_Device->newTexture(depthDesc);
            depthDesc->release();
        }

        // ---- Render pass descriptor ----
        m_RenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();

        auto* colorAttach = m_RenderPassDescriptor->colorAttachments()->object(0);
        colorAttach->setTexture(m_ColorTexture);
        colorAttach->setLoadAction(MTL::LoadActionClear);
        colorAttach->setStoreAction(MTL::StoreActionStore);
        colorAttach->setClearColor(MTL::ClearColor::Make(0.1, 0.1, 0.1, 1.0));

        if (m_DepthTexture) {
            auto* depthAttach = m_RenderPassDescriptor->depthAttachment();
            depthAttach->setTexture(m_DepthTexture);
            depthAttach->setLoadAction(MTL::LoadActionClear);
            depthAttach->setStoreAction(MTL::StoreActionDontCare);
            depthAttach->setClearDepth(1.0);
        }
    }

    void MetalFramebuffer::Release() {
        if (m_ColorTexture)         { m_ColorTexture->release();         m_ColorTexture         = nullptr; }
        if (m_DepthTexture)         { m_DepthTexture->release();         m_DepthTexture         = nullptr; }
        if (m_RenderPassDescriptor) { m_RenderPassDescriptor->release(); m_RenderPassDescriptor = nullptr; }
    }

    void MetalFramebuffer::Bind()   {}
    void MetalFramebuffer::Unbind() {}

    void MetalFramebuffer::Resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0) return;
        m_Specification.Width  = width;
        m_Specification.Height = height;
        Invalidate();
    }

    void* MetalFramebuffer::GetColorAttachmentRendererID() const {
        // Bridge metal-cpp → void* for ImGui::Image()
        // ImGui::Image((ImTextureID)framebuffer->GetColorAttachmentRendererID(), size);
        return (__bridge void*)((__bridge id<MTLTexture>) m_ColorTexture);
    }

    void MetalFramebuffer::BeginRenderPass(MTL::CommandBuffer* commandBuffer,
                                            MTL::RenderCommandEncoder*& outEncoder) const {
        if (!commandBuffer || !m_RenderPassDescriptor) {
            outEncoder = nullptr;
            return;
        }
        outEncoder = commandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    }

    void MetalFramebuffer::SetClearColor(float r, float g, float b, float a) {
    if (m_RenderPassDescriptor)
        m_RenderPassDescriptor->colorAttachments()->object(0)
            ->setClearColor(MTL::ClearColor::Make(r, g, b, a));
    }

}// Nebula
