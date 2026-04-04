//
// Created by Christian Cruz on 3/15/26.
//

#include "Transform.h"

namespace Nebula {

    Transform::Transform()
    : m_MatrixDirty(true),
      m_ModelMatrix(1.0),
      m_Translation(0.0f),
      m_Rotation(0.0f),
      m_Scale(1.0f) {
    }

    Transform::operator glm::mat4() const {
        return GetMatrix();
    }

    glm::mat4 Transform::GetMatrix() const {
        if (m_MatrixDirty) {
            RecalculateMatrix();
         }
         return m_ModelMatrix;
    }
    void Transform::RecalculateMatrix() const
    {
        const glm::mat4 rotation =
            glm::rotate(glm::mat4(1.0f), m_Rotation.x, { 1,0,0 }) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.y, { 0,1,0 }) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.z, { 0,0,1 });

        m_ModelMatrix =
            glm::translate(glm::mat4(1.0f), m_Translation) *
            rotation *
            glm::scale(glm::mat4(1.0f), m_Scale);

        m_MatrixDirty = false;
    }

    const float* Transform::GetTransform() const {
        if (m_MatrixDirty)
            RecalculateMatrix();

        return glm::value_ptr(m_ModelMatrix);
    }

    void Transform::SetRotation(const glm::vec3& angles) {
        m_Rotation = glm::radians(angles);
        m_MatrixDirty = true;
    }

    void Transform::SetTranslation(const glm::vec3& translation) {
        m_Translation = translation;
        m_MatrixDirty = true;
    }

    void Transform::SetScale(const glm::vec3& scale) {
        m_Scale = scale;
        m_MatrixDirty = true;
    }

    void Transform::IncrementRotation(const glm::vec3& delta) {
        m_Rotation += glm::radians(delta);
        m_MatrixDirty = true;
    }

    void Transform::IncrementTranslation(const glm::vec3& delta) {
        m_Translation += delta;
        m_MatrixDirty = true;
    }

    void Transform::IncrementScale(const glm::vec3& delta) {
        m_Scale += delta;
        m_MatrixDirty = true;
    }
}