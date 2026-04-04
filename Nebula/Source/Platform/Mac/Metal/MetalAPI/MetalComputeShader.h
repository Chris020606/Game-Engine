//
// Created by Christian Cruz on 3/11/26.
//

#ifndef METALCOMPUTESHADER_H
#define METALCOMPUTESHADER_H

#include <filesystem>

#include "ComputeShader.h"
#include "Metal/MTLDevice.hpp"

namespace Nebula {

class MetalComputeShader : public ComputeShader {
public:

    MetalComputeShader(const std::filesystem::path& filepath);
    MetalComputeShader(const std::filesystem::path& filepath, const std::string& functionName);
    ~MetalComputeShader() override;
    void Bind() override;
    void UnBind() override;
    void Dispatch(uint32_t width, uint32_t height) override;
    void SetFloat(const std::string& name, float value) override;
    void Resize(uint32_t width, uint32_t height) override;
    void* GetOutputTexture() const override;

    private:
    MTL::Device* m_device;
    MTL::ComputePipelineState* m_computePipelineState;
    MTL::Library* m_library;
    std::string m_Name;
    std::string m_FunctionName;
    NS::Error* error = nullptr;
    MTL::Texture*              m_OutputTexture         = nullptr;
    MTL::Buffer*               m_TimeBuffer            = nullptr;
    uint32_t                   m_Width  = 512;
    uint32_t                   m_Height = 512;


    void BuildFromSource(const char* source);
    void BuildFromFile(const char* filepath);
    void CreateComputePipelineState();
    void CreateOutputTexture(uint32_t width, uint32_t height);
};

} // Nebula

#endif //METALCOMPUTESHADER_H
