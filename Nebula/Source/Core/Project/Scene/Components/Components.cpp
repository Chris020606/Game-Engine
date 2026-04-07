//
// Created by Christian Cruz on 4/5/26.
//

#include "Components.h"

namespace Nebula {
    TransformComponent::TransformComponent()
    : m_Rotation(0.0f),
      m_Translation(0.0f),
      m_Scale(1.0f),
      m_ModelMatrix(1.0),
      m_MatrixDirty(true) {
    }

    TransformComponent::operator glm::mat4() const {
        return GetMatrix();
    }

    glm::mat4 TransformComponent::GetMatrix() const {
        if (m_MatrixDirty) {
             glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_Translation);
             glm::mat4 rotMatrix = glm::eulerAngleXYZ(m_Rotation.x, m_Rotation.y, m_Rotation.z);
             glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
             m_ModelMatrix = transMatrix * rotMatrix * scaleMatrix;
             m_MatrixDirty = false;
         }
         return m_ModelMatrix;
            // glm::mat4 rotation =
            //     glm::yawPitchRoll(
            //         glm::radians(m_Rotation.y),
            //         glm::radians(m_Rotation.x),
            //         glm::radians(m_Rotation.z)
            //     );
            //
            // return glm::translate(glm::mat4(1.0f), m_Translation)
            //      * rotation
            //      * glm::scale(glm::mat4(1.0f), m_Scale);

    }
    void TransformComponent::RecalculateMatrix() const
    {
        glm::mat4 rotation =
            glm::rotate(glm::mat4(1.0f), m_Rotation.x, { 1,0,0 }) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.y, { 0,1,0 }) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.z, { 0,0,1 });

        m_ModelMatrix =
            glm::translate(glm::mat4(1.0f), m_Translation) *
            rotation *
            glm::scale(glm::mat4(1.0f), m_Scale);

        m_MatrixDirty = false;
    }

    float* TransformComponent::GetTransform() {
        if (m_MatrixDirty)
            RecalculateMatrix();

        return glm::value_ptr(m_ModelMatrix);
    }

    void TransformComponent::SetRotation(const glm::vec3& euler) {
        m_Rotation = glm::radians(euler);
        m_MatrixDirty = true;
    }

    void TransformComponent::SetTranslation(const glm::vec3& translation) {
        m_Translation = translation;
        m_MatrixDirty = true;
    }

    void TransformComponent::SetScale(const glm::vec3& scale) {
        m_Scale = scale;
        m_MatrixDirty = true;
    }

    void TransformComponent::IncrementRotation(const glm::vec3& delta) {
        m_Rotation += glm::radians(delta);
        m_MatrixDirty = true;
    }

    void TransformComponent::IncrementTranslation(const glm::vec3& delta) {
        m_Translation += delta;
        m_MatrixDirty = true;
    }

    void TransformComponent::IncrementScale(const glm::vec3& delta) {
        m_Scale += delta;
        m_MatrixDirty = true;
    }
}