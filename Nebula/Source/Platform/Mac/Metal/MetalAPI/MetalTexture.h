//
// Created by Christian Cruz on 3/9/26.
//

#ifndef METALTEXTURE_H
#define METALTEXTURE_H

#include <string>

#include "Texture.h"
#include "Metal/MTLDevice.hpp"

namespace Nebula {

class MetalTexture2D : public Texture2D {
    public:
    MetalTexture2D() = default;
    MetalTexture2D(const std::string& path, const TextureParams& params);
    ~MetalTexture2D() override;

    uint32_t GetWidth() const override { return m_Width; }
    uint32_t GetHeight() const override { return m_Height; }

    void Bind(uint32_t index = 0) const override;

    void Load() const override;
    void GetFormat() const override;

    const std::string& GetPath() const override { return m_Path; }

    static Ref<Texture2D> WrapNative(void* mtlTexture, const TextureParams& params);

    private:

    std::string m_Path;
    MTL::Device* m_Device;
    MTL::SamplerState* m_Sampler = nullptr;
    MTL::Texture* m_Texture;
    MTL::TextureDescriptor* m_Descriptor;
    uint32_t m_RendererID;

    uint32_t m_Width, m_Height;

};

} // Nebula

#endif //METALTEXTURE_H
