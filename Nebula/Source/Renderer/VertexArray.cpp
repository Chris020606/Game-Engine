//
// Created by Christian Cruz on 2/2/26.
//

#include "VertexArray.h"
#include "RendererAPI.h"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalVertexArray.h"

namespace Nebula {
    VertexArray* VertexArray::Create() {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:             return nullptr;
            // case RendererAPI::API::OpenGL:        return new OpenGLVertexArray();
             case RendererAPI::API::Metal:         return new MetalVertexArray();
        }
        printf("Failed to create vertex buffer\n");
        return nullptr;
    }
} // Nebula