//
// Created by Christian Cruz on 3/14/26.
//

#include "MetalUniformBuffer.h"
#include <Metal/MTLBuffer.hpp>

namespace Nebula {
    MetalUniformBuffer::MetalUniformBuffer(uint32_t size, MTL::Device *device) {
        m_Buffer = device->newBuffer(size, MTL::ResourceStorageModeManaged);
    }

    MetalUniformBuffer::~MetalUniformBuffer()
    {
        if (m_Buffer)
            m_Buffer->release();
    }

    void MetalUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset) {
        memcpy((uint8_t*)m_Buffer->contents() + offset, data, size);
        m_Buffer->didModifyRange(NS::Range::Make(offset, size));
    }
} // Nebula