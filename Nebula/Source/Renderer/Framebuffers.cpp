//
// Created by Christian Cruz on 1/14/26.
//

#include "Framebuffers.h"
#include "RendererAPI.h"

#include "Source/Platform/Mac/Metal/MetalAPI/MetalFramebuffer.h"

namespace Nebula {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::Metal:
                return std::make_shared<MetalFramebuffer>(
                    spec
                );
            default:
                return nullptr;
        }
    }
} // Nebula