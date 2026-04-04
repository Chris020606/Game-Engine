//
// Created by Christian Cruz on 2/5/26.
//

#ifndef METALVERTEXARRAY_H
#define METALVERTEXARRAY_H

#include <memory>

#include "Core.h"
#include "VertexArray.h"
#include "VertexBuffers.h"
#include "Metal/MTLVertexDescriptor.hpp"

namespace Nebula {
    static MTL::VertexFormat ShaderDataTypeToMetalFormat(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:    return MTL::VertexFormatFloat;
            case ShaderDataType::Float2:   return MTL::VertexFormatFloat2;
            case ShaderDataType::Float3:   return MTL::VertexFormatFloat3;
            case ShaderDataType::Float4:   return MTL::VertexFormatFloat4;
            case ShaderDataType::Mat3:     return MTL::VertexFormatFloat;
            case ShaderDataType::Mat4:     return MTL::VertexFormatFloat;
            case ShaderDataType::Int:      return MTL::VertexFormatInt;
            case ShaderDataType::Int2:     return MTL::VertexFormatInt2;
            case ShaderDataType::Int3:     return MTL::VertexFormatInt3;
            case ShaderDataType::Int4:     return MTL::VertexFormatInt4;
            case ShaderDataType::Bool:     return MTL::VertexFormatInt; // I don't see Bool, but I think it can be somthing like 1- True, 0- False
            default:
                printf("Unsupported ShaderDataType for Metal vertex format\n");
                return MTL::VertexFormatInvalid;
        }
    }

class MetalVertexArray : public VertexArray {
    public:
        ~MetalVertexArray() override;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) override;
        void AddIndexBuffer(Ref<IndexBuffer>& indexBuffer) override;

        Ref<VertexBuffer> GetVertexBuffer(uint32_t index) override
        {
            return m_vertexBuffers.at(index);
        }

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override
        {
            return m_vertexBuffers;
        }

        Ref<IndexBuffer> GetIndexBuffer() override
        {
            return m_indexBuffers;
        }

        MTL::VertexDescriptor* CreateMetalVertexDescriptor(const BufferLayout& layout, uint32_t bufferIndex = 0);
        [[nodiscard]] MTL::VertexDescriptor* GetVertexDescriptor() const { return m_VertexDescriptor; }

    private:
        std::vector<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffers;
        MTL::VertexDescriptor* m_VertexDescriptor = nullptr;
};

} // Nebula

#endif //METALVERTEXARRAY_H
