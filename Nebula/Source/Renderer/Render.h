//
// Created by Christian Cruz on 12/20/25.
//

#ifndef RENDER_H
#define RENDER_H

#include "Cameras.h"
#include "Mesh.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "Model.h"
#include "Core.h"

namespace Nebula {

    struct RendererSettings {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;
        uint32_t VertexCount = 0;
        uint32_t IndexCount = 0;
    };

    struct DrawCommand
    {
        Ref<Mesh> mesh;
        Ref<Material> material;
        glm::mat4 transform;
    };

    class Renderer {
        public:
            static void BeginScene(const Ref<Camera>& camera);
            static void EndScene();
            static void SetRendererAPI(const Ref<RendererAPI>& api) { s_RendererAPI = api; }
            static RendererAPI* GetRendererAPI();
            /// TODO make an overload function that also accepts models, or maybe added to the material system
            static void Submit( Ref<Mesh>& mesh,
                                Ref<Material>& material,
                                const glm::mat4& transform = glm::mat4(1.0f));
            static void SubmitModel( Ref<Model>& model, Ref<Material>& material, glm::mat4& transform);

        private:
            static Ref<RendererAPI> s_RendererAPI;

            struct SceneData {
                glm::mat4 ViewProjectionMatrix;
                std::vector<DrawCommand> DrawList;
            };
            static SceneData *m_SceneData;

            static void Flush();
        };

    class Renderer2D {
        public:

            static void DrawQuad(const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));
            static void DrawQuad(const glm::vec3& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), Texture2D* texture = nullptr);

            // Basic shapes drawing functions
            static void DrawPixel(int posX, int posY, Color color);                                                   // Draw a pixel using geometry [Can be slow, use with care]
            static void DrawPixelV(Vector2 position, Color color);                                                    // Draw a pixel using geometry (Vector version) [Can be slow, use with care]
            static void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);                // Draw a line
            static void DrawLineV(Vector2 startPos, Vector2 endPos, Color color);                                     // Draw a line (using gl lines)
            static void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);                       // Draw a line (using triangles/quads)
            static void DrawLineStrip(const Vector2 *points, int pointCount, Color color);                            // Draw lines sequence (using gl lines)
            static void DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color);                   // Draw line segment cubic-bezier in-out interpolation
            static void DrawCircle(int centerX, int centerY, float radius, Color color);                              // Draw a color-filled circle
            static void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color);      // Draw a piece of a circle
            static void DrawCircleSectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color); // Draw circle sector outline
            static void DrawCircleGradient(int centerX, int centerY, float radius, Color inner, Color outer);         // Draw a gradient-filled circle
            static void DrawCircleV(Vector2 center, float radius, Color color);                                       // Draw a color-filled circle (Vector version)
            static void DrawCircleLines(int centerX, int centerY, float radius, Color color);                         // Draw circle outline
            static void DrawCircleLinesV(Vector2 center, float radius, Color color);                                  // Draw circle outline (Vector version)
            static void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);             // Draw ellipse
            static void DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color);        // Draw ellipse outline
            static void DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color); // Draw ring
            static void DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color);    // Draw ring outline
            static void DrawRectangle(int posX, int posY, int width, int height, Color color);                        // Draw a color-filled rectangle
            static void DrawRectangleV(Vector2 position, Vector2 size, Color color);                                  // Draw a color-filled rectangle (Vector version)
            static void DrawRectangleRec(Rectangle rec, Color color);                                                 // Draw a color-filled rectangle
            static void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color);                 // Draw a color-filled rectangle with pro parameters
            static void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);   // Draw a vertical-gradient-filled rectangle
            static void DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right);   // Draw a horizontal-gradient-filled rectangle
            static void DrawRectangleGradientEx(Rectangle rec, Color topLeft, Color bottomLeft, Color topRight, Color bottomRight); // Draw a gradient-filled rectangle with custom vertex colors
            static void DrawRectangleLines(int posX, int posY, int width, int height, Color color);                   // Draw rectangle outline
            static void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);                            // Draw rectangle outline with extended parameters
            static void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);              // Draw rectangle with rounded edges
            static void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, Color color);         // Draw rectangle lines with rounded edges
            static void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color); // Draw rectangle with rounded edges outline
            static void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
            static void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);                           // Draw triangle outline (vertex in counter-clockwise order!)
            static void DrawTriangleFan(const Vector2 *points, int pointCount, Color color);                          // Draw a triangle fan defined by points (first vertex is the center)
            static void DrawTriangleStrip(const Vector2 *points, int pointCount, Color color);                        // Draw a triangle strip defined by points
            static void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);               // Draw a regular polygon (Vector version)
            static void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);          // Draw a polygon outline of n sides
            static void DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color); // Draw a polygon outline of n sides with extended parameters

            // Splines drawing functions
            static void DrawSplineLinear(const Vector2 *points, int pointCount, float thick, Color color);                  // Draw spline: Linear, minimum 2 points
            static void DrawSplineBasis(const Vector2 *points, int pointCount, float thick, Color color);                   // Draw spline: B-Spline, minimum 4 points
            static void DrawSplineCatmullRom(const Vector2 *points, int pointCount, float thick, Color color);              // Draw spline: Catmull-Rom, minimum 4 points
            static void DrawSplineBezierQuadratic(const Vector2 *points, int pointCount, float thick, Color color);         // Draw spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
            static void DrawSplineBezierCubic(const Vector2 *points, int pointCount, float thick, Color color);             // Draw spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
            static void DrawSplineSegmentLinear(Vector2 p1, Vector2 p2, float thick, Color color);                    // Draw spline segment: Linear, 2 points
            static void DrawSplineSegmentBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color); // Draw spline segment: B-Spline, 4 points
            static void DrawSplineSegmentCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color); // Draw spline segment: Catmull-Rom, 4 points
            static void DrawSplineSegmentBezierQuadratic(Vector2 p1, Vector2 c2, Vector2 p3, float thick, Color color); // Draw spline segment: Quadratic Bezier, 2 points, 1 control point
            static void DrawSplineSegmentBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float thick, Color color); // Draw spline segment: Cubic Bezier, 2 points, 2 control points

            // Spline segment point evaluation functions, for a given t [0.0f .. 1.0f]
            static Vector2 GetSplinePointLinear(Vector2 startPos, Vector2 endPos, float t);                           // Get (evaluate) spline point: Linear
            static Vector2 GetSplinePointBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t);              // Get (evaluate) spline point: B-Spline
            static Vector2 GetSplinePointCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t);         // Get (evaluate) spline point: Catmull-Rom
            static Vector2 GetSplinePointBezierQuad(Vector2 p1, Vector2 c2, Vector2 p3, float t);                     // Get (evaluate) spline point: Quadratic Bezier
            static Vector2 GetSplinePointBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float t);        // Get (evaluate) spline point: Cubic Bezier

        private:
            static Ref<RendererAPI> s_RendererAPI;
    };

    class Renderer3D {
        public:

            static void DrawMesh( Ref<Mesh> mesh,
                                  Ref<Material> material,
                                  const glm::mat4& transform = glm::mat4(1.0f));

            // Basic geometric 3D shapes drawing functions
            static void DrawLine3D(Vector3 startPos, Vector3 endPos, Color color);                                    // Draw a line in 3D world space
            static void DrawPoint3D(Vector3 position, Color color);                                                   // Draw a point in 3D space, actually a small line
            static void DrawCircle3D(Vector3 center, float radius, Vector3 rotationAxis, float rotationAngle, Color color); // Draw a circle in 3D world space
            static void DrawTriangle3D(Vector3 v1, Vector3 v2, Vector3 v3, Color color);                              // Draw a color-filled triangle (vertex in counter-clockwise order!)
            static void DrawTriangleStrip3D(const Vector3 *points, int pointCount, Color color);                      // Draw a triangle strip defined by points
            static void DrawCube(Vector3 position, float width, float height, float length, Color color);             // Draw cube
            static void DrawCubeV(Vector3 position, Vector3 size, Color color);                                       // Draw cube (Vector version)
            static void DrawCubeWires(Vector3 position, float width, float height, float length, Color color);        // Draw cube wires
            static void DrawCubeWiresV(Vector3 position, Vector3 size, Color color);                                  // Draw cube wires (Vector version)
            static void DrawSphere(Vector3 centerPos, float radius, Color color);                                     // Draw sphere
            static void DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color);            // Draw sphere with extended parameters
            static void DrawSphereWires(Vector3 centerPos, float radius, int rings, int slices, Color color);         // Draw sphere wires
            static void DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); // Draw a cylinder/cone
            static void DrawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color); // Draw a cylinder with base at startPos and top at endPos
            static void DrawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); // Draw a cylinder/cone wires
            static void DrawCylinderWiresEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color); // Draw a cylinder wires with base at startPos and top at endPos
            static void DrawCapsule(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color); // Draw a capsule with the center of its sphere caps at startPos and endPos
            static void DrawCapsuleWires(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color); // Draw capsule wireframe with the center of its sphere caps at startPos and endPos
            static void DrawPlane(Vector3 centerPos, Vector2 size, Color color);                                      // Draw a plane XZ
            static void DrawRay(Ray ray, Color color);                                                                // Draw a ray line
            static void DrawGrid(int slices, float spacing);                                                          // Draw a grid (centered at (0, 0, 0))

            // Model drawing functions
            static void DrawModel(Model model, Vector3 position, float scale, Color tint);               // Draw a model (with texture if set)
            static void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model with extended parameters
            static void DrawModelWires(Model model, Vector3 position, float scale, Color tint);          // Draw a model wires (with texture if set)
            static void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model wires (with texture if set) with extended parameters
            static void DrawModelPoints(Model model, Vector3 position, float scale, Color tint); // Draw a model as points
            static void DrawModelPointsEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model as points with extended parameters
            // void DrawBoundingBox(BoundingBox box, Color color);                                   // Draw bounding box (wires)
            static void DrawBillboard(Camera camera, Ref<Texture2D> texture, Vector3 position, float scale, Color tint);   // Draw a billboard texture
            static void DrawBillboardRec(Camera camera, Ref<Texture2D> texture, Rectangle source, Vector3 position, Vector2 size, Color tint); // Draw a billboard texture defined by source
            static void DrawBillboardPro(Camera camera, Ref<Texture2D> texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint); // Draw a billboard texture defined by source and rotation

            // Mesh
            static void DrawMesh(Mesh mesh, Material material, Matrix transform);                        // Draw a 3d mesh with material and transform
            static void DrawMeshInstanced(Mesh mesh, Material material, const Matrix *transforms, int instances); // Draw multiple mesh instances with material and different transforms

        private:
            static Ref<RendererAPI> s_RendererAPI;
    };


}


#endif //RENDER_H
