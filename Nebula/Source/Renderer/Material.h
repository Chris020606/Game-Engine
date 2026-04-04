#ifndef MATERIAL_H
#define MATERIAL_H
#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Core.h"
#include <string>

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "glm/glm.hpp"

#define LIGHTGRAY  (Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       (Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   (Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     (Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       (Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     (Color){ 255, 161, 0, 255 }     // Orange
#define PINK       (Color){ 255, 109, 194, 255 }   // Pink
#define RED        (Color){ 230, 41, 55, 255 }     // Red
#define MAROON     (Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      (Color){ 0, 228, 48, 255 }      // Green
#define LIME       (Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  (Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    (Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       (Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   (Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     (Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     (Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE (Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      (Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      (Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  (Color){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      (Color){ 255, 255, 255, 255 }   // White
#define BLACK      (Color){ 0, 0, 0, 255 }         // Black
#define BLANK      (Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    (Color){ 255, 0, 255, 255 }     // Magenta

namespace Nebula {

    struct Color {
        glm::vec4 color;
    };

    struct Vector2 {
        glm::vec2 position;
    };

    struct Vector3 {
        glm::vec3 position;
    };

    struct Rectangle {

    };

    struct Matrix {
        glm::mat4 matrix;
    };

    struct Ray {
        Vector3 origin;
        Vector3 direction;
    };

class Material {
public:
    Material(const Ref<Shader>& shader, std::unordered_map<std::string, Ref<Texture>> texture);
    void SetTexture(const std::string& name, const Ref<Texture>& texture);
    Ref<Texture> GetTexture(const std::string& name);

    [[nodiscard]] Ref<Shader> GetShader() const { return m_Shader;}

    void SetFloat(const std::string& name, float value) const;
    void SetFloat2(const std::string& name, glm::vec2 value) const;
    void SetFloat3(const std::string& name, glm::vec3 value) const;
    void SetFloat4(const std::string& name, glm::vec4 value) const;
    void SetInt(const std::string& name, int value) const;
    void SetMat4(const std::string& name, glm::mat4 value) const;

    void Bind() const;
    void Bind(const Ref<VertexArray>& va) const;

    void SetDoubleSided(bool value) { m_DoubleSided = value; }
    bool IsDoubleSided() const { return m_DoubleSided; }

    static glm::vec4 CreateFlatColorMaterial(Color color) { return {color.color.x, color.color.y, color.color.z, color.color.w}; };

private:
    Ref<Shader> m_Shader;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, glm::vec3> vec3s;
    std::unordered_map<std::string, glm::vec4> vec4s;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, Ref<Texture>> textures;
    std::unordered_map<std::string, uint32_t> m_TextureSlots;
    bool m_DoubleSided = false;
};

} // Nebula

#endif //MATERIAL_H
