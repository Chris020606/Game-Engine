//
// Created by Christian Cruz on 3/16/26.
//

#include "Mesh.h"

#include <complex>

#include "VertexArray.h"
#include "VertexBuffers.h"
#include "../../../../../../../opt/homebrew/Cellar/glm/1.0.1/include/glm/ext/scalar_constants.hpp"
#include "../../../../../../../opt/homebrew/Cellar/glm/1.0.1/include/glm/gtc/constants.hpp"

namespace Nebula {

    static Ref<Mesh> MakeMesh(const std::vector<Vertex>& verts,
                           const std::vector<uint32_t>& indices)
    {
        return std::make_shared<Mesh>(verts, indices);
    }

    Mesh::Mesh(const std::vector<Ref<VertexBuffer>> &vertices, const Ref<IndexBuffer> &indices)
            : m_VertexBuffer(vertices), m_IndexBuffer(indices)
    {
        setupMesh();
    }

    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) {
        auto vb = Ref<VertexBuffer>(VertexBuffer::Create(
            (float*)vertices.data(),
            vertices.size() * sizeof(Vertex)
        ));

        vb->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float3, "a_Color"    },
        });

        auto ib = Ref<IndexBuffer>(IndexBuffer::Create(
            (uint32_t*)indices.data(),
            indices.size()
        ));

        m_VertexBuffer = { vb };
        m_IndexBuffer  = ib;
        setupMesh();
    }

    void Mesh::setupMesh() {
        m_VertexArray = Ref<VertexArray>(VertexArray::Create());

        for (auto& vb : m_VertexBuffer)
        {
            m_VertexArray->AddVertexBuffer(vb);
        }

        m_VertexArray->AddIndexBuffer(m_IndexBuffer);
    }

    Ref<Mesh> MeshFactory::CreateQuad() {
        std::vector<Vertex> verts = {
            // position             normal           texCoord      color
            {{ -0.5f, -0.5f, 0.f }, { 0,0,1 }, { 0.f, 0.f }, { 1,1,1 }},
            {{  0.5f, -0.5f, 0.f }, { 0,0,1 }, { 1.f, 0.f }, { 1,1,1 }},
            {{  0.5f,  0.5f, 0.f }, { 0,0,1 }, { 1.f, 1.f }, { 1,1,1 }},
            {{ -0.5f,  0.5f, 0.f }, { 0,0,1 }, { 0.f, 1.f }, { 1,1,1 }},
        };
        std::vector<uint32_t> indices = { 0,1,2, 2,3,0 };
        return MakeMesh(verts, indices);
    }

    Ref<Mesh> MeshFactory::CreateTriangle() {
        // Vertices placed so the centroid is at the origin
        std::vector<Vertex> verts = {
            {{ -0.5f, -0.289f, 0.f }, { 0,0,1 }, { 0.f, 0.f }, { 1,1,1 }},
            {{  0.5f, -0.289f, 0.f }, { 0,0,1 }, { 1.f, 0.f }, { 1,1,1 }},
            {{  0.0f,  0.577f, 0.f }, { 0,0,1 }, { 0.5f,1.f }, { 1,1,1 }},
        };
        std::vector<uint32_t> indices = { 0,1,2 };
        return MakeMesh(verts, indices);
    }

    Ref<Mesh> MeshFactory::CreateCube() {
        const float s = 0.5f;

        std::vector<Vertex> verts = {
            // Front  (+Z)
            {{ -s,-s, s }, { 0, 0, 1 }, { 0,0 }, { 1,1,1 }},
            {{  s,-s, s }, { 0, 0, 1 }, { 1,0 }, { 1,1,1 }},
            {{  s, s, s }, { 0, 0, 1 }, { 1,1 }, { 1,1,1 }},
            {{ -s, s, s }, { 0, 0, 1 }, { 0,1 }, { 1,1,1 }},
            // Back   (-Z)
            {{  s,-s,-s }, { 0, 0,-1 }, { 0,0 }, { 1,1,1 }},
            {{ -s,-s,-s }, { 0, 0,-1 }, { 1,0 }, { 1,1,1 }},
            {{ -s, s,-s }, { 0, 0,-1 }, { 1,1 }, { 1,1,1 }},
            {{  s, s,-s }, { 0, 0,-1 }, { 0,1 }, { 1,1,1 }},
            // Left   (-X)
            {{ -s,-s,-s }, {-1, 0, 0 }, { 0,0 }, { 1,1,1 }},
            {{ -s,-s, s }, {-1, 0, 0 }, { 1,0 }, { 1,1,1 }},
            {{ -s, s, s }, {-1, 0, 0 }, { 1,1 }, { 1,1,1 }},
            {{ -s, s,-s }, {-1, 0, 0 }, { 0,1 }, { 1,1,1 }},
            // Right  (+X)
            {{  s,-s, s }, { 1, 0, 0 }, { 0,0 }, { 1,1,1 }},
            {{  s,-s,-s }, { 1, 0, 0 }, { 1,0 }, { 1,1,1 }},
            {{  s, s,-s }, { 1, 0, 0 }, { 1,1 }, { 1,1,1 }},
            {{  s, s, s }, { 1, 0, 0 }, { 0,1 }, { 1,1,1 }},
            // Top    (+Y)
            {{ -s, s, s }, { 0, 1, 0 }, { 0,0 }, { 1,1,1 }},
            {{  s, s, s }, { 0, 1, 0 }, { 1,0 }, { 1,1,1 }},
            {{  s, s,-s }, { 0, 1, 0 }, { 1,1 }, { 1,1,1 }},
            {{ -s, s,-s }, { 0, 1, 0 }, { 0,1 }, { 1,1,1 }},
            // Bottom (-Y)
            {{ -s,-s,-s }, { 0,-1, 0 }, { 0,0 }, { 1,1,1 }},
            {{  s,-s,-s }, { 0,-1, 0 }, { 1,0 }, { 1,1,1 }},
            {{  s,-s, s }, { 0,-1, 0 }, { 1,1 }, { 1,1,1 }},
            {{ -s,-s, s }, { 0,-1, 0 }, { 0,1 }, { 1,1,1 }},
        };

        // Each face = 2 triangles from 4 verts (offset by face * 4)
        std::vector<uint32_t> indices;
        for (uint32_t f = 0; f < 6; f++)
        {
            uint32_t base = f * 4;
            indices.insert(indices.end(), {
                base+0, base+1, base+2,
                base+2, base+3, base+0
            });
        }

        return MakeMesh(verts, indices);
    }

    Ref<Mesh> MeshFactory::CreateSphere(float radius, int stacks, int slices) {
        std::vector<Vertex> verts;
        std::vector<uint32_t> indices;

        for (int i = 0; i <= stacks; i++)
        {
            float phi = (float)i / stacks * glm::pi<float>();        // 0 → π  (top → bottom)
            float y   = cosf(phi) * radius;
            float r   = sinf(phi) * radius;                          // ring radius at this latitude

            for (int j = 0; j <= slices; j++)
            {
                float theta = (float)j / slices * glm::two_pi<float>(); // 0 → 2π
                float x = cosf(theta) * r;
                float z = sinf(theta) * r;

                glm::vec3 pos    = { x, y, z };
                glm::vec3 normal = glm::normalize(pos);              // sphere normal == normalize(position)
                glm::vec2 uv     = { (float)j / slices, (float)i / stacks };

                verts.push_back({ pos, normal, uv, {1,1,1} });
            }
        }

        // Stitch quads between rings
        for (int i = 0; i < stacks; i++)
        {
            for (int j = 0; j < slices; j++)
            {
                uint32_t a = (uint32_t)( i      * (slices + 1) + j    );
                uint32_t b = (uint32_t)( i      * (slices + 1) + j + 1);
                uint32_t c = (uint32_t)((i + 1) * (slices + 1) + j    );
                uint32_t d = (uint32_t)((i + 1) * (slices + 1) + j + 1);

                // Skip degenerate triangles at the poles
                if (i != 0)          indices.insert(indices.end(), { a, b, c });
                if (i != stacks - 1) indices.insert(indices.end(), { b, d, c });
            }
        }

        return MakeMesh(verts, indices);
    }

    Ref<Mesh> MeshFactory::CreateCone(float radius, float height, int slices) {
    std::vector<Vertex> verts;
    std::vector<uint32_t> indices;

    float half = height * 0.5f;

    // ── Side surface ──────────────────────────────────────────────────────────
    // Tip vertex (one per slice for correct normals)
    for (int i = 0; i < slices; i++)
    {
        float angle0 = (float) i      / slices * glm::two_pi<float>();
        float angle1 = (float)(i + 1) / slices * glm::two_pi<float>();

        float x0 = cosf(angle0) * radius,  z0 = sinf(angle0) * radius;
        float x1 = cosf(angle1) * radius,  z1 = sinf(angle1) * radius;

        // Side normal: perpendicular to slant edge
        glm::vec3 n0 = glm::normalize(glm::vec3(x0, radius / height, z0));
        glm::vec3 n1 = glm::normalize(glm::vec3(x1, radius / height, z1));
        glm::vec3 nt = glm::normalize((n0 + n1) * 0.5f);

        uint32_t base = (uint32_t)verts.size();

        verts.push_back({ { x0, -half, z0 }, n0, { (float)i/slices,     1.f }, {1,1,1} }); // base left
        verts.push_back({ { x1, -half, z1 }, n1, { (float)(i+1)/slices, 1.f }, {1,1,1} }); // base right
        verts.push_back({ { 0,   half,  0  }, nt, { (float)i/slices + 0.5f/slices, 0.f }, {1,1,1} }); // tip

        indices.insert(indices.end(), { base, base+1, base+2 });
    }

    // ── Base cap ──────────────────────────────────────────────────────────────
    uint32_t centerIdx = (uint32_t)verts.size();
    verts.push_back({ {0, -half, 0}, {0,-1,0}, {0.5f,0.5f}, {1,1,1} });

    for (int i = 0; i <= slices; i++)
    {
        float angle = (float)i / slices * glm::two_pi<float>();
        float x = cosf(angle) * radius,  z = sinf(angle) * radius;
        float u = 0.5f + cosf(angle) * 0.5f,  v = 0.5f + sinf(angle) * 0.5f;
        verts.push_back({ {x, -half, z}, {0,-1,0}, {u,v}, {1,1,1} });
    }

    for (int i = 0; i < slices; i++)
    {
        indices.insert(indices.end(), {
            centerIdx,
            centerIdx + (uint32_t)i + 2,
            centerIdx + (uint32_t)i + 1
        });
    }

    return MakeMesh(verts, indices);
    }

    Ref<Mesh> MeshFactory::CreateTorus(float majorRadius, float minorRadius, int majorSegments, int minorSegments) {
        std::vector<Vertex> verts;
        std::vector<uint32_t> indices;

        for (int i = 0; i <= majorSegments; i++)
        {
            float u     = (float)i / majorSegments * glm::two_pi<float>(); // angle around the ring
            float cosU  = cosf(u),  sinU = sinf(u);

            for (int j = 0; j <= minorSegments; j++)
            {
                float v     = (float)j / minorSegments * glm::two_pi<float>(); // angle around the tube
                float cosV  = cosf(v),  sinV = sinf(v);

                // Point on torus surface
                float x = (majorRadius + minorRadius * cosV) * cosU;
                float y =  minorRadius * sinV;
                float z = (majorRadius + minorRadius * cosV) * sinU;

                // Normal = direction from tube center to surface point
                glm::vec3 tubeCenter = { majorRadius * cosU, 0.f, majorRadius * sinU };
                glm::vec3 normal     = glm::normalize(glm::vec3(x,y,z) - tubeCenter);

                glm::vec2 uv = { (float)i / majorSegments, (float)j / minorSegments };

                verts.push_back({ {x,y,z}, normal, uv, {1,1,1} });
            }
        }

        int ring = minorSegments + 1;
        for (int i = 0; i < majorSegments; i++)
        {
            for (int j = 0; j < minorSegments; j++)
            {
                uint32_t a = (uint32_t)( i      * ring + j    );
                uint32_t b = (uint32_t)( i      * ring + j + 1);
                uint32_t c = (uint32_t)((i + 1) * ring + j    );
                uint32_t d = (uint32_t)((i + 1) * ring + j + 1);

                indices.insert(indices.end(), { a,c,b, b,c,d });
            }
        }

        return MakeMesh(verts, indices);
    }

    Ref<Mesh> MeshFactory::CreateCircle(float radius, int segments) {
        std::vector<Vertex> verts;
        std::vector<uint32_t> indices;

        // Center vertex
        verts.push_back({ {0,0,0}, {0,0,1}, {0.5f,0.5f}, {1,1,1} });

        for (int i = 0; i <= segments; i++)
        {
            float angle = (float)i / (float)segments * glm::two_pi<float>();
            float x = cosf(angle) * radius;
            float y = sinf(angle) * radius;
            float u = 0.5f + cosf(angle) * 0.5f;
            float v = 0.5f + sinf(angle) * 0.5f;
            verts.push_back({ {x,y,0}, {0,0,1}, {u,v}, {1,1,1} });
        }

        for (int i = 1; i <= segments; i++)
        {
            indices.push_back(0);
            indices.push_back((uint32_t)i);
            indices.push_back((uint32_t)(i + 1));
        }

        return MakeMesh(verts, indices);
    }

} // Nebula