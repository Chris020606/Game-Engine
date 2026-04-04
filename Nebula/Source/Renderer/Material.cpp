#include "Material.h"

#include <ranges>

#include "Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h"

namespace Nebula {

    Material::Material(const Ref<Shader>& shader,
                   std::unordered_map<std::string, Ref<Texture>> textureMap)
    : m_Shader(shader), textures(std::move(textureMap))
    {
    }

    void Material::SetTexture(const std::string &name, const Ref<Texture> &texture) {
        textures[name] = texture;

        if (m_TextureSlots.find(name) == m_TextureSlots.end()) {
            m_TextureSlots[name] = static_cast<uint32_t>(m_TextureSlots.size());
        }
    }

    Ref<Texture> Material::GetTexture(const std::string &name) {
        for (auto &[fst, snd] : textures) {
            if (fst == name) {
                return snd;
            }
        }
        return nullptr;
    }

    void Material::SetFloat(const std::string &name, float value) const {
        m_Shader->SetFloat(name, value);
    }

    void Material::SetFloat2(const std::string &name, glm::vec2 value) const {
        m_Shader->SetFloat2(name, value);
    }

    void Material::SetFloat3(const std::string &name, glm::vec3 value) const {
        m_Shader->SetFloat3(name, value);
    }

    void Material::SetFloat4(const std::string &name, glm::vec4 value) const {
        m_Shader->SetFloat4(name, value);
    }

    void Material::SetInt(const std::string &name, int value) const {
        m_Shader->SetInt(name, value);
    }

    void Material::SetMat4(const std::string &name, glm::mat4 value) const {
        m_Shader->SetMat4(name, value);
    }

    void Material::Bind() const {
        uint32_t slot = 0;

        m_Shader->Bind();

        for (const auto& [name, texture] : textures) {
            texture->Bind(slot);

            m_Shader->SetInt(name, static_cast<int>(slot)); // TODO Why this?

            slot++;
        }
    }

    void Material::Bind(const Ref<VertexArray> &va) const {
        m_Shader->Bind(va);

        for (const auto& [name, texture] : textures) {
            if (!texture) continue;
            auto it = m_TextureSlots.find(name);
            uint32_t slot = (it != m_TextureSlots.end()) ? it->second : 0;
            texture->Bind(slot);
        }
    }
} // Nebula