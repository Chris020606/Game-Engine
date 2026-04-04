//
// Created by Christian Cruz on 3/15/26.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace Nebula {
    class Transform {
    public:
        Transform();
        explicit operator glm::mat4() const;

        glm::vec3 GetRotation() const { return glm::degrees(m_Rotation); }
        glm::vec3 GetTranslation() const { return m_Translation; }
        glm::vec3 GetScale() const { return m_Scale; }

        glm::mat4 GetMatrix() const;
        const float* GetTransform() const;

        void SetRotation(const glm::vec3& angles);
        void SetTranslation(const glm::vec3& translation);
        void SetScale(const glm::vec3& scale);

        void IncrementRotation(const glm::vec3& delta);
        void IncrementTranslation(const glm::vec3& delta);
        void IncrementScale(const glm::vec3& delta);

    private:
        mutable bool m_MatrixDirty = true;
        mutable glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

        glm::vec3 m_Translation = {0,0,0};
        glm::vec3 m_Rotation = {0,0,0};
        glm::vec3 m_Scale = {1,1,1};

        void RecalculateMatrix() const;

    };
}

#endif //TRANSFORM_H
