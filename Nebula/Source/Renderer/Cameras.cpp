//
// Created by Christian Cruz on 3/15/26.
//

#include "Cameras.h"

namespace Nebula {

    Camera::Camera(glm::vec3 position)
        : m_Position(position)
    {
        RecalculateMatrices();
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        m_viewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        return m_viewMatrix;
    }

    glm::mat4 Camera::GetProjection()
    {
        m_projectionMatrix = glm::perspectiveRH_ZO(glm::radians(fov), m_Aspect, nearClip, farClip);
        return m_projectionMatrix;
    }

    glm::mat4 Camera::GetViewProjectionMatrix(){
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        return m_viewProjectionMatrix;
    }

    void Camera::SetPosition(const glm::vec3 &pos) {
        m_Position = pos;
        RecalculateMatrices();
    }

    void Camera::SetRotation(const glm::vec3 &rot) {
        m_Rotation = rot;
        RecalculateMatrices();
    }

    void Camera::SetAspectRatio(float aspect) {
        m_Aspect = aspect;
        RecalculateMatrices();
    }

    void Camera::RecalculateMatrices() {
        float yaw   = glm::radians(m_Rotation.y);
        float pitch = glm::radians(m_Rotation.x);

        m_Front = glm::normalize(glm::vec3(
            cos(pitch) * sin(yaw),
            sin(pitch),
            cos(pitch) * cos(yaw)
        ));

        m_viewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

        float aspect = (m_Aspect > 0.f) ? m_Aspect : 16.f / 9.f;

        m_projectionMatrix = glm::perspectiveRH_ZO(glm::radians(fov), aspect, nearClip, farClip);


        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }



    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));

        m_viewMatrix = glm::inverse(transform);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

}
