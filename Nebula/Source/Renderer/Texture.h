//
// Created by Christian Cruz on 3/9/26.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <cstdint>
#include <memory>
#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Core.h"

namespace Nebula {

    enum class TextureFilter
    {
        Linear = 0,
        Nearest
    };

    enum class TextureWrap
    {
        Repeat = 0,
        Clamp,
        Mirror
    };

    struct TextureParams
    {
        TextureFilter Filter = TextureFilter::Linear;
        TextureWrap Wrap = TextureWrap::Repeat;
    };

    struct TextureSpecification {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t samples;
    };

    class Texture {
        public:
        virtual ~Texture() = default;
        virtual void Load() const = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void GetFormat() const = 0;

        virtual void Bind(uint32_t index) const = 0;

    };

    class Texture2D : public Texture {
        public:
        ~Texture2D() override;
        static Ref<Texture2D> Create(const std::string& path, const TextureParams& params = TextureParams());

        virtual const std::string& GetPath() const = 0;

        static Ref<Texture2D> WrapNative(void* nativeTexture, const TextureParams& params);
    };

} // Nebula

#endif //TEXTURE_H
