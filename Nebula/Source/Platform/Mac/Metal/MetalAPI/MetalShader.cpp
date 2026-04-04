//
// Created by Christian Cruz on 2/13/26.
//

#include "MetalShader.h"

#include <sstream>
#include "MetalVertexArray.h"
#include "MetalBuffers.h"

#include <Metal/Metal.hpp>

namespace Nebula {

    MetalShader::MetalShader(const std::filesystem::path& filepath)
        : m_PipelineState(nullptr)
        , m_Library(nullptr)
        , m_Name(filepath.filename().string())
        , m_VertexFunctionName("vertexMain")
        , m_FragmentFunctionName("fragmentMain")
    {
        BuildFromFile(filepath.c_str());
    }

    MetalShader::MetalShader(const std::filesystem::path& filepath,
                             const std::string& vertex,
                             const std::string& fragment)
        : m_PipelineState(nullptr)
        , m_Library(nullptr)
        , m_Name(filepath.filename().string())
        , m_VertexFunctionName(vertex)
        , m_FragmentFunctionName(fragment)
    {
        BuildFromFile(filepath.c_str());
    }

    MetalShader::~MetalShader() {
        if (m_PipelineState) { m_PipelineState->release(); m_PipelineState = nullptr; }
        if (m_Library)       { m_Library->release();       m_Library = nullptr;       }
        if (m_UniformBuffer) { m_UniformBuffer->release(); m_UniformBuffer = nullptr; }
    }

    void MetalShader::BuildFromFile(const char* filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "ERROR: Could not open shader file: " << filepath << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        BuildFromSource(buffer.str().c_str());
    }

    void MetalShader::BuildFromSource(const char* source) {
        m_Device = MetalRendererAPI::GetDevice();
        NS::Error* error = nullptr;
        m_Library = m_Device->newLibrary(
            NS::String::string(source, NS::StringEncoding::UTF8StringEncoding),
            nullptr, &error);

        if (!m_Library) {
            std::cerr << "ERROR: Failed to compile shader: " << m_Name << std::endl;
            if (error)
                std::cerr << error->localizedDescription()->utf8String() << std::endl;
            return;
        }
    }

   void MetalShader::CreatePipelineState(MTL::VertexDescriptor* vertexDescriptor) {
        if (!m_Library) { printf("no library\n"); return; }
        if (!m_Device)  { printf("no device\n");  return; }

        // Cache descriptor for later rebuilds
        m_LastDescriptor = vertexDescriptor;

        MTL::Function* vertexFunc = m_Library->newFunction(
            NS::String::string(m_VertexFunctionName.c_str(), NS::StringEncoding::UTF8StringEncoding));
        MTL::Function* fragmentFunc = m_Library->newFunction(
            NS::String::string(m_FragmentFunctionName.c_str(), NS::StringEncoding::UTF8StringEncoding));

        if (!vertexFunc || !fragmentFunc) {
            if (vertexFunc)   vertexFunc->release();
            if (fragmentFunc) fragmentFunc->release();
            return;
        }

        // Helper lambda to build a pipeline with a given sample count
        auto buildPipeline = [&](uint32_t sampleCount, MTL::PixelFormat colorFormat) -> MTL::RenderPipelineState* {
            MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
            desc->setVertexFunction(vertexFunc);
            desc->setFragmentFunction(fragmentFunc);
            desc->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
            desc->setSampleCount(sampleCount);

            if (vertexDescriptor)
                desc->setVertexDescriptor(vertexDescriptor);

            auto* ca = desc->colorAttachments()->object(0);
            ca->setPixelFormat(colorFormat);
            ca->setBlendingEnabled(true);
            ca->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
            ca->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
            ca->setRgbBlendOperation(MTL::BlendOperationAdd);
            ca->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
            ca->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
            ca->setAlphaBlendOperation(MTL::BlendOperationAdd);

            NS::Error* error = nullptr;
            MTL::RenderPipelineState* pso = m_Device->newRenderPipelineState(desc, &error);
            desc->release();

            if (!pso && error)
                std::cerr << "Pipeline error: " << error->localizedDescription()->utf8String() << std::endl;

            return pso;
        };

        // Release old pipelines
        if (m_PipelineState)   { m_PipelineState->release();   m_PipelineState   = nullptr; }
        if (m_PipelineStateFB) { m_PipelineStateFB->release(); m_PipelineStateFB = nullptr; }

        // Build MSAA pipeline for main pass
        m_PipelineState   = buildPipeline(4, MTL::PixelFormatBGRA8Unorm_sRGB);

        // Build non-MSAA pipeline for framebuffer rendering
        m_PipelineStateFB = buildPipeline(1, MTL::PixelFormatBGRA8Unorm_sRGB);

        vertexFunc->release();
        fragmentFunc->release();

        if (m_PipelineState && m_PipelineStateFB)
            std::cout << "Successfully created pipeline state for " << m_Name << std::endl;
    }

    void MetalShader::SetVertexDescriptor(MTL::VertexDescriptor* descriptor) {
        CreatePipelineState(descriptor);
    }

    void MetalShader::RebuildPipeline(MTL::VertexDescriptor* descriptor) {
        CreatePipelineState(descriptor);
    }

    const std::string &MetalShader::GetName() {
        return m_Name;
    }

    void MetalShader::SetInt(const std::string& name, int value) {

    }

    void MetalShader::SetFloat(const std::string& name, float value) {

    }

    void MetalShader::SetFloat2(const std::string& name, const glm::vec2& value) {

    }

    void MetalShader::SetFloat3(const std::string& name, const glm::vec3& value) {

    }

    void MetalShader::SetFloat4(const std::string& name, const glm::vec4& value) {

    }

    void MetalShader::SetMat4(const std::string& name, const glm::mat4& value) {
        if      (name == "viewProjection") m_Uniforms.viewProjection = value;
        else if (name == "model")          m_Uniforms.model          = value;

        if (!m_UniformBuffer)
            m_UniformBuffer = MetalRendererAPI::GetDevice()->newBuffer(
                sizeof(UniformBlock), MTL::ResourceStorageModeManaged);

        memcpy(m_UniformBuffer->contents(), &m_Uniforms, sizeof(UniformBlock));
        m_UniformBuffer->didModifyRange(NS::Range::Make(0, sizeof(UniformBlock)));
    }

    void MetalShader::Bind() const {
        Bind(nullptr);
    }

    void MetalShader::Bind(const Ref<VertexArray>& va) const {
        auto* encoder = MetalRendererAPI::GetEncoder();
        if (!encoder) return;

        // Build pipelines if not yet created
        if (!m_PipelineState || !m_PipelineStateFB) {
            auto* self = const_cast<MetalShader*>(this);
            if (va) {
                auto metalVA = std::static_pointer_cast<MetalVertexArray>(va);
                self->CreatePipelineState(metalVA->GetVertexDescriptor());
            } else {
                self->CreatePipelineState(nullptr);
            }
        }

        // Select pipeline based on current render target sample count
        uint32_t sampleCount = MetalRendererAPI::GetCurrentSampleCount();
        MTL::RenderPipelineState* pso = (sampleCount == 1) ? m_PipelineStateFB : m_PipelineState;

        if (!pso) return;

        // Init uniform buffer if needed
        if (!m_UniformBuffer) {
            auto* self = const_cast<MetalShader*>(this);
            self->m_UniformBuffer = MetalRendererAPI::GetDevice()->newBuffer(
                sizeof(UniformBlock), MTL::ResourceStorageModeManaged);
            self->m_Uniforms.viewProjection = glm::mat4(1.f);
            self->m_Uniforms.model          = glm::mat4(1.f);
            memcpy(self->m_UniformBuffer->contents(), &self->m_Uniforms, sizeof(UniformBlock));
            self->m_UniformBuffer->didModifyRange(NS::Range::Make(0, sizeof(UniformBlock)));
        }

        encoder->setRenderPipelineState(pso);
        encoder->setVertexBuffer(m_UniformBuffer, 0, 1);
    }
    void MetalShader::Unbind() const {}
} // Nebula