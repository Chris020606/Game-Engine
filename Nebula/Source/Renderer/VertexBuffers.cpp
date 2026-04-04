//
// Created by Christian Cruz on 1/18/26.
//

#include "VertexBuffers.h"

#include "Render.h"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalBuffers.h"
#include "Source/Platform/OpenGlAPI/OpenGLBuffer.h"

namespace Nebula {
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:          return nullptr;
            case RendererAPI::API::OpenGL:        return new OpenGLVertexBuffer(vertices, size);
            case RendererAPI::API::Metal:         return new MetalVertexBuffer(vertices, size);
        }
        printf("Failed to create vertex buffer\n");
        return nullptr;
    }

    VertexBuffer* VertexBuffer::Create(std::vector<Vertex>& vertices) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:          return nullptr;
            // case RendererAPI::API::OpenGL:        return new OpenGLVertexBuffer(vertices);
            case RendererAPI::API::Metal:         return new MetalVertexBuffer(vertices);

        }
        printf("Failed to create vertex buffer\n");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:          return nullptr;
            case RendererAPI::API::OpenGL:        return new OpenGLIndexBuffer(indices, count);
            case RendererAPI::API::Metal:         return new MetalIndexBuffer(indices, count);
        }
        printf("Failed to create index buffer\n");
        return nullptr;
    }
} // Nebula