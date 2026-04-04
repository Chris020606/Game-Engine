//
// Created by Christian Cruz on 12/21/25.
//

#include "RendererAPI.h"

#include <Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h>


namespace Nebula {

    RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
    RendererAPI* RendererAPI::s_Instance = nullptr;

    Ref<RendererAPI> RendererAPI::Create(const RendererAPI::API api) {
        s_API = api;
        Ref<RendererAPI> instance;
       switch (api) {
           case RendererAPI::API::Metal:
               printf("Renderer API: Metal\n");
               instance = std::make_shared<MetalRendererAPI>();
               break;
           case RendererAPI::API::OpenGL:
                printf("Renderer API: OpenGL\n");
                // instance = std::make_shared<OpenGLRendererAPI>();
               break;
            case RendererAPI::API::None: return nullptr;
        }
        s_Instance = instance.get();
        return instance;
    }
}
