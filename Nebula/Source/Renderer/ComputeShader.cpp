//
// Created by Christian Cruz on 1/6/26.
//

#include "ComputeShader.h"

#include "RendererAPI.h"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalComputeShader.h"

namespace MTL {
    class Device;
}

namespace Nebula {
    void ComputeShader::Bind() {
    }

    void ComputeShader::UnBind() {
    }

    Ref<ComputeShader> ComputeShader::Create(const std::string &filepath) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::Metal:
                return std::make_shared<MetalComputeShader>(filepath);
            case RendererAPI::API::OpenGL:
                // return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
                break;
            case RendererAPI::API::None:
                printf("None API");
        }
    }

    Ref<ComputeShader> ComputeShader::Create(const std::string &filepath, const std::string &functionName) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::Metal:
                return std::make_shared<MetalComputeShader>(filepath, functionName);
            case RendererAPI::API::OpenGL:
                // return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
                break;
            case RendererAPI::API::None:
                printf("None API");
        }
    }
} // Nebula