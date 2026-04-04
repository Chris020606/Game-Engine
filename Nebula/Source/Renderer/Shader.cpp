#include "Shader.h"
#include "RendererAPI.h"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalShader.h"
#include <iostream>

#ifdef NEBULA_PLATFORM_MACOS
    #include "Platform/Metal/MetalShader.h"
#endif

namespace Nebula {

    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::Metal:
                return std::make_shared<MetalShader>(filepath);
            case RendererAPI::API::OpenGL:
                // return std::make_shared<OpenGLShader>(filepath);
                break;
            case RendererAPI::API::None:
                std::printf("None API/n");
        }
        
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::Metal:
                return std::make_shared<MetalShader>(name, vertexSrc, fragmentSrc);
            case RendererAPI::API::OpenGL:
                // return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
                break;
            case RendererAPI::API::None:
                printf("None API");
        }
        
        return nullptr;
    }

}