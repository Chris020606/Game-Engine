//
// Created by Christian Cruz on 1/19/26.
//

#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H
#include "VertexBuffers.h"

namespace Nebula {

class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void UnBind() const override;

        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        const BufferLayout& GetLayout() override { return m_Layout; }

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
};

    class OpenGLIndexBuffer : public IndexBuffer {
        public:
            OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
            ~OpenGLIndexBuffer() override;

            void Bind() const override;
            void UnBind() const override;

            uint32_t GetCount() const override { return m_Count;}

        private:
            uint32_t m_Count;
            uint32_t m_RendererID;

    };

} // Nebula

#endif //OPENGLBUFFER_H
