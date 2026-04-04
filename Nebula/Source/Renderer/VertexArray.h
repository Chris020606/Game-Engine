//
// Created by Christian Cruz on 2/2/26.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vector>

#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Core.h"
namespace Nebula {
    class VertexBuffer;
    class IndexBuffer;
}

namespace Nebula {

class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void AddIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;

        virtual Ref<VertexBuffer> GetVertexBuffer(uint32_t index) = 0;
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual Ref<IndexBuffer> GetIndexBuffer() = 0;

        static VertexArray* Create();
};

} // Nebula

#endif //VERTEXARRAY_H
