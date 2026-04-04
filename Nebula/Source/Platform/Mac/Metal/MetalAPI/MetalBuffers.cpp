//
// Created by Christian Cruz on 1/20/26.
//

#include "MetalBuffers.h"
#include <simd/simd.h>

#include "MetalRendererAPI.h"
#include "Metal/MTLBuffer.hpp"
#include "Metal/MTLDevice.hpp"

namespace Nebula {
    /// Vertex Buffer
    MetalVertexBuffer::MetalVertexBuffer(float* vertices, uint32_t size)
        : m_Size(size)
    {
        auto device = MetalRendererAPI::GetDevice();
        assert(device && "Metal device not available");

        m_Buffer = device->newBuffer(size, MTL::ResourceStorageModeManaged);
        assert(m_Buffer && "Failed to create Metal vertex buffer");

        memcpy(m_Buffer->contents(), vertices, size);

        m_Buffer->didModifyRange(NS::Range::Make(0, size));
    }

    MetalVertexBuffer::MetalVertexBuffer(std::vector<Vertex> &vertices) {
        auto device = MetalRendererAPI::GetDevice();
        assert(device && "Metal device not available");

        m_Buffer = device->newBuffer(vertices.size() * sizeof(Vertex), MTL::ResourceStorageModeManaged);
        assert(m_Buffer && "Failed to create Metal vertex buffer");

        memcpy(m_Buffer->contents(), vertices.data(), vertices.size() * sizeof(Vertex));

        m_Buffer->didModifyRange(NS::Range::Make(0, vertices.size()));
    }

    MetalVertexBuffer::~MetalVertexBuffer() {
        if (m_Buffer) {
            m_Buffer->release();
            m_Buffer = nullptr;
        }
    }

    /// Index Buffer
    MetalIndexBuffer::MetalIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        auto device = MetalRendererAPI::GetDevice();
        assert(device && "Metal device not available");

        uint32_t size = count * sizeof(uint32_t);
        m_Buffer = device->newBuffer(size, MTL::ResourceStorageModeManaged);
        assert(m_Buffer && "Failed to create Metal index buffer");

        memcpy(m_Buffer->contents(), indices, size);
        m_Buffer->didModifyRange(NS::Range::Make(0, size));
    }


    MetalIndexBuffer::~MetalIndexBuffer() {
        if (m_Buffer) {
            m_Buffer->release();
            m_Buffer = nullptr;
        }
    }
} // Nebula