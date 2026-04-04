//
// Created by Christian Cruz on 12/20/25.
//

#include "Render.h"
#include <memory>
#include "RenderCommand.h"

namespace Nebula {
    /////////////////////////////////////////  RENDERER /////////////////////////////////////////////////
    Ref<RendererAPI> Renderer::s_RendererAPI = nullptr;
    Renderer::SceneData *Renderer::m_SceneData = new SceneData();

    RendererAPI* Renderer::GetRendererAPI() {
        return RendererAPI::Get();
    }

    void Renderer::Submit(Ref<Mesh>& mesh, Ref<Material>& material, const glm::mat4 &transform) {
        m_SceneData->DrawList.push_back({ mesh, material, transform });
    }

    void Renderer::SubmitModel(Ref<Model>& model, Ref<Material>& material, glm::mat4& transform)
    {
        for (auto& mesh : model->GetMeshes())
        {
            Ref<Mesh> m = mesh; // Submit takes non-const ref
            Submit(m, const_cast<Ref<Material>&>(material), transform);
        }
    }

    void Renderer::Flush() {
        for (auto& cmd : m_SceneData->DrawList)
        {
            bool doubleSided = cmd.material->IsDoubleSided();
            if (doubleSided)
                s_RendererAPI->SetCulling(false);
            auto vp = m_SceneData->ViewProjectionMatrix;
            cmd.material->Bind(cmd.mesh->GetVertexArray());

            const auto shader = cmd.material->GetShader();

            shader->SetMat4("viewProjection", m_SceneData->ViewProjectionMatrix);
            shader->SetMat4("model", cmd.transform);

            s_RendererAPI->DrawMesh(cmd.mesh);

            if (doubleSided)
                s_RendererAPI->SetCulling(true);
        }
            m_SceneData->DrawList.clear();
    }

    void Renderer::BeginScene(const Ref<Camera>& camera) {
        m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
        auto& vp = m_SceneData->ViewProjectionMatrix;
    }

    void Renderer::EndScene() {
        Flush();
    }

    /////////////////////////////////////////  RENDERER 2D ///////////////////////////////////////////////
    // Render quads
    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {

    }

    // Render quads with textures
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, Texture2D *texture) {
        // Ref<Mesh> quadMesh = GetQuadMesh();
        // Ref<Material> material = GetColorMaterial(color);
        //
        // Renderer::Submit(quadMesh, material, transform);
    }

    void Renderer2D::DrawPixel(int posX, int posY, Color color) {
    }

    void Renderer2D::DrawPixelV(Vector2 position, Color color) {
    }

    void Renderer2D::DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
        DrawLineEx({glm::vec2{(float)startPosX,(float)startPosY}}, {glm::vec2{(float)endPosX,(float)endPosY}}, 1.0f, color);
    }

    void Renderer2D::DrawLineV(Vector2 startPos, Vector2 endPos, Color color) {
        DrawLineEx(startPos, endPos, 1.0f, color);
    }

    void Renderer2D::DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color) {
        glm::vec2 a = startPos.position;
        glm::vec2 b = endPos.position;
        glm::vec2 dir = glm::normalize(b - a);
        glm::vec2 perp = glm::vec2(-dir.y, dir.x) * (thick * 0.5f);

        std::vector<float> verts = {
            a.x - perp.x, a.y - perp.y, 0.f,  0,0,1,  0,0,
            a.x + perp.x, a.y + perp.y, 0.f,  0,0,1,  0,1,
            b.x + perp.x, b.y + perp.y, 0.f,  0,0,1,  1,1,
            b.x - perp.x, b.y - perp.y, 0.f,  0,0,1,  1,0,
        };
        std::vector<uint32_t> indices = { 0,1,2, 2,3,0 };

        // auto mesh = CreateMeshFromVertices(verts, indices);
        // auto mat  = CreateFlatColorMaterial(ColorToVec4(color));
        // Renderer::Submit(mesh, mat, glm::mat4(1.f));
    }

    void Renderer2D::DrawLineStrip(const Vector2 *points, int pointCount, Color color) {
    }

    void Renderer2D::DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color) {
    }

    void Renderer2D::DrawCircle(int centerX, int centerY, float radius, Color color) {
        // static Ref<Mesh> s_CircleMesh = MeshFactory::CreateCircle(1.0f, 32);
        //
        // auto material = Material::CreateFlatColorMaterial(color);
        //
        // glm::mat4 transform = glm::translate(glm::mat4(1.f), { (float)centerX, (float)centerY, 0.f })
        //                     * glm::scale(glm::mat4(1.f), { radius, radius, 1.f });
        //
        // Renderer::Submit(s_CircleMesh, material, transform);
    }

    void Renderer2D::DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color) {
    }

    void Renderer2D::DrawCircleSectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color) {
    }

    void Renderer2D::DrawCircleGradient(int centerX, int centerY, float radius, Color inner, Color outer) {
    }

    void Renderer2D::DrawCircleV(Vector2 center, float radius, Color color) {
    }

    void Renderer2D::DrawCircleLines(int centerX, int centerY, float radius, Color color) {
    }

    void Renderer2D::DrawCircleLinesV(Vector2 center, float radius, Color color) {
    }

    void Renderer2D::DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    }

    void Renderer2D::DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    }

    void Renderer2D::DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color) {
    }

    void Renderer2D::DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color) {
    }

    void Renderer2D::DrawRectangle(int posX, int posY, int width, int height, Color color) {
    }

    void Renderer2D::DrawRectangleV(Vector2 position, Vector2 size, Color color) {
    }

    void Renderer2D::DrawRectangleRec(Rectangle rec, Color color) {
    }

    void Renderer2D::DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color) {
    }

    void Renderer2D::DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom) {
    }

    void Renderer2D::DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right) {
    }

    void Renderer2D::DrawRectangleGradientEx(Rectangle rec, Color topLeft, Color bottomLeft, Color topRight, Color bottomRight) {
    }

    void Renderer2D::DrawRectangleLines(int posX, int posY, int width, int height, Color color) {
    }

    void Renderer2D::DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color) {
    }

    void Renderer2D::DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) {
    }

    void Renderer2D::DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, Color color) {
    }

    void Renderer2D::DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color) {
    }

    void Renderer2D::DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
        std::vector<float> verts = {
            v1.position.x, v1.position.y, 0.f,  0,0,1,  0,0,
            v2.position.x, v2.position.y, 0.f,  0,0,1,  0.5f,1,
            v3.position.x, v3.position.y, 0.f,  0,0,1,  1,0,
        };
        std::vector<uint32_t> indices = { 0, 1, 2 };
        // auto mesh = CreateMeshFromVertices(verts, indices);
        // auto mat  = CreateFlatColorMaterial(ColorToVec4(color));
        // Renderer::Submit(mesh, mat, glm::mat4(1.f));
    }

    void Renderer2D::DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
    }

    void Renderer2D::DrawTriangleFan(const Vector2 *points, int pointCount, Color color) {
    }

    void Renderer2D::DrawTriangleStrip(const Vector2 *points, int pointCount, Color color) {
    }

    void Renderer2D::DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color) {
    }

    void Renderer2D::DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color) {
    }

    void Renderer2D::DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color) {
    }

    void Renderer2D::DrawSplineLinear(const Vector2 *points, int pointCount, float thick, Color color) {
        for (int i = 0; i < pointCount - 1; i++)
            DrawLineEx(points[i], points[i+1], thick, color);
    }

    void Renderer2D::DrawSplineBasis(const Vector2 *points, int pointCount, float thick, Color color) {
    }

    void Renderer2D::DrawSplineCatmullRom(const Vector2 *points, int pointCount, float thick, Color color) {
        const int steps = 12; // segments per span
        for (int i = 0; i < pointCount - 1; i++) {
            Vector2 p0 = points[std::max(i-1, 0)];
            Vector2 p1 = points[i];
            Vector2 p2 = points[i+1];
            Vector2 p3 = points[std::min(i+2, pointCount-1)];
            for (int s = 0; s < steps; s++) {
                float t0 = (float)s / steps;
                float t1 = (float)(s+1) / steps;
                Vector2 a = { GetSplinePointCatmullRom(p0, p1, p2, p3, t0) };
                Vector2 b = { GetSplinePointCatmullRom(p0, p1, p2, p3, t1) };
                DrawLineEx(a, b, thick, color);
            }
        }
    }

    void Renderer2D::DrawSplineBezierQuadratic(const Vector2 *points, int pointCount, float thick, Color color) {
    }

    void Renderer2D::DrawSplineBezierCubic(const Vector2 *points, int pointCount, float thick, Color color) {
    }

    void Renderer2D::DrawSplineSegmentLinear(Vector2 p1, Vector2 p2, float thick, Color color) {
    }

    void Renderer2D::DrawSplineSegmentBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color) {
    }

    void Renderer2D::DrawSplineSegmentCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color) {
    }

    void Renderer2D::DrawSplineSegmentBezierQuadratic(Vector2 p1, Vector2 c2, Vector2 p3, float thick, Color color) {
    }

    void Renderer2D::DrawSplineSegmentBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float thick, Color color) {
    }

    Vector2 Renderer2D::GetSplinePointLinear(Vector2 startPos, Vector2 endPos, float t) {
    }

    Vector2 Renderer2D::GetSplinePointBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t) {
    }

    Vector2 Renderer2D::GetSplinePointCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t) {
        float t2 = t*t, t3 = t2*t;
        glm::vec2 result = 0.5f * (
            (2.f * p2.position) +
            (-p1.position + p3.position) * t +
            (2.f*p1.position - 5.f*p2.position + 4.f*p3.position - p4.position) * t2 +
            (-p1.position + 3.f*p2.position - 3.f*p3.position + p4.position) * t3
        );
        return { result };
    }

    Vector2 Renderer2D::GetSplinePointBezierQuad(Vector2 p1, Vector2 c2, Vector2 p3, float t) {
    }

    Vector2 Renderer2D::GetSplinePointBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float t) {
    }

    /////////////////////////////////////////  RENDERER 3D ///////////////////////////////////////////////
    void Renderer3D::DrawMesh(Ref<Mesh> mesh, Ref<Material> material, const glm::mat4 &transform) {
        Renderer::Submit(mesh, material, transform);
    }

    void Renderer3D::DrawLine3D(Vector3 startPos, Vector3 endPos, Color color) {
    }

    void Renderer3D::DrawPoint3D(Vector3 position, Color color) {
    }

    void Renderer3D::DrawCircle3D(Vector3 center, float radius, Vector3 rotationAxis, float rotationAngle, Color color) {
    }

    void Renderer3D::DrawTriangle3D(Vector3 v1, Vector3 v2, Vector3 v3, Color color) {
    }

    void Renderer3D::DrawTriangleStrip3D(const Vector3 *points, int pointCount, Color color) {
    }

    void Renderer3D::DrawCube(Vector3 position, float width, float height, float length, Color color) {
    }

    void Renderer3D::DrawCubeV(Vector3 position, Vector3 size, Color color) {
    }

    void Renderer3D::DrawCubeWires(Vector3 position, float width, float height, float length, Color color) {
    }

    void Renderer3D::DrawCubeWiresV(Vector3 position, Vector3 size, Color color) {
    }

    void Renderer3D::DrawSphere(Vector3 centerPos, float radius, Color color) {
    }

    void Renderer3D::DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color) {
    }

    void Renderer3D::DrawSphereWires(Vector3 centerPos, float radius, int rings, int slices, Color color) {
    }

    void Renderer3D::DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color) {
    }

    void Renderer3D::DrawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color) {
    }

    void Renderer3D::DrawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color) {
    }

    void Renderer3D::DrawCylinderWiresEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color) {
    }

    void Renderer3D::DrawCapsule(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color) {
    }

    void Renderer3D::DrawCapsuleWires(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color) {
    }

    void Renderer3D::DrawPlane(Vector3 centerPos, Vector2 size, Color color) {
    }

    void Renderer3D::DrawRay(Ray ray, Color color) {
    }

    void Renderer3D::DrawGrid(int slices, float spacing) {
    }

    void Renderer3D::DrawModel(Model model, Vector3 position, float scale, Color tint) {
    }

    void Renderer3D::DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) {
    }

    void Renderer3D::DrawModelWires(Model model, Vector3 position, float scale, Color tint) {
    }

    void Renderer3D::DrawModelWiresEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) {
    }

    void Renderer3D::DrawModelPoints(Model model, Vector3 position, float scale, Color tint) {
    }

    void Renderer3D::DrawModelPointsEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) {
    }

    void Renderer3D::DrawBillboard(Camera camera, Ref<Texture2D> texture, Vector3 position, float scale, Color tint) {
    }

    void Renderer3D::DrawBillboardRec(Camera camera, Ref<Texture2D> texture, Rectangle source, Vector3 position, Vector2 size, Color tint) {
    }

    void Renderer3D::DrawBillboardPro(Camera camera, Ref<Texture2D> texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint) {
    }

    void Renderer3D::DrawMesh(Mesh mesh, Material material, Matrix transform) {
    }

    void Renderer3D::DrawMeshInstanced(Mesh mesh, Material material, const Matrix *transforms, int instances) {
    }

}
