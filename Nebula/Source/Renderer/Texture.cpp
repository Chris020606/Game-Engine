//
// Created by Christian Cruz on 3/9/26.
//

#include "Texture.h"

#include <cstdio>
#include "stb_image.h"

#include "RendererAPI.h"
#include "Metal/MTLDevice.hpp"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalTexture.h"

namespace Nebula {

    Ref<Texture2D> Texture2D::Create(const std::string& path, const TextureParams& params) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::Metal:
                return std::make_shared<MetalTexture2D>(path,  params);
            case RendererAPI::API::OpenGL:
                // Nothing for now
                break;
            case RendererAPI::API::None:
                printf("None API");
        }
        return nullptr;
    }

    Texture2D::~Texture2D() = default;

    Ref<Texture2D> Texture2D::WrapNative(void* nativeTexture, const TextureParams& params) {
        return MetalTexture2D::WrapNative(nativeTexture, params);
    }

} // Nebula