//
// Created by Christian Cruz on 4/5/26.
//

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "ID.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "Mesh.h"
#include "Cameras.h"
#include "Transform.h"
#include "Material.h"

namespace Nebula {
        struct IDComponent {
        IDComponent() = default;
        IDComponent(ID id)
            : id(id) {};

        ID id;
    };

    struct TagComponent {
        TagComponent() = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {};

        std::string Tag;
    };

    struct TransformComponent {
    public:
        TransformComponent();
        operator glm::mat4() const;

        inline glm::vec3 GetRotation() { return glm::degrees(m_Rotation); }
        inline glm::vec3 GetTranslation() { return m_Translation; }
        inline glm::vec3 GetScale() { return m_Scale; }

        // Returns the 4x4 Local to World Matrix
        glm::mat4 GetMatrix() const;
        float* GetTransform();

        void SetRotation(const glm::vec3& angles);
        void SetTranslation(const glm::vec3& translation);
        void SetScale(const glm::vec3& scale);

        void IncrementRotation(const glm::vec3& delta);
        void IncrementTranslation(const glm::vec3& delta);
        void IncrementScale(const glm::vec3& delta);

    private:
        mutable bool m_MatrixDirty;
        mutable glm::mat4 m_ModelMatrix;

        glm::vec3 m_Rotation;
        glm::vec3 m_Translation;
        glm::vec3 m_Scale;

        void RecalculateMatrix() const;
    };

    struct CameraComponent {
        CameraComponent() = default;

        Camera m_Camera;
        bool is_main = true;
        float fov = m_Camera.GetFov();
    };

    struct LightComponent {
        glm::vec3 Position;
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
    };

    struct MaterialComponent {
        glm::vec3 ambient  = { 0.2f, 0.2f, 0.2f };
        glm::vec3 diffuse  = { 0.8f, 0.8f, 0.8f };
        glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
        float shininess    = 32.0f;
        glm::vec3 color    = { 1.0f, 1.0f, 1.0f };
    };

    struct MeshComponent
    {
        Mesh* mesh = nullptr;
        ID guid = ID::INVALID;
        std::string sourceName = "";

        bool hasTexture = false;

        bool HasTexture() { return hasTexture; }
    };

}
#endif //COMPONENTS_H
