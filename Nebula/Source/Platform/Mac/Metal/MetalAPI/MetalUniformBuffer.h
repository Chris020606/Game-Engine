//
// Created by Christian Cruz on 3/14/26.
//

#ifndef METALUNIFORMBUFFER_H
#define METALUNIFORMBUFFER_H
#include "UniformBuffers.h"
#include "Metal/MTLDevice.hpp"

namespace Nebula {

class MetalUniformBuffer : public UniformBuffers {
public:
    MetalUniformBuffer(uint32_t size, MTL::Device* device);
    ~MetalUniformBuffer() override;

    void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

    [[nodiscard]] MTL::Buffer* GetBuffer() const { return m_Buffer; }

private:
    MTL::Buffer* m_Buffer;

};

} // Nebula

#endif //METALUNIFORMBUFFER_H
