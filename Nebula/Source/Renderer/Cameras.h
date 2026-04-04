//
// Created by Christian Cruz on 3/15/26.
//

#ifndef CAMERAS_H
#define CAMERAS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Nebula {

    class Camera {
        public:
            Camera() = default;
            explicit Camera(glm::vec3 position);

            void SetPosition(const glm::vec3& pos);
            void SetRotation(const glm::vec3& rot);
            void SetAspectRatio(float aspect);

            [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
            [[nodiscard]] glm::vec3 GetRotation() const { return m_Rotation; }
            [[nodiscard]] glm::mat4 GetViewMatrix();
            [[nodiscard]] glm::mat4 GetProjection();
            [[nodiscard]] glm::mat4 GetViewProjectionMatrix();

        private:
            void RecalculateMatrices();

            float fov{ 45.0f };
            float m_Aspect{};
            float nearClip{ 0.1f };
            float farClip{ 1000.0f };
            glm::vec3 m_Position{};
            glm::vec3 m_Rotation{};
            glm::vec3 m_Front{0.0f, 0.0f, -1.0f};
            glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
            glm::mat4 m_projectionMatrix{};
            glm::mat4 m_viewMatrix{};
            glm::mat4 m_viewProjectionMatrix{};
    };

    class OrthographicCamera {
        public:
        OrthographicCamera(float left, float right, float bottom, float top);

        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
        [[nodiscard]] float GetRotation() const { return m_Rotation; }
        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
        [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        void SetPosition(const glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }
        void SetRotation(const float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        private:
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_viewProjectionMatrix{};

        glm::vec3 m_Position{};
        float m_Rotation = 0.0f;

        void RecalculateViewMatrix();
    };
}


#endif //CAMERAS_H
