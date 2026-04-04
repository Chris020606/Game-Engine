#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "AppKit/AppKit.hpp"
#include "MetalKit/MetalKit.hpp"
#include "MetalRendererAPI.h"

#include "Framebuffers.h"
#include "MetalBuffers.h"
#include "MetalFramebuffer.h"
#include "MetalVertexArray.h"


namespace Nebula {

    MTL::Device* MetalRendererAPI::s_Device = nullptr;
    MTL::RenderCommandEncoder* MetalRendererAPI::s_Encoder = nullptr;
    MTL::CommandBuffer* MetalRendererAPI::s_CommandBuffer = nullptr;
    uint32_t MetalRendererAPI::s_CurrentSampleCount = 4;

    void MetalRendererAPI::Init() {
        printf("MetalRendererAPI::Init: s_Device=%p\n", s_Device);
        _pDevice = s_Device;
        if (!_pDevice) {
            printf("MetalRendererAPI::Init — device is null, was SetDevice called?\n");
            return;
        }

        // Depth test
        MTL::DepthStencilDescriptor* depthDesc = MTL::DepthStencilDescriptor::alloc()->init();
        depthDesc->setDepthCompareFunction(MTL::CompareFunctionLess);
        depthDesc->setDepthWriteEnabled(true);
        _pDepthStateEnabled = _pDevice->newDepthStencilState(depthDesc);
        depthDesc->release();

        MTL::DepthStencilDescriptor* fbDepthDesc = MTL::DepthStencilDescriptor::alloc()->init();
        fbDepthDesc->setDepthCompareFunction(MTL::CompareFunctionLess);
        fbDepthDesc->setDepthWriteEnabled(true);
        _pDepthStateFramebuffer = _pDevice->newDepthStencilState(fbDepthDesc);
        fbDepthDesc->release();

        _pCommandQueue = _pDevice->newCommandQueue();

    }

    void MetalRendererAPI::Shutdown() {
    }

    MetalRendererAPI::~MetalRendererAPI() {
        if (_pDepthStateEnabled) {
            _pDepthStateEnabled->release();
            _pDepthStateEnabled = nullptr;
        }

        if (_pDepthStateDisabled) {
            _pDepthStateDisabled->release();
            _pDepthStateDisabled = nullptr;
        }

        if (_pDepthStateFramebuffer) {
            _pDepthStateFramebuffer->release();
            _pDepthStateFramebuffer = nullptr;
        }

        if (m_MSAATexture)   { m_MSAATexture->release();   m_MSAATexture = nullptr; }
        if (m_DepthTexture)  { m_DepthTexture->release();  m_DepthTexture = nullptr; }

        _pCommandQueue->release();
        _pDevice->release();
    }

    void MetalRendererAPI::BeginFrame()
    {

    s_CurrentSampleCount = k_SampleCount;

    if (!m_CurrentDrawable) return;

    auto width  = static_cast<uint32_t>(m_CurrentDrawable->texture()->width());
    auto height = static_cast<uint32_t>(m_CurrentDrawable->texture()->height());

    if (!m_MSAATexture  ||
        m_MSAATexture->width()  != width ||
        m_MSAATexture->height() != height)
    {
        if (m_MSAATexture)  { m_MSAATexture->release();  m_MSAATexture  = nullptr; }
        if (m_DepthTexture) { m_DepthTexture->release(); m_DepthTexture = nullptr; }
        CreateMSAATextures(width, height);
        CreateDepthTexture(width, height);
    }

    _pCommandBuffer = _pCommandQueue->commandBuffer();
    s_CommandBuffer = _pCommandBuffer;

    }

    void MetalRendererAPI::BeginRenderPass() {
        if (!_pCommandBuffer || !m_CurrentDrawable) return;

        auto width  = static_cast<uint32_t>(m_CurrentDrawable->texture()->width());
        auto height = static_cast<uint32_t>(m_CurrentDrawable->texture()->height());

        MTL::RenderPassDescriptor* rpd = MTL::RenderPassDescriptor::alloc()->init();
        auto* ca = rpd->colorAttachments()->object(0);
        ca->setTexture(m_MSAATexture);
        ca->setResolveTexture(m_CurrentDrawable->texture());
        ca->setLoadAction(MTL::LoadActionClear);
        ca->setStoreAction(MTL::StoreActionMultisampleResolve);
        ca->setClearColor(MTL::ClearColor::Make( m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));

        rpd->depthAttachment()->setTexture(m_DepthTexture);
        rpd->depthAttachment()->setLoadAction(MTL::LoadActionClear);
        rpd->depthAttachment()->setStoreAction(MTL::StoreActionDontCare);
        rpd->depthAttachment()->setClearDepth(1.0);

        s_Encoder  = _pCommandBuffer->renderCommandEncoder(rpd);
        rpd->release();

        // s_Encoder->setTriangleFillMode(MTL::TriangleFillModeLines);

        SetDepthTest(true);
        SetCulling(true);
        SetViewport(0, 0, width, height);
    }

    void MetalRendererAPI::EndFrame()
    {
        if (s_Encoder) {
            s_Encoder->endEncoding();
            s_Encoder= nullptr;
            s_Encoder = nullptr;
        }

        if (m_CurrentDrawable) {
            _pCommandBuffer->presentDrawable(m_CurrentDrawable);
            m_CurrentDrawable = nullptr;
        }

        _pCommandBuffer->commit();
        _pCommandBuffer = nullptr;
    }

    void MetalRendererAPI::BeginImGuiPass() {

        if (s_Encoder) {
            s_Encoder->endEncoding();
            s_Encoder = nullptr;
        }

        MTL::RenderPassDescriptor* rpd = MTL::RenderPassDescriptor::alloc()->init();
        rpd->colorAttachments()->object(0)->setTexture(m_CurrentDrawable->texture());
        rpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionLoad);
        rpd->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);

        s_Encoder = _pCommandBuffer->renderCommandEncoder(rpd);

        rpd->release();

    }

    void MetalRendererAPI::DrawIndexed( uint32_t indexCount ) {
    }

    void MetalRendererAPI::RenderToFramebuffer( Framebuffer* framebuffer ) {
        if (!framebuffer || !_pCommandBuffer)
            return;

        // End any existing encoder before starting a new one
        if (s_Encoder) {
            s_Encoder->endEncoding();
            s_Encoder = nullptr;
        }

        auto* metalFB = static_cast<MetalFramebuffer*>(framebuffer);
        MTL::RenderPassDescriptor* pass = metalFB->GetRenderPassDescriptor();

        if (!pass)
            return;

        // Create encoder targeting the framebuffer's textures
        s_Encoder = _pCommandBuffer->renderCommandEncoder(pass);


        s_CurrentSampleCount = 1;

        if (!s_Encoder)
            return;

        auto& spec = framebuffer->GetSpecification();
        SetViewport(0, 0, spec.Width, spec.Height);
        SetCulling(true);
        SetDepthTest(true);

        if (s_Encoder && _pDepthStateFramebuffer)
            s_Encoder->setDepthStencilState(_pDepthStateFramebuffer);

    }

    void MetalRendererAPI::Draw()
    {
        printf("MetalRendererAPI::Draw\n");
        NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();


        pool->release();
    }

    void MetalRendererAPI::DrawArrays(const Ref<VertexArray>& vao, uint32_t count) {
        if (!s_Encoder || !vao)
            return;

        auto metalVAO = std::static_pointer_cast<MetalVertexArray>(vao);
        const auto& vertexBuffers = metalVAO->GetVertexBuffers();

        for (uint32_t i = 0; i < vertexBuffers.size(); ++i)
        {
            auto vb = std::static_pointer_cast<MetalVertexBuffer>(vertexBuffers[i]);
            s_Encoder->setVertexBuffer(vb->GetNativeBuffer(), 0, i);
        }

        s_Encoder->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle,
                NS::UInteger(0),
                NS::UInteger(count));

    }

    void MetalRendererAPI::DrawIndexed(const Ref<VertexArray>& vao, uint32_t count) {

        if (!s_Encoder || !vao)
            return;

        auto metalVAO = std::static_pointer_cast<MetalVertexArray>(vao);
        auto indexBuffer = std::static_pointer_cast<MetalIndexBuffer>(metalVAO->GetIndexBuffer());


        if (!indexBuffer)
            return;

        uint32_t indexCount = count ? count : indexBuffer->GetCount();

        // Draw indexed
        s_Encoder->drawIndexedPrimitives(
                MTL::PrimitiveTypeTriangle,
                indexCount,
                MTL::IndexTypeUInt32,
                indexBuffer->GetNativeBuffer(),
                0
        );
    }

    void MetalRendererAPI::DrawLines(int startPosX, int startPosY, int endPosX, int endPosY) {
        if (!s_Encoder)
            return;
        // TODO This needs attention!
        s_Encoder->drawPrimitives(MTL::PrimitiveTypeLine, 2, MTL::IndexTypeUInt32, 0);
        // All the primitives I have to cover
        // _pEncoder->drawPrimitives(MTL::PrimitiveTypeLineStrip, 2, MTL::IndexTypeUInt32, 0);
        // _pEncoder->drawPrimitives(MTL::PrimitiveTypePoint, 2, MTL::IndexTypeUInt32, 0);
        // _pEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, 2, MTL::IndexTypeUInt32, 0);
    }

    void MetalRendererAPI::DrawMesh(Ref<Mesh>& mesh) {
        if (!s_Encoder) return;

        auto vao = mesh->GetVertexArray();
        auto metalVAO = std::static_pointer_cast<MetalVertexArray>(vao);
        const auto& vertexBuffers = metalVAO->GetVertexBuffers();

        for (uint32_t i = 0; i < vertexBuffers.size(); i++) {
            auto metalVB = std::static_pointer_cast<MetalVertexBuffer>(vertexBuffers[i]);
            s_Encoder->setVertexBuffer(metalVB->GetNativeBuffer(), 0, i);
        }

        DrawIndexed(vao, vao->GetIndexBuffer()->GetCount());
    }

    void MetalRendererAPI::CreateMSAATextures(uint32_t width, uint32_t height) {
        // MSAA target
        MTL::TextureDescriptor* msaaDesc = MTL::TextureDescriptor::alloc()->init();
        msaaDesc->setTextureType(MTL::TextureType2DMultisample);
        msaaDesc->setPixelFormat(m_CurrentDrawable->texture()->pixelFormat());
        msaaDesc->setWidth(width);
        msaaDesc->setHeight(height);
        msaaDesc->setSampleCount(k_SampleCount);
        msaaDesc->setStorageMode(MTL::StorageModePrivate);
        msaaDesc->setUsage(MTL::TextureUsageRenderTarget);
        m_MSAATexture = _pDevice->newTexture(msaaDesc);
        msaaDesc->release();
    }

    MTL::SamplerState * MetalRendererAPI::GetDefaultSampler() {
        if (!m_DefaultSampler) {
            MTL::SamplerDescriptor* desc = MTL::SamplerDescriptor::alloc()->init();
            desc->setMinFilter(MTL::SamplerMinMagFilterLinear);
            desc->setMagFilter(MTL::SamplerMinMagFilterLinear);
            desc->setSAddressMode(MTL::SamplerAddressModeRepeat);
            desc->setTAddressMode(MTL::SamplerAddressModeRepeat);
            m_DefaultSampler = _pDevice->newSamplerState(desc);
            desc->release();
        }
        return m_DefaultSampler;
    }

    void MetalRendererAPI::EndCurrentEncoder() {
        if (s_Encoder) {
            s_Encoder->endEncoding();
            s_Encoder = nullptr;
        }
    }

    void MetalRendererAPI::CreateDepthTexture(uint32_t width, uint32_t height) {
        if (m_DepthTexture) {
            m_DepthTexture->release();
            m_DepthTexture = nullptr;
        }

        MTL::TextureDescriptor* depthDesc = MTL::TextureDescriptor::alloc()->init();
        depthDesc->setTextureType(MTL::TextureType2DMultisample);
        depthDesc->setPixelFormat(MTL::PixelFormatDepth32Float);
        depthDesc->setWidth(width);
        depthDesc->setHeight(height);
        depthDesc->setSampleCount(k_SampleCount);
        depthDesc->setStorageMode(MTL::StorageModePrivate);
        depthDesc->setUsage(MTL::TextureUsageRenderTarget);
        m_DepthTexture = _pDevice->newTexture(depthDesc);
        depthDesc->release();
    }

    void MetalRendererAPI::SetBlending(bool enabled) {
        // Needs implementation
    }

    void MetalRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        if (!s_Encoder)
            return;

        MTL::Viewport vp{};
        vp.originX = x;
        vp.originY = y;
        vp.width   = width;
        vp.height  = height;
        vp.znear   = 0.0;
        vp.zfar    = 1.0;

        s_Encoder->setViewport(vp);
    }

    void MetalRendererAPI::SetDepthTest(bool enabled) {
        if (!s_Encoder)
            return;

        if (enabled)
            s_Encoder->setDepthStencilState(_pDepthStateEnabled);
        else
            s_Encoder->setDepthStencilState(_pDepthStateDisabled);
    }

    void MetalRendererAPI::SetCulling(bool enabled) {
        if (!s_Encoder)
            return;

        if (enabled) {
           s_Encoder->setCullMode(MTL::CullModeBack);
        } else {
            s_Encoder->setCullMode(MTL::CullModeNone);
        }
        s_Encoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    }

    void MetalRendererAPI::SetClearColor(const glm::vec4& color) {
        m_ClearColor = color;
    }

    void MetalRendererAPI::Clear() {
        // auto* pass = m_CurrentDrawable->texture()->

        // pass->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    }

    void MetalRendererAPI::SetDevice(MTL::Device *device) {
        s_Device = device->retain();
    }

} // Nebula
