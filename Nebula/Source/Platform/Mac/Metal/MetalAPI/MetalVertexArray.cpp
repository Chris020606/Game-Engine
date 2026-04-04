//
// Created by Christian Cruz on 2/5/26.
//

#include "MetalVertexArray.h"
#include "MetalBuffers.h"

namespace Nebula {

    MetalVertexArray::~MetalVertexArray() {
        m_vertexBuffers.clear();
        m_indexBuffers.reset();
        if (m_VertexDescriptor) {
            m_VertexDescriptor->release();
            m_VertexDescriptor = nullptr;
        }
    }

    MTL::VertexDescriptor* MetalVertexArray::CreateMetalVertexDescriptor(const BufferLayout& layout, uint32_t bufferIndex) {
        MTL::VertexDescriptor* vd = MTL::VertexDescriptor::vertexDescriptor();

        const auto& elements = layout.GetElements();

        // Set up each attribute
        for (size_t i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];

            auto* attr = vd->attributes()->object(i);
            attr->setFormat(ShaderDataTypeToMetalFormat(element.type));
            attr->setOffset(element.offset);
            attr->setBufferIndex(bufferIndex);
        }

        // Set up buffer layout (stride and step function)
        auto* bufferLayout = vd->layouts()->object(bufferIndex);
        bufferLayout->setStride(layout.GetStride());
        bufferLayout->setStepFunction(MTL::VertexStepFunctionPerVertex);

        return vd;
    }

    void MetalVertexArray::Bind() const {
    }

    void MetalVertexArray::Unbind() const {
    }

    void MetalVertexArray::AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) {
        MTL::VertexDescriptor* vd = MTL::VertexDescriptor::vertexDescriptor();

        const auto& layout = vertexBuffer->GetLayout();
        uint32_t index = 0;
        for (const auto& element : layout) {
            auto* attr = vd->attributes()->object(index);
            attr->setFormat(ShaderDataTypeToMetalFormat(element.type));
            attr->setOffset(element.offset);
            attr->setBufferIndex(0);

            index++;
        }

        auto* bufferLayout = vd->layouts()->object(0);
        bufferLayout->setStride(layout.GetStride());
        bufferLayout->setStepFunction(MTL::VertexStepFunctionPerVertex);

        m_VertexDescriptor = vd->retain();
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void MetalVertexArray::AddIndexBuffer(Ref<IndexBuffer>& indexBuffer) {
        if (!indexBuffer) {
            std::printf("ERROR: Attempted to add null index buffer\n");
            return;
        }

        m_indexBuffers = indexBuffer;
    }

   // std::vector<Ref<VertexBuffer>>& MetalVertexArray::GetVertexBuffers() {
   //     return m_vertexBuffers;
   // }

    //Ref<IndexBuffer>& MetalVertexArray::GetIndexBuffer() {
    //    return m_indexBuffers;
    //}

} // Nebula