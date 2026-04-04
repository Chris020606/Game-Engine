//
// Created by Christian Cruz on 3/9/26.
//

#include "Texture.h"
#include "MetalTexture.h"

#include <iostream>

#include "MetalRendererAPI.h"
#include "stb_image.h"
#include "Metal/MTLSampler.hpp"
#include "Metal/MTLTexture.hpp"
#include "Metal/MTLDevice.hpp"
#include "Metal/MTLRenderCommandEncoder.hpp"

namespace Nebula {
    MetalTexture2D::MetalTexture2D(const std::string &path, const TextureParams& params) {
        auto device = MetalRendererAPI::GetDevice();
        std::cout << "MetalTexture device: " << device << std::endl;
        int width;
        int height;
        int channels;

        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (!data)
        {
            std::cerr << "Failed to load texture: " << path << std::endl;
            return;
        }

        printf("Texture loaded: %s  %dx%d channels=%d\n", path.c_str(), width, height, channels);

        m_Width = width;
        m_Height = height;

        m_Descriptor = MTL::TextureDescriptor::alloc()->init();
        m_Descriptor->setWidth( m_Width );
        m_Descriptor->setHeight( m_Height );
        m_Descriptor->setPixelFormat( MTL::PixelFormatRGBA8Unorm );
        m_Descriptor->setTextureType( MTL::TextureType2D );
        m_Descriptor->setStorageMode( MTL::StorageModeManaged );
        m_Descriptor->setUsage(MTL::TextureUsageShaderRead);

        m_Texture = device->newTexture(m_Descriptor);

        MTL::Region region = MTL::Region(0, 0, 0, m_Width, m_Height, 1);

        m_Texture->replaceRegion(
            region,
            0,
            data,
            m_Width * 4
        );

        MTL::SamplerDescriptor* samplerDesc = MTL::SamplerDescriptor::alloc()->init();

        if (params.Filter == TextureFilter::Linear)
        {
            samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
            samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
        }
        else
        {
            samplerDesc->setMinFilter(MTL::SamplerMinMagFilterNearest);
            samplerDesc->setMagFilter(MTL::SamplerMinMagFilterNearest);
        }

        MTL::SamplerAddressMode wrapMode = {};

        switch (params.Wrap)
        {
            case TextureWrap::Repeat:
                wrapMode = MTL::SamplerAddressModeRepeat;
                break;

            case TextureWrap::Clamp:
                wrapMode = MTL::SamplerAddressModeClampToEdge;
                break;

            case TextureWrap::Mirror:
                wrapMode = MTL::SamplerAddressModeMirrorRepeat;
                break;
        }

        samplerDesc->setSAddressMode(wrapMode);
        samplerDesc->setTAddressMode(wrapMode);

        m_Sampler = device->newSamplerState(samplerDesc);

        stbi_image_free(data);
        m_Descriptor->release();
        samplerDesc->release();
    }

    MetalTexture2D::~MetalTexture2D() {
        if (m_Texture)
            m_Texture->release();

        if (m_Sampler)
            m_Sampler->release();
    }

    void MetalTexture2D::Bind(uint32_t index) const {
        auto* encoder =  MetalRendererAPI::GetEncoder();
        if (!encoder) return;
        if (m_Texture) encoder->setFragmentTexture(m_Texture, index);
        if (m_Sampler) encoder->setFragmentSamplerState(m_Sampler, index);
    }

    void MetalTexture2D::Load() const {

    }

    void MetalTexture2D::GetFormat() const {

    }

    Ref<Texture2D> MetalTexture2D::WrapNative(void* mtlTexture, const TextureParams& params) {
        auto* tex = static_cast<MTL::Texture*>(mtlTexture);
        auto result = std::make_shared<MetalTexture2D>();
        result->m_Device  = MetalRendererAPI::GetDevice();
        result->m_Texture = tex->retain();
        result->m_Width   = static_cast<uint32_t>(tex->width());
        result->m_Height  = static_cast<uint32_t>(tex->height());
        result->m_Path    = "compute_output";

        MTL::SamplerDescriptor* sd = MTL::SamplerDescriptor::alloc()->init();
        sd->setMinFilter(MTL::SamplerMinMagFilterLinear);
        sd->setMagFilter(MTL::SamplerMinMagFilterLinear);
        sd->setSAddressMode(MTL::SamplerAddressModeClampToEdge);
        sd->setTAddressMode(MTL::SamplerAddressModeClampToEdge);
        result->m_Sampler = result->m_Device->newSamplerState(sd);
        sd->release();

        return result;
    }



} // Nebula