//
// Created by Christian Cruz on 2/13/26.
//

#ifndef METALSHADER_H
#define METALSHADER_H

#include <fstream>
#include "Shader.h"
#include "MetalRendererAPI.h"

namespace MTL {
    class VertexDescriptor;
}

namespace Nebula {

class MetalShader : public Shader {
    public:
    MetalShader(const std::filesystem::path& filepath);
    MetalShader(const std::filesystem::path& filepath, const std::string& vertex, const std::string& fragment);

    ~MetalShader() override ;

    void Bind() const override;
    void Bind(const Ref<VertexArray>& va) const override;
    void Unbind() const override;

    void SetInt(const std::string& name, int value) override;
    void SetFloat(const std::string& name, float value) override;
    void SetFloat2(const std::string& name, const glm::vec2& value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

    const std::string& GetName() override;
    [[nodiscard]] MTL::RenderPipelineState* GetPipelineState() const { return m_PipelineState; }
    [[nodiscard]] MTL::Library* GetLibrary() const { return m_Library; }
    void SetVertexDescriptor(MTL::VertexDescriptor* descriptor);
    void RebuildPipeline(MTL::VertexDescriptor* descriptor);
    void SetSampleCount(uint32_t count) { m_SampleCount = count; }
    uint32_t GetSampleCount() const { return m_SampleCount; }

    private:

    MTL::RenderPipelineState* m_PipelineState     = nullptr;
    MTL::RenderPipelineState* m_PipelineStateFB   = nullptr;
    MTL::VertexDescriptor* m_LastDescriptor = nullptr;
    MTL::Library* m_Library;
    std::string m_Name;
    std::string m_VertexFunctionName;
    std::string m_FragmentFunctionName;
    MTL::Device* m_Device;
    uint32_t m_SampleCount = 4;

    void BuildFromSource(const char* source);
    void BuildFromFile(const char* filepath);
    void CreatePipelineState(MTL::VertexDescriptor* vertexDescriptor = nullptr);

    struct UniformBlock {
        glm::mat4 viewProjection;
        glm::mat4 model;
    } m_Uniforms;

    MTL::Buffer* m_UniformBuffer = nullptr;

};

} // Nebula

#endif //METALSHADER_H
