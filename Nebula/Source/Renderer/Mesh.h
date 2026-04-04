//
// Created by Christian Cruz on 3/16/26.
//

#ifndef MESH_H
#define MESH_H

#include "Material.h"
#include <vector>

namespace Nebula {
    class VertexBuffer;
    class IndexBuffer;
    class VertexArray;
}

namespace Nebula {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 color;
        // This for later
        // glm::vec3 tangent;
        // glm::vec3 bitangent;
    };

    class Mesh {
        public:
            Mesh( const std::vector<Ref<VertexBuffer>>& vertices, const Ref<IndexBuffer>& indices);
            Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
            ~Mesh() = default;

            [[nodiscard]] const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

        private:
            std::vector<Ref<VertexBuffer>> m_VertexBuffer;
            Ref<IndexBuffer> m_IndexBuffer;
            Ref<VertexArray> m_VertexArray;

            void setupMesh();
    };

    struct MeshFactory {
        static Ref<Mesh> CreateQuad();
        static Ref<Mesh> CreateTriangle();
        static Ref<Mesh> CreateCircle(float radius = 1.0f, int segments = 32);
        static Ref<Mesh> CreateCube();
        static Ref<Mesh> CreateSphere(float radius = 1.0f, int stacks = 16, int slices = 32);
        static Ref<Mesh> CreateCone(float radius = 1.0f, float height = 1.0f, int slices = 32);
        static Ref<Mesh> CreateTorus(float majorRadius = 1.0f, float minorRadius = 0.25f,
                                     int majorSegments = 32, int minorSegments = 16);
    };
} // Nebula

#endif //MESH_H
