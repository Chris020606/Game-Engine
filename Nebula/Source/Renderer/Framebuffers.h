//
// Created by Christian Cruz on 1/14/26.
//

#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H
#include <cstdint>
#include <memory>

#include "Core.h"

namespace Nebula {

    struct FramebufferSpecification
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        bool HasDepth = true;
        bool Samples = false; // optional for later
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        // For ImGui::Image
        [[nodiscard]] virtual void* GetColorAttachmentRendererID() const = 0;

        [[nodiscard]] virtual const FramebufferSpecification& GetSpecification() const = 0;

        // Framebuffers.h
        virtual void SetClearColor(float r, float g, float b, float a) = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };

} // Nebula

#endif //FRAMEBUFFERS_H
