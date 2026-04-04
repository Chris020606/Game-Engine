//
// Created by Christian Cruz on 3/11/26.
//

#include "MetalComputeShader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "MetalRendererAPI.h"
#include "Metal/MTLComputePipeline.hpp"
#include "Metal/MTLLibrary.hpp"
#include "Metal/MTLBuffer.hpp"
#include "Metal/MTLCommandBuffer.hpp"
#include "Metal/MTLComputeCommandEncoder.hpp"


namespace Nebula {
    MetalComputeShader::MetalComputeShader(const std::filesystem::path& filepath)
            : m_Name(filepath.filename().c_str()),
              m_FunctionName("kernelMain"),
              m_computePipelineState(nullptr)
    {
        m_device = MetalRendererAPI::GetDevice();
        BuildFromFile(filepath.c_str());
        CreateComputePipelineState();
        CreateComputePipelineState();
        CreateOutputTexture(m_Width, m_Height);
        m_TimeBuffer = m_device->newBuffer(sizeof(float), MTL::ResourceStorageModeShared);
    }

    MetalComputeShader::MetalComputeShader(const std::filesystem::path& filepath, const std::string &functionName)
            : m_Name(filepath.filename().c_str()),
              m_FunctionName(functionName),
              m_computePipelineState(nullptr)
    {
        BuildFromFile(filepath.c_str());
        CreateComputePipelineState();
    }

    MetalComputeShader::~MetalComputeShader() {
        if (m_computePipelineState) {
            m_computePipelineState->release();
            m_computePipelineState = nullptr;
        }
    }

    void MetalComputeShader::Bind() {
    }

    void MetalComputeShader::UnBind() {
    }

    void MetalComputeShader::BuildFromSource(const char* source) {
        m_device = MetalRendererAPI::GetDevice();
        std::cout << "MetalComputeShader device: " << m_device << std::endl;
        NS::String* sourceCode = NS::String::string(source, NS::StringEncoding::UTF8StringEncoding);
        MTL::CompileOptions* options = nullptr;
        m_library = m_device->newLibrary(sourceCode, options, &error);

        if ( !m_library)
        {
            printf( "%s", error->localizedDescription()->utf8String() );
            assert(false);
        }
    }

    void MetalComputeShader::BuildFromFile(const char* filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "ERROR: Could not open shader file: " << filepath << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source = buffer.str();
        file.close();

        BuildFromSource(source.c_str());
    }

    void MetalComputeShader::CreateComputePipelineState() {
        if (!m_library) {
            printf("Error: Could not create compute pipeline state\n");
            return;
        }

        NS::String* functionName = NS::String::string(m_FunctionName.c_str(), NS::StringEncoding::UTF8StringEncoding);
        MTL::Function* functionKernel = m_library->newFunction( functionName );

        m_computePipelineState = m_device->newComputePipelineState(functionKernel , &error);
        if ( !m_computePipelineState )
        {
            printf( "%s", error->localizedDescription()->utf8String() );
            assert(false);
        }
    }

    void MetalComputeShader::CreateOutputTexture(uint32_t width, uint32_t height) {
        if (m_OutputTexture) { m_OutputTexture->release(); m_OutputTexture = nullptr; }

        MTL::TextureDescriptor* desc = MTL::TextureDescriptor::texture2DDescriptor(
            MTL::PixelFormatRGBA16Float, width, height, false);
        desc->setUsage(MTL::TextureUsageShaderWrite | MTL::TextureUsageShaderRead);
        desc->setStorageMode(MTL::StorageModePrivate);

        m_OutputTexture = m_device->newTexture(desc);
        desc->release();

        m_Width  = width;
        m_Height = height;
    }

    void MetalComputeShader::SetFloat(const std::string& name, float value) {
        if (name == "time" && m_TimeBuffer)
            memcpy(m_TimeBuffer->contents(), &value, sizeof(float));
    }

    void MetalComputeShader::Dispatch(uint32_t width, uint32_t height) {
        if (!m_computePipelineState || !m_OutputTexture) return;

        auto* cmdBuffer = MetalRendererAPI::GetCommandBuffer();
        if (!cmdBuffer) return;

        MetalRendererAPI::EndCurrentEncoder();

        auto* encoder = cmdBuffer->computeCommandEncoder();
        if (!encoder) return;

        encoder->setComputePipelineState(m_computePipelineState);
        encoder->setTexture(m_OutputTexture, 0);
        encoder->setBuffer(m_TimeBuffer, 0, 0);

        MTL::Size threadGroupSize = MTL::Size(16, 16, 1);
        MTL::Size threadGroups    = MTL::Size((width + 15) / 16, (height + 15) / 16, 1);

        encoder->dispatchThreadgroups(threadGroups, threadGroupSize);
        encoder->endEncoding();
    }

    void* MetalComputeShader::GetOutputTexture() const {
        return static_cast<void*>(m_OutputTexture);
    }

    void MetalComputeShader::Resize(uint32_t width, uint32_t height) {
        if (width == m_Width && height == m_Height) return;
        CreateOutputTexture(width, height);
    }

} // Nebula