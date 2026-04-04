//
// Created by Christian Cruz on 1/20/26.
//

#ifndef METALBUFFERS_H
#define METALBUFFERS_H
#include "VertexBuffers.h"
#include "Metal/MTLDevice.hpp"
#include "Metal/MTLStageInputOutputDescriptor.hpp"

namespace MTL {
    class Buffer;
    class Device;
}


namespace Nebula {

    class MetalVertexBuffer : public VertexBuffer {
    public:
        MetalVertexBuffer(float* vertices, uint32_t size);
        MetalVertexBuffer(std::vector<Vertex>& vertices);
        ~MetalVertexBuffer() override;

        void Bind() const override {}
        void UnBind() const override {}

        MTL::Buffer* GetNativeBuffer() const { return m_Buffer; }
        uint32_t GetSize() const { return m_Size; }

        void SetStride(uint32_t stride) { m_Stride = stride; }
        uint32_t GetStride() const { return m_Stride; }

        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        const BufferLayout& GetLayout() override { return m_Layout; }

    private:
        MTL::Device* m_Device = nullptr;
        MTL::Buffer* m_Buffer = nullptr;
        uint32_t m_Size = 0;
        uint32_t m_Stride = 0;
        BufferLayout m_Layout;
    };

    class MetalIndexBuffer : public IndexBuffer {
    public:
        MetalIndexBuffer(uint32_t* indices, uint32_t count);
        ~MetalIndexBuffer() override;

        void Bind() const override {}
        void UnBind() const override {}

        uint32_t GetCount() const override { return m_Count; }

        // Metal-specific
        MTL::Buffer* GetNativeBuffer() const { return m_Buffer; }
        MTL::IndexType GetIndexType() const { return MTL::IndexTypeUInt32; }

    private:
        MTL::Device* m_Device = nullptr;
        MTL::Buffer* m_Buffer = nullptr;
        uint32_t m_Count = 0;
    };

} // Nebula

#endif //METALBUFFERS_H
