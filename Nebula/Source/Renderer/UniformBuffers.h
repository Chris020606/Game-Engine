//
// Created by Christian Cruz on 1/18/26.
//

#ifndef UNIFORMBUFFERS_H
#define UNIFORMBUFFERS_H
#include <cstdint>

namespace Nebula {

class UniformBuffers {
    public:
    virtual ~UniformBuffers() = default;

    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

};

} // Nebula

#endif //UNIFORMBUFFERS_H
